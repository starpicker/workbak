#ifndef __OPUSCODEC_H__
#define __OPUSCODEC_H__

struct OpusEncoder;
struct OpusDecoder;

class OpusCodec 
{
	
public:	
	OpusCodec();
	~OpusCodec();
	
	int Init();
	int Uninit();

	int Encode(unsigned char* src, int srcLen, unsigned char* dst, int dstLen);
	int Decode(unsigned char* src, int srcLen, unsigned char* dst, int dstLen);
    void HandlePCMData(unsigned char* pAudioData, int nLen);
	
private:
	
	OpusEncoder* m_pEnc;
	OpusDecoder* m_pDec;  

	unsigned char* m_pAudioBuffer;
	int	m_nRemain;
};

#endif //__OPUSCODEC_H__
