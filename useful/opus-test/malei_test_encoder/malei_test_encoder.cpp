#include "opus.h"  
#include "opus_types.h"  
#include "opus_multistream.h"  
  
#define SAMPLE_RATE 16000  
#define CHANNEL_NUM 1  
#define BIT_RATE 16000  
#define BIT_PER_SAMPLE 16  
#define WB_FRAME_SIZE 320  
#define DATA_SIZE 1024 * 1024 * 4  
  
int encode(char* in, int len, unsigned char* opus, int* opus_len) {  
    int err = 0;  
    opus_int32 skip = 0;  
  
    OpusEncoder *enc = opus_encoder_create(SAMPLE_RATE, CHANNEL_NUM,  
            OPUS_APPLICATION_VOIP, &err);  
    if (err != OPUS_OK) {  
        fprintf(stderr, "cannnot create opus encoder: %s\n",  
                opus_strerror(err));  
        enc = NULL;  
        return -1;  
    }  
  
    opus_encoder_ctl(enc, OPUS_SET_BANDWIDTH(OPUS_BANDWIDTH_WIDEBAND));  
    opus_encoder_ctl(enc, OPUS_SET_BITRATE(BIT_RATE));  
    opus_encoder_ctl(enc, OPUS_SET_VBR(1));  
    opus_encoder_ctl(enc, OPUS_SET_COMPLEXITY(10));  
    opus_encoder_ctl(enc, OPUS_SET_INBAND_FEC(0));  
    opus_encoder_ctl(enc, OPUS_SET_FORCE_CHANNELS(OPUS_AUTO));  
    opus_encoder_ctl(enc, OPUS_SET_DTX(0));  
    opus_encoder_ctl(enc, OPUS_SET_PACKET_LOSS_PERC(0));  
    opus_encoder_ctl(enc, OPUS_GET_LOOKAHEAD(&skip));  
    opus_encoder_ctl(enc, OPUS_SET_LSB_DEPTH(16));  
  
    short frame_size = WB_FRAME_SIZE;  
    int frame_bytes = (frame_size << 1);  
  
    opus_int16 *frame = (opus_int16 *) in;  
    unsigned char *cbits = opus;  
  
    while (len > frame_bytes) {  
        int nbytes = opus_encode(enc, frame, frame_size, cbits + sizeof(char),  
                640 - sizeof(short));  
        if (nbytes > frame_size * 2 || nbytes < 0) {  
            return -1;  
        }  
        cbits[0] = nbytes;  
        frame += WB_FRAME_SIZE;  
        cbits += nbytes + sizeof(char);  
        len -= frame_bytes;  
        *opus_len += nbytes + sizeof(char);  
    }  
    opus_encoder_destroy(enc);  
    return 0;  
}  

#include "stdlib.h"
#include <unistd.h>
#include "mltime.h"
#define RAWFILENAME "DUMP_AUDIO_RAW.raw"
#define OPUS_SEQUENCE "decoder_test_sequence.opus"

int main(int argc, char** argv)
{
    
    unsigned char* inputBuf = 0;
    
    #define INPUTBUFFER (480)
    #define OUTPUTBUFFER (480)
    
	inputBuf = (unsigned char*)malloc(INPUTBUFFER*sizeof(short)*channels);
	if(inputBuf == 0) {
		printf("malloc raw inputBuf error\n");
		exit(0);
	}
    
        
    unlink(OPUS_SEQUENCE);
    
    FILE* fopus = fopen(OPUS_SEQUENCE, "a");
    if (fopus == 0) {
        printf("Error: error in open(%s)\n", OPUS_SEQUENCE);
        return -1;
    }
                    
	FILE* fraw = 0;
	fraw = fopen(RAWFILENAME, "r");
	if(fraw == 0) {
		printf("open opus test raw pcm error\n");
		exit(0);
	}
    
    
    int encoderOutDataSize = 0;
	int pcmBufferUsedSize = 0;
	unsigned char* opusBuffer = NULL;
	opusBuffer = (unsigned char*) malloc(sizeof(unsigned char)*(OUTPUTBUFFER));
	if(opusBuffer == NULL) {
		printf("malloc opusBuffer error\n");
		exit(0);
	}
    
    
    while(fread(inputBuf, INPUTBUFFER*channels*sizeof(short), 1, fraw) > 0)
    {
        #error "not now"
    }
    
    
    
    return 0;
}