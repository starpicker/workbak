#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "opus.h"  
#include "opus_types.h"  
#include "opus_multistream.h"

#include "opuscodec.h"

#include "assert.h"
#define		BIT_RATE 		48000
#define		SAMPLE_RATE		48000
#define		CHANNEL_NUM		2

#define 	MAX_PACKET 		2000
#define		FRAME_SIZE		480 //samples


void OpusCodec::HandlePCMData(unsigned char* pAudioData, int nLen)
{
	
    int m_iRemain = 0;
    int CHANNELS = 2;
    
    
	//保存压缩后的数据，保存数据为char
	if(nLen + m_iRemain < FRAME_SIZE * CHANNELS * sizeof(short))
	{
        assert(0);
		memcpy(m_pAudioBuffer + m_iRemain,pAudioData,nLen);
		m_iRemain += nLen;
        //Sleep(10);
		return ;
	}
	else
	{
		//Sleep(5);
	}

	int max_frame_size = FRAME_SIZE;
	short* in = (short*)malloc(max_frame_size*CHANNELS*sizeof(short));
	short* out = (short*)malloc(max_frame_size*CHANNELS*sizeof(short));
	unsigned char* fbytes = (unsigned char*)malloc(max_frame_size*CHANNELS*sizeof(short));
	unsigned char* data = (unsigned char*)calloc(MAX_PACKET,sizeof(char));
	int iNeedCopy = FRAME_SIZE * CHANNELS * sizeof(short) - m_iRemain;
	memset(fbytes,0,max_frame_size*CHANNELS*sizeof(short));
    if(m_iRemain)
    {
        assert(0);
        memcpy(fbytes,m_pAudioBuffer,m_iRemain);
    }

    
	memcpy(fbytes + m_iRemain,pAudioData,iNeedCopy);
	memset(m_pAudioBuffer,0,FRAME_SIZE * CHANNELS * sizeof(short));
	
	int readLen = (m_iRemain + iNeedCopy) / (sizeof(short) * 2);
	if(nLen - iNeedCopy > 0)
	{
        assert(0);
		memcpy(m_pAudioBuffer,pAudioData + iNeedCopy,nLen - iNeedCopy);
		m_iRemain = nLen - iNeedCopy;
	}
	
	for(int i=0;i<readLen*CHANNELS;i++)
	{
		opus_int32 s;
		s=fbytes[2*i+1]<<8|fbytes[2*i];
		s=((s&0xFFFF)^0x8000)-0x8000;
		in[i+0*CHANNELS]=s;
	}

	if (readLen < FRAME_SIZE)
	{
        assert(0);
		for (int i=readLen*CHANNELS;i<FRAME_SIZE*CHANNELS;i++)
			in[i] = 0;
	}
	int i_realLen = opus_encode(m_pEnc,in,FRAME_SIZE,data,MAX_PACKET);
	//opus_uint32 range = 0;
	//opus_encoder_ctl(m_pEnc,OPUS_GET_FINAL_RANGE(&range));
    
        FILE* fopus = fopen("decoder_test_sequence.opus.2", "a");
        fwrite(&i_realLen, sizeof(i_realLen), 1, fopus);
        fwrite(data, i_realLen, 1, fopus);
        fclose(fopus);
        
        assert(!m_iRemain);
        
}

OpusCodec::OpusCodec()
{
	m_pEnc = NULL;
	m_pDec = NULL;  	

	m_pAudioBuffer = NULL;
	m_nRemain = 0;
}

OpusCodec::~OpusCodec()
{
	Uninit();
}

int OpusCodec::Init()
{
	int err = 0;  
	opus_int32 skip = 0;  
	
	if(m_pEnc != NULL)
		return 0;
	
	m_pEnc = opus_encoder_create(SAMPLE_RATE, CHANNEL_NUM, OPUS_APPLICATION_VOIP, &err);
	///m_pEnc = opus_encoder_create(SAMPLE_RATE, CHANNEL_NUM, OPUS_APPLICATION_RESTRICTED_LOWDELAY, &err);
	if (err != OPUS_OK) {  
		fprintf(stderr, "cannnot create opus encoder: %s\n", opus_strerror(err));
		m_pEnc = NULL;  
		return -1;  
	}  
	
	opus_encoder_ctl(m_pEnc, OPUS_SET_BITRATE(BIT_RATE));
	opus_encoder_ctl(m_pEnc, OPUS_SET_BANDWIDTH(OPUS_AUTO));
	opus_encoder_ctl(m_pEnc, OPUS_SET_VBR(1));
	opus_encoder_ctl(m_pEnc, OPUS_SET_VBR_CONSTRAINT(0));
	opus_encoder_ctl(m_pEnc, OPUS_SET_COMPLEXITY(10));
	opus_encoder_ctl(m_pEnc, OPUS_SET_INBAND_FEC(0));
	opus_encoder_ctl(m_pEnc, OPUS_SET_FORCE_CHANNELS(OPUS_AUTO));
	opus_encoder_ctl(m_pEnc, OPUS_SET_DTX(0));
	opus_encoder_ctl(m_pEnc, OPUS_SET_PACKET_LOSS_PERC(0));
	opus_encoder_ctl(m_pEnc, OPUS_GET_LOOKAHEAD(&skip));
	opus_encoder_ctl(m_pEnc, OPUS_SET_LSB_DEPTH(16));
	//opus_encoder_ctl(m_pEnc, OPUS_SET_EXPERT_FRAME_DURATION(5000));//1.1.1 use dont know why
	
	//create audio buffer
	m_pAudioBuffer = (unsigned char*)malloc(FRAME_SIZE * CHANNEL_NUM * 2);
	memset(m_pAudioBuffer, 0, FRAME_SIZE * CHANNEL_NUM * 2);
	return 0;  

}

