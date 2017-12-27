#include "opusdecoder.h"

#include <unistd.h>
#include "mltime.h"
#define RAWFILENAME "DUMP_AUDIO_RAW.raw"
#define OPUS_SEQUENCE "decoder_test_sequence.opus"

int main(int argc, char** argv)
{
    printf("main in\n");
	void* buf = 0;
	buf = malloc(48000 * 2 * 2);
	if(buf == 0) {
		printf("malloc raw buf error\n");
		exit(0);
	}

	int channels = 2;
	int sample_rate = 48000;
    printf("new OpusDecoder before\n");
	OpusDecoder* decoder = new OpusDecoder(sample_rate, channels);
    printf("decoder is %p\n", decoder);
    
    unlink(RAWFILENAME);
    FILE* fpRaw = fopen(RAWFILENAME, "a");
    if (fpRaw == 0) {
        printf("Error: error in open(%s)\n", RAWFILENAME);
        return -1;
    }
                    
	FILE* pOpusFile = 0;
	pOpusFile = fopen(OPUS_SEQUENCE, "r");
	if(pOpusFile == 0) {
		printf("open opus test sequence error\n");
		exit(0);
	}
    
    
	int i;
	int dataSize = 0;
	int decoderOutDataSize = 0;
	int opusBufferUsedSize = 0;
	unsigned char* opusBuffer = NULL;
	opusBuffer = (unsigned char*) malloc(sizeof(unsigned char)*(1275*3+7));
	if(opusBuffer == NULL) {
		printf("malloc opusBuffer error\n");
		exit(0);
	}

    char temp[8];
	while(fread(&dataSize, sizeof(int), 1, pOpusFile) > 0) {
        
        

        
		//#define XWU_TEST
		
		#ifdef XWU_TEST
			
			unsigned char* p =  (unsigned char*)(&dataSize);
			dataSize = (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + (p[3] << 0);
			printf("dataSize is %d\n", dataSize);
			
		#else //XWU_TEST
		
			printf("dataSize is %d\n", dataSize);
			
			#if 1 //arc presenter way
			fread(temp, 8, 1, pOpusFile);
			dataSize -= 8;
			#endif 
			
		#endif
		
		fread(opusBuffer, sizeof(unsigned char), dataSize, pOpusFile);

        
        #if 0//mobile camera way
		if(decoder->DecodeFrame(
				opusBuffer,
				dataSize,
				&opusBufferUsedSize,
				(unsigned char*)buf,
				sizeof(short) * OPUS_MAX_FRAME_SIZE * channels,
				&decoderOutDataSize) != 0) {
			printf("audio decode is failure\n");
        #else //arc presenter way
        
        MLTIME* t = new MLTIME("DecodeFrame4ArcPresenter");
        
        if(decoder->DecodeFrame4ArcPresenter(
				opusBuffer,
				dataSize,
				&opusBufferUsedSize,
				(unsigned char*)buf,
				sizeof(short) * OPUS_MAX_FRAME_SIZE * channels,
				&decoderOutDataSize) != 0) {
			printf("audio decode is failure\n");
        #endif
        
		} else {
                delete t;
                printf("audio decoded out size: %d\n", decoderOutDataSize);
        #if 0//mobile camera way
                fwrite(buf, decoderOutDataSize*2, 1, fpRaw);
        #else //arc presenter way
                fwrite(buf, decoderOutDataSize, 1, fpRaw);
        #endif
		}
	}

	delete decoder;
	decoder = 0;
	free(opusBuffer);
	opusBuffer = 0;
	free(buf);
	buf = 0;
	fclose(fpRaw);
    fpRaw = 0;
    fclose(pOpusFile);
    pOpusFile = 0;

	return 0;
}


