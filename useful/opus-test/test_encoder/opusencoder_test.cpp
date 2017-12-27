#include "OpusCodec.h"

#include "stdlib.h"
#include <unistd.h>
#include "mltime.h"
#define RAWFILENAME "DUMP_AUDIO_RAW.raw"
#define OPUS_SEQUENCE "decoder_test_sequence.opus"

int main(int argc, char** argv)
{
    printf("main in\n");
    
    int channels = 2;
	int sample_rate = 48000;
    
	unsigned char* inputBuf = 0;
    
    #define INPUTBUFFER (480)
    #define OUTPUTBUFFER (480)
    
	inputBuf = (unsigned char*)malloc(INPUTBUFFER*sizeof(short)*channels);
	if(inputBuf == 0) {
		printf("malloc raw inputBuf error\n");
		exit(0);
	}

    printf("new OpusCodec before\n");
	OpusCodec* encoder = new OpusCodec();
    printf("encoder is %p\n", encoder);
    encoder->Init();
    
    unlink(OPUS_SEQUENCE);
    unlink("decoder_test_sequence.opus.2");
    
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

    int dataSize = 0;
	while(fread(inputBuf, INPUTBUFFER*channels*sizeof(short), 1, fraw) > 0) {
        
        encoderOutDataSize =  encoder->Encode(inputBuf, INPUTBUFFER, opusBuffer, OUTPUTBUFFER);
        //printf("encoderOutDataSize is %d\n", encoderOutDataSize);
            
        //#define XWU_TEST
        
		#ifdef XWU_TEST
			             
			unsigned char* p =  (unsigned char*)(&encoderOutDataSize);
			dataSize = (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + (p[3] << 0);
            encoderOutDataSize = dataSize;
			
		#endif
        
        fwrite(&encoderOutDataSize, sizeof(encoderOutDataSize), 1, fopus);
        fwrite(opusBuffer, encoderOutDataSize, 1, fopus);
        
	}

    encoder->Uninit();
	delete encoder;
	encoder = 0;
	free(opusBuffer);
	opusBuffer = 0;
	free(inputBuf);
	inputBuf = 0;
	fclose(fopus);
    fopus = 0;
    fclose(fraw);
    fraw = 0;

	return 0;
}