int OpusCodec::Uninit()
{
	if(m_pEnc != NULL)
	{
		opus_encoder_destroy(m_pEnc);  
		m_pEnc = NULL;

		return 0;
	}

	if(m_pAudioBuffer != NULL)
	{
		free(m_pAudioBuffer);
		m_pAudioBuffer = NULL;
		m_nRemain = 0;
	}
	return -1;
}

int OpusCodec::Encode(unsigned char* src, int srcLen, unsigned char* dst, int dstLen)
{
	if(m_pEnc == NULL)
		return 0;
	
    HandlePCMData(src, srcLen*4);
    return 0;
    
	short* in = (short*)malloc(FRAME_SIZE * CHANNEL_NUM * sizeof(short));
	int readLen = (FRAME_SIZE * CHANNEL_NUM * sizeof(short)) / (sizeof(short) * 2);
    //printf("OpusCodec::Encode readLen :%d\n", readLen);
        
	for(int i=0; i < readLen*CHANNEL_NUM; i++)
	{
		opus_int32 s;
		s = src[2*i+1]<<8 | src[2*i];
		s = ((s&0xFFFF)^0x8000)-0x8000;
		in[i + 0*CHANNEL_NUM] = s;
	}

	if (readLen < FRAME_SIZE)
	{
		for (int i=readLen*CHANNEL_NUM; i < FRAME_SIZE*CHANNEL_NUM; i++)
			in[i] = 0;
	}
	
   // printf("OpusCodec::Encode srcLen :%d\n", srcLen);
    
	int ret = opus_encode(m_pEnc, in, srcLen, dst, dstLen);
	
	//opus_uint32 enc_final_range;
	//opus_encoder_ctl(m_pEnc, OPUS_GET_FINAL_RANGE(&enc_final_range));
	
	//free(in);
	
	return ret;
	/*
	//保存压缩后的数据，保存数据为char
	if(srcLen + m_nRemain < FRAME_SIZE * CHANNEL_NUM * 2)
	{
		memcpy(m_pAudioBuffer + m_nRemain, src, srcLen);
		m_nRemain += srcLen;

		return 0;
	}
	else
	{

	}
	
	int max_frame_size = FRAME_SIZE * CHANNEL_NUM * sizeof(short);
	short* in = (short*)malloc(max_frame_size);
	//short* out = (short*)malloc(max_frame_size);
	unsigned char* fbytes = (unsigned char*)malloc(max_frame_size);
	
	int iNeedCopy = 0;
	int nbytes = 0;
	int nLeft = srcLen;
	unsigned char* pcm = src;
	while(true)
	{
		iNeedCopy = max_frame_size - m_nRemain;
		memset(fbytes, 0, max_frame_size);
		
		if(m_nRemain > 0)
			memcpy(fbytes, m_pAudioBuffer, m_nRemain);
		
		if(iNeedCopy > 0)
			memcpy(fbytes + m_nRemain, pcm, iNeedCopy);
		
		memset(m_pAudioBuffer, 0, max_frame_size);
		
		int ret = opus_encode(m_pEnc, (opus_int16 *)fbytes, FRAME_SIZE, dst, dstLen);
		if(ret < 0)
		{
			nbytes = ret;
			break;
		}
		nbytes += ret;
		
		opus_uint32 range = 0;
		opus_encoder_ctl(m_pEnc, OPUS_GET_FINAL_RANGE(&range));
		
		nLeft -= iNeedCopy;
		if(nLeft > 0)
		{
			if( nLeft < max_frame_size)
			{
				m_nRemain = nLeft;
				memcpy(m_pAudioBuffer, pcm + iNeedCopy, m_nRemain);
	
				break;
			}
			else
			{
				m_nRemain = 0;
				//memcpy(m_pAudioBuffer, pcm + iNeedCopy, m_nRemain);
				pcm += iNeedCopy;
			}
		}
		else
		{
			break;
		}
	}
	
	if(in)
		free(in);
	if(fbytes)
		free(fbytes);
	
	return nbytes;
		*/
}
	
int OpusCodec::Decode(unsigned char* src, int srcLen, unsigned char* dst, int dstLen)
{
	return 0;
}
