#include "opusdecoder.h"

#include <limits.h>
#include <string.h>
#define printf
OpusDecoder::OpusDecoder(int sample_rate, int channel)
:sample_size(16), sampling_rate(sample_rate), channels(channel),
nb_streams(1), nb_coupled(1), shapemem(1), mute(960), rngseed(22222)
{
	stream_map[0] = 0;
	stream_map[1] = 1;
    fs = sampling_rate;
        
	int err;
    
    printf("opus_multistream_decoder_create before\n");
    mDec = opus_multistream_decoder_create(sampling_rate, channels, nb_streams, nb_coupled, stream_map, &err);
    printf("opus_multistream_decoder_create after\n");
    
    if(err != OPUS_OK)
    {
        fprintf(stderr, "OpusDecoder Cannot create decoder: %s\n", opus_strerror(err));
        //throw
	}
    else
    {
        printf("OpusMSDecoder mDec is %p", mDec);
    }

	a_buf=(float*)calloc(channels, sizeof(float)*4);
    b_buf=(float*)calloc(channels, sizeof(float)*4);
    //check a_buf and b_buf and throw
    
    outBufferSizeLimit = sizeof(short) * OPUS_MAX_FRAME_SIZE * channels;
    
    m_pDecoder = opus_decoder_create(sampling_rate,channels,&err);
    decOut = (short*)malloc(ARCPRESENTER_MAX_FRAME_SIZE*channels*sizeof(short));
}

OpusDecoder::~OpusDecoder()
{
	if(mDec != 0)
		opus_multistream_decoder_destroy(mDec);
	free(a_buf);
	free(b_buf);
    
	if (m_pDecoder)
	{
        opus_decoder_destroy(m_pDecoder);
        m_pDecoder = NULL;
	}
    free(decOut);
}

int OpusDecoder::DecodeFrame4ArcPresenter(unsigned char *pIn, int lInSize, int *plInSize, 
    unsigned char *pOut, int lOutBufferSize, int *plOutSize)
{
    int max_frame_size = ARCPRESENTER_MAX_FRAME_SIZE;
        
    if(lOutBufferSize < max_frame_size*channels*sizeof(short)) return -1;
	
	int output_samples = opus_decode(m_pDecoder, pIn, lInSize, decOut, max_frame_size, 0);
    printf("opus_decode output_samples is %d\n", output_samples);
    
	for(int i = 0; i< output_samples*channels; i++)
	{
        short s;
        s = decOut[i];
        pOut[2*i]=s&0xFF;
        pOut[2*i+1]=(s>>8)&0xFF;
	}
    
    *plOutSize = max_frame_size*channels*sizeof(opus_int16);

    return 0;
}
        
int OpusDecoder::DecodeFrame(unsigned char *pIn, int lInSize, int *plInSize, 
		unsigned char *pOut, int lOutBufferSize, int *plOutSize)
{
	if(mDec == 0) 
    {
		fprintf(stderr, "OpusMSDecoder mDec is 0\n");
		return -1;								
	}

	if(lOutBufferSize < outBufferSizeLimit)
    {
		fprintf(stderr, "lOutBufferSize need larger than sizeof(short)*OPUS_MAX_FRAME_SIZE*channels.\n");
		return -1;
	}

	float* pcm=(float*)alloca(outBufferSizeLimit);
	
	*plOutSize = opus_multistream_decode_float(mDec, pIn, lInSize, pcm, OPUS_MAX_FRAME_SIZE, 0)*sample_size/8;
	*plInSize = lInSize;
	
	int frame_size = *plOutSize/(sample_size/8);
	opus_int64 sampout=0;
	int i,ret,tmp_skip;
	int* skip = 0;
	unsigned out_len;
	short *out;
	float *output;
	out=(short*)alloca(outBufferSizeLimit);

	do {
		if (skip){
			tmp_skip = (*skip>frame_size) ? (int)frame_size : *skip;
			*skip -= tmp_skip;
		} else {
			tmp_skip = 0;
		}
     
		output=pcm+channels*tmp_skip;
		out_len=frame_size-tmp_skip;
		frame_size=0;
     

		/*Convert to short and save to output file*/
		if (shapemem){
			shape_dither_toshort(out,output,out_len,channels);
		}else{
		for (i=0;i<(int)out_len*channels;i++)
			out[i]=(short)float2int(fmaxf(-32768,fminf(output[i]*32768.f,32767)));
		}

		if ((le_short(1)!=1)){
			for (i=0;i<(int)out_len*channels;i++)
					out[i]=le_short(out[i]);
		}
	}while (frame_size>0);

	memcpy(pOut, (unsigned char*)out, sizeof(short)*2*(int)out_len*channels);

	//*plOutSize = *plOutSize * 2;
	return 0;
}

