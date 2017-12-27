#ifndef OPUSDECODER_H__
#define OPUSDECODER_H__

#include <stdlib.h>
#include <stdio.h>

#ifdef	__cplusplus
extern "C" {
#endif
#include "opus.h"
#include "opus_multistream.h"
#include "opus_types.h"
#ifdef	__cplusplus
}
#endif
#include "math.h"

#ifndef float2int(flt)
#ifdef HAVE_LRINTF
# define float2int(x) lrintf(x)
#else
# define float2int(flt) ((int)(floor(.5+flt)))
#endif

#ifndef HAVE_FMINF
# define fminf(_x,_y) ((_x)<(_y)?(_x):(_y))
#endif

#ifndef HAVE_FMAXF
# define fmaxf(_x,_y) ((_x)>(_y)?(_x):(_y))
#endif

#if !defined(__LITTLE_ENDIAN__) && ( defined(WORDS_BIGENDIAN) || defined(__BIG_ENDIAN__) )
#define le_short(s) ((short) ((unsigned short) (s) << 8) | ((unsigned short) (s) >> 8))
#define be_short(s) ((short) (s))
#else
#define le_short(s) ((short) (s))
#define be_short(s) ((short) ((unsigned short) (s) << 8) | ((unsigned short) (s) >> 8))
#endif 

#define MINI(_a,_b)      ((_a)<(_b)?(_a):(_b))
#define MAXI(_a,_b)      ((_a)>(_b)?(_a):(_b))
#endif

class OpusDecoder
{

/* 120ms at 48000 */
#define OPUS_MAX_FRAME_SIZE (960*6)

#define ARCPRESENTER_MAX_FRAME_SIZE (480)
#define ARCPRESENTER_MAX_PACKET (2000)

public: 
	OpusDecoder(int sampling_rate, int channel);
	~OpusDecoder();
    
	int DecodeFrame(unsigned char *pIn, int lInSize, int *plInSize, 
		unsigned char *pOut, int lOutBufferSize, int *plOutSize);
        
    int DecodeFrame4ArcPresenter(unsigned char *pIn, int lInSize, int *plInSize, 
		unsigned char *pOut, int lOutBufferSize, int *plOutSize);
        
	int SetGain (int value);
	int GetGain (int* value);

private:
    //mobile camera way
	OpusMSDecoder		*mDec;
    
    //arc presenter way
    OpusDecoder*        m_pDecoder;
    short*              decOut;
    
	int					sample_size;

	int					sampling_rate;
	int					channels;
	int					nb_streams;
	int					nb_coupled;
	unsigned char		stream_map[255];

	//shapestate
	int					shapemem;
	float *				b_buf;
	float *				a_buf;
	int					fs;
	int					mute;
	unsigned int        rngseed;
	int                 outBufferSizeLimit;

	// resampler
	/*int					hasResampler;
	SpeexResamplerState *resampler;
    float				*bufs;
    int					bufpos;
    int					bufsize;
    int					done;*/

	unsigned int fast_rand(); 
	int classMemberInit();
	int setDefaultParam();
	int setConfigure(int channels);
	void shape_dither_toshort(short *_o, float *_i,int _n,int _CC);
}; 

#endif // OPUSDECODER_H__