int OpusDecoder::SetGain(int value)
{
	int ret = opus_multistream_decoder_ctl(mDec, OPUS_SET_GAIN(value));
    
	if(ret != OPUS_OK){
		fprintf(stderr,"Error SetGain returned: %s\n",opus_strerror(ret));
		return ret;
	}

	return 0;
}

int OpusDecoder::GetGain(int* value)
{
    if(!value) return -1;
	int ret = opus_multistream_decoder_ctl(mDec, OPUS_GET_GAIN(value));
    
	if(ret!=OPUS_OK){
		fprintf(stderr,"Error GetGain returned: %s\n",opus_strerror(ret));
		return ret;
	}
	return 0;
}

unsigned int OpusDecoder::fast_rand()
{
	rngseed = (rngseed * 96314165) + 907633515;
	return rngseed;
}


/* This implements a 16 bit quantization with full triangular dither
   and IIR noise shaping. The noise shaping filters were designed by
   Sebastian Gesemann based on the LAME ATH curves with flattening
   to limit their peak gain to 20 dB.
   (Everyone elses' noise shaping filters are mildly crazy)
   The 48kHz version of this filter is just a warped version of the
   44.1kHz filter and probably could be improved by shifting the
   HF shelf up in frequency a little bit since 48k has a bit more
   room and being more conservative against bat-ears is probably
   more important than more noise suppression.
   This process can increase the peak level of the signal (in theory
   by the peak error of 1.5 +20 dB though this much is unobservable rare)
   so to avoid clipping the signal is attenuated by a couple thousandths
   of a dB. Initially the approach taken here was to only attenuate by
   the 99.9th percentile, making clipping rare but not impossible (like
   SoX) but the limited gain of the filter means that the worst case was
   only two thousandths of a dB more, so this just uses the worst case.
   The attenuation is probably also helpful to prevent clipping in the DAC
   reconstruction filters or downstream resampling in any case.*/
void OpusDecoder::shape_dither_toshort( short *_o, float *_i, int _n, int _CC) {
  const float gains[3]={32768.f-15.f,32768.f-15.f,32768.f-3.f};
  const float fcoef[3][8] =
  {
    {2.2374f, -.7339f, -.1251f, -.6033f, 0.9030f, .0116f, -.5853f, -.2571f}, /* 48.0kHz noise shaping filter sd=2.34*/
    {2.2061f, -.4706f, -.2534f, -.6214f, 1.0587f, .0676f, -.6054f, -.2738f}, /* 44.1kHz noise shaping filter sd=2.51*/
    {1.0000f, 0.0000f, 0.0000f, 0.0000f, 0.0000f,0.0000f, 0.0000f, 0.0000f}, /* lowpass noise shaping filter sd=0.65*/
  };
  int i;
  int rate=fs==44100?1:(fs==48000?0:2);
  float gain=gains[rate];

  /*In order to avoid replacing digital silence with quiet dither noise
    we mute if the output has been silent for a while*/
  if(mute>64)
    memset(a_buf,0,sizeof(float)*_CC*4);
  for(i=0;i<_n;i++)
  {
    int c;
    int pos = i*_CC;
    int silent=1;
    for(c=0;c<_CC;c++)
    {
      int j, si;
      float r,s,err=0;
      silent&=_i[pos+c]==0;
      s=_i[pos+c]*gain;
      for(j=0;j<4;j++)
        err += fcoef[rate][j]*b_buf[c*4+j] - fcoef[rate][j+4]*a_buf[c*4+j];
      memmove(&a_buf[c*4+1],&a_buf[c*4],sizeof(float)*3);
      memmove(&b_buf[c*4+1],&b_buf[c*4],sizeof(float)*3);
      a_buf[c*4]=err;
      s = s - err;
      r=(float)fast_rand()*(1/(float)UINT_MAX) - (float)fast_rand()*(1/(float)UINT_MAX);
      if (mute>16)r=0;
      /*Clamp in float out of paranoia that the input will be >96 dBFS and wrap if the
        integer is clamped.*/
      _o[pos+c] = si = float2int(fmaxf(-32768,fminf(s + r,32767)));
      /*Including clipping in the noise shaping is generally disastrous:
        the futile effort to restore the clipped energy results in more clipping.
        However, small amounts-- at the level which could normally be created by
        dither and rounding-- are harmless and can even reduce clipping somewhat
        due to the clipping sometimes reducing the dither+rounding error.*/
      b_buf[c*4] = (mute>16)?0:fmaxf(-1.5f,fminf(si - s,1.5f));
    }
    mute++;
    if(!silent)mute=0;
  }
  mute=MINI(mute,960);
}