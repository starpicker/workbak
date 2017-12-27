#include <jni.h>

#include "render.h"

Render r;

int main___(int argc, char** argv);

JNIEXPORT void JNICALL Init(JNIEnv *env, jobject jobj, jint previewWidth, jint previewHeigth)
{
    r.init(previewWidth, previewHeigth);
}

JNIEXPORT void JNICALL Release(JNIEnv *env, jobject jobj)
{
	r.unit();
}

JNIEXPORT void JNICALL ProcessData(JNIEnv *env, jobject jobj, jbyteArray dataArray)
{
	jbyte* srcData = env->GetByteArrayElements(dataArray, 0);
	jsize dataLength = env->GetArrayLength(dataArray);
    
	r.updateTextureFrame((unsigned char*)srcData);

	env->ReleaseByteArrayElements(dataArray, srcData, 0);
}

JNIEXPORT void JNICALL ResetPreviewSize(JNIEnv *env, jobject jobj, jint previewWidth, jint previewHeigth)
{
	r.setSize(previewWidth, previewHeigth);
}

JNIEXPORT void JNICALL onSurfaceCreated(JNIEnv *env, jobject jobj)
{
	r.onSurfaceCreated();
}

JNIEXPORT void JNICALL onSurfaceChanged(JNIEnv *env, jobject jobj, jint surfaceWidth, jint surfaceHeight)
{
	r.onSurfaceChanged(surfaceWidth, surfaceHeight);
}

JNIEXPORT void JNICALL onDrawFrame(JNIEnv *env, jobject jobj)
{
    main___(0, 0);
    return;
	r.onDrawFrame();
}

JNIEXPORT void JNICALL setMirror(JNIEnv *env, jobject jobj, jboolean bMirror)
{
	r.setMirror(bMirror);
}

static JNINativeMethod gMethods[] = {
	/* name,                            signature,											funcPtr */
	{"native_Init",						"(II)V",					(void*)Init},
	{"native_Release",					"()V",												(void*)Release},
	{"native_ProcessData",				"([B)V",											(void*)ProcessData},
	
	{"native_ResetPreviewSize",			"(II)V",											(void*)ResetPreviewSize},

	{"native_onSurfaceCreated",			"()V",												(void*)onSurfaceCreated},
	{"native_onSurfaceChanged",			"(II)V",											(void*)onSurfaceChanged},
	{"native_onDrawFrame",				"()V",												(void*)onDrawFrame},
	{"native_setMirror",				"(Z)V",												(void*)setMirror},
};

jint Register_Demo_Function( JNIEnv * env )
{ 
	jclass clazz = env->FindClass("com/arcsoft/rendernv21/rendernv21");
	if (clazz == 0)
	{
		return JNI_ERR;
	}

	jint res = env->RegisterNatives(clazz,gMethods,sizeof(gMethods) / sizeof(JNINativeMethod));

	if (res < 0)
	{
		return JNI_ERR;
	}
	return JNI_OK;
}

extern "C" JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = 0;
	jint result = -1;

	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return result;
	}

	if(Register_Demo_Function(env) != 0)
		return -1;

	return JNI_VERSION_1_4;
}

#include <stdio.h>
#include <string.h>

#include "amcomdef.h"
#include "merror.h"
#include "ammem.h"
#include "asvloffscreen.h"

#include "mdutils.h"
#include "amcm.h"
#include "amutil.h"

#define  FILE_FLAG			1
#define  MEMORY_FLAG		2
#define  ASYNC_FILE_FLAG	3

typedef struct _tag_file
{
	MPChar file_path;
	MPChar file_name;
}FILE_TAG;

typedef struct _tag_mem
{
	MLong size;
	MLong off_set;		//currently memory pointer
}MEM_TAG;

typedef struct _tag_STREAM_HANDLE
{
	void* handle;
	MLong flag;
	union {
		FILE_TAG file_pty;
		MEM_TAG  mem_pty;
	}pty;							

#ifdef PALM_DRM_SUPPORT
	MDWord dwLockType;//stream lock type,now support none lock,palm drm typy
	MDWord dwDrmHeaderLen;//record the length of lock header length
	MDWord dwIDLen;//the length of device ID
	MByte * pID;//the device id content
	MDWord dwWidthBase;// video width base
	MDWord dwHeightBase; //video height base
	MBool  bWriteImgDrmHeader;
#endif
	//Add for support drm and feature 
	MLong	paratype;
	MVoid*	para;

	//Add for present the stream size
	MLong			lDataLen;//the data valid length
	MLong			lTotalLen;

	//Add for async file I/O
#if		defined(__WIN32__) || defined(_WINCE_) || defined(__SYMBIAN32__) || defined(_LINUX_) ||(defined(_PALM_OS_) && defined(_PALM_OS_ADS_))
	THREAD_TAG* thWrite;
#endif
} STREAMHANDLE,*LPSTREAMHANDLE;

extern "C"
FILE *
fmemopen(void *buf, size_t size, const char *mode);

HMSTREAM	MStreamOpenFromStream(const MVoid *jpegStream, int jpegSize, MWord mode)
{
	LPSTREAMHANDLE hStream;
	FILE * fs ;
  
	MChar   rgMode[6][4] = {"rb", "wb", "ab", "rb+", "wb+", "ab+"};
	if(mode > 6 || mode < 1)
		mode = 1;
	fs	=	fmemopen((void*)jpegStream, jpegSize, rgMode[mode-1]);	
    
    
    printf("fs is %p\n", fs);
    
	if ( MNull == fs)
		return MNull;
	
    // TODO:
    // free it
	hStream = (LPSTREAMHANDLE)MMemAlloc(MNull,sizeof(STREAMHANDLE));
	if(MNull == hStream)
		return MNull;
	MMemSet(hStream,0,sizeof(STREAMHANDLE));

	hStream->handle = fs;
	hStream->flag   = FILE_FLAG;

	return hStream;	
}

MRESULT MBitmapAlloc(MBITMAP* bitmap);
MRESULT MBitmapFree(MBITMAP bitmap);
MLong MBitmapSize(MBITMAP bitmap);
MLong BUILD_ASVLOFFSCREEN_FROM_MBITMAP(MBITMAP* bitmap, ASVLOFFSCREEN *offscreen);
MLong BUILD_MBITMAP_FROM_ASVLOFFSCREEN(ASVLOFFSCREEN *offscreen, MBITMAP *bitmap);

MLong BUILD_ASVLOFFSCREEN_FROM_MBITMAP(MBITMAP* bitmap, ASVLOFFSCREEN *offscreen)
{
	offscreen->i32Width = bitmap->lWidth;
	offscreen->i32Height = bitmap->lHeight;
	offscreen->pi32Pitch[0] = bitmap->lPitch[0];
	offscreen->pi32Pitch[1] = bitmap->lPitch[1];
	offscreen->pi32Pitch[2] = bitmap->lPitch[2];
	
	switch(bitmap->dwPixelArrayFormat)
	{
	case MPAF_RGB32_B8G8R8A8:
		offscreen->u32PixelArrayFormat = ASVL_PAF_RGB32_B8G8R8A8;
		break;

	case MPAF_RGB32_A8R8G8B8:
		offscreen->u32PixelArrayFormat = ASVL_PAF_RGB32_A8R8G8B8;
		break;

	case MPAF_RGB32_R8G8B8:
		offscreen->u32PixelArrayFormat = ASVL_PAF_RGB32_R8G8B8;
		break;

	case MPAF_RGB32_B8G8R8:
		offscreen->u32PixelArrayFormat = ASVL_PAF_RGB32_B8G8R8;
		break;

	case MPAF_RGB16_B5G6R5:
		offscreen->u32PixelArrayFormat = ASVL_PAF_RGB16_B5G6R5;
		break;

	case MPAF_RGB16_R5G6B5:
		offscreen->u32PixelArrayFormat = ASVL_PAF_RGB16_R5G6B5;
		break;

	case MPAF_RGB24_R8G8B8:
		offscreen->u32PixelArrayFormat = ASVL_PAF_RGB24_R8G8B8;
		break;

	case MPAF_RGB24_B8G8R8:
		offscreen->u32PixelArrayFormat = ASVL_PAF_RGB24_B8G8R8;
		break;

	case MPAF_YUYV:
		offscreen->u32PixelArrayFormat = ASVL_PAF_YUYV;
		break;

	case MPAF_YVYU:
		offscreen->u32PixelArrayFormat = ASVL_PAF_YVYU;
		break;

	case MPAF_UYVY:
		offscreen->u32PixelArrayFormat = ASVL_PAF_UYVY;
		break;

	case MPAF_VYUY:
		offscreen->u32PixelArrayFormat = ASVL_PAF_VYUY;
		break;

	case MPAF_I420:
		offscreen->u32PixelArrayFormat = ASVL_PAF_I420;
		break;

	case MPAF_YV12:
		offscreen->u32PixelArrayFormat = ASVL_PAF_YV12;
		break;

	case MPAF_OTHERS_NV21:
	case MPAF_OTHERS_NV21 | MPAF_BT601_YCBCR:
		offscreen->u32PixelArrayFormat = ASVL_PAF_NV21;
		break;

	case MPAF_OTHERS_NV12:
		offscreen->u32PixelArrayFormat = ASVL_PAF_NV12;
		break;

	case MPAF_I422V:
		offscreen->u32PixelArrayFormat = ASVL_PAF_I422V;
		break;

	case MPAF_I422H:
		offscreen->u32PixelArrayFormat = ASVL_PAF_I422H;
		break;

	case MPAF_I444:
		offscreen->u32PixelArrayFormat = ASVL_PAF_I444;
		break;

	case MPAF_YUV:
		offscreen->u32PixelArrayFormat = ASVL_PAF_YUV;
		break;

	case MPAF_YVU:
		offscreen->u32PixelArrayFormat = ASVL_PAF_YVU;
		break;

	case MPAF_UVY:
		offscreen->u32PixelArrayFormat = ASVL_PAF_UVY;
		break;

	case MPAF_VUY:
		offscreen->u32PixelArrayFormat = ASVL_PAF_VUY;
		break;

	default:
		return MERR_UNSUPPORTED;
	}

	switch (bitmap->dwPixelArrayFormat)
	{
	case MPAF_OTHERS_NV21:
	case MPAF_OTHERS_NV21  | MPAF_BT601_YCBCR:
		offscreen->ppu8Plane[0] = bitmap->pPlane[0];
		offscreen->ppu8Plane[1] = bitmap->pPlane[2];
		offscreen->ppu8Plane[2] = MNull;
		break;

	case MPAF_OTHERS_NV12:
		offscreen->ppu8Plane[0] = bitmap->pPlane[0];
		offscreen->ppu8Plane[1] = bitmap->pPlane[1];
		offscreen->ppu8Plane[2] = bitmap->pPlane[1]+1;
		break;

	default:
		offscreen->ppu8Plane[0] = bitmap->pPlane[0];
		offscreen->ppu8Plane[1] = bitmap->pPlane[1];
		offscreen->ppu8Plane[2] = bitmap->pPlane[2];
		break;
	}

	return MOK;
}

MLong BUILD_MBITMAP_FROM_ASVLOFFSCREEN(ASVLOFFSCREEN *offscreen, MBITMAP *bitmap)
{
	bitmap->lHeight = offscreen->i32Height;
	bitmap->lWidth  = offscreen->i32Width;
	bitmap->lPitch[0] = offscreen->pi32Pitch[0];
	bitmap->lPitch[1] = offscreen->pi32Pitch[1];
	bitmap->lPitch[2] = offscreen->pi32Pitch[1];
	
	switch (offscreen->u32PixelArrayFormat)
	{
	case ASVL_PAF_RGB32_B8G8R8A8:
		bitmap->dwPixelArrayFormat = MPAF_RGB32_B8G8R8A8;
		break;

	case ASVL_PAF_RGB32_A8R8G8B8:
		bitmap->dwPixelArrayFormat = MPAF_RGB32_A8R8G8B8;
		break;
		
	case ASVL_PAF_RGB32_R8G8B8:
		bitmap->dwPixelArrayFormat = MPAF_RGB32_R8G8B8;
		break;
		
	case ASVL_PAF_RGB32_B8G8R8:
		bitmap->dwPixelArrayFormat = MPAF_RGB32_B8G8R8;
		break;

	case ASVL_PAF_RGB24_R8G8B8:
		bitmap->dwPixelArrayFormat = MPAF_RGB24_R8G8B8;
		break;
		
	case ASVL_PAF_RGB24_B8G8R8:
		bitmap->dwPixelArrayFormat = MPAF_RGB24_B8G8R8;
		break;
		
	case ASVL_PAF_RGB16_B5G6R5:
		bitmap->dwPixelArrayFormat = MPAF_RGB16_B5G6R5;
		break;
		
	case ASVL_PAF_RGB16_R5G6B5:
		bitmap->dwPixelArrayFormat = MPAF_RGB16_R5G6B5;
		break;
		
	case ASVL_PAF_YUYV:
		bitmap->dwPixelArrayFormat = MPAF_YUYV;
		break;

	case ASVL_PAF_YVYU:
		bitmap->dwPixelArrayFormat = MPAF_YVYU;
		break;
		
	case ASVL_PAF_UYVY:
		bitmap->dwPixelArrayFormat = MPAF_UYVY;
		break;
		
	case ASVL_PAF_VYUY:
		bitmap->dwPixelArrayFormat = MPAF_VYUY;
		break;
		
	case ASVL_PAF_I420:
		bitmap->dwPixelArrayFormat = MPAF_I420;
		break;

	case ASVL_PAF_YV12:
		bitmap->dwPixelArrayFormat = MPAF_YV12;
		break;
		
	case ASVL_PAF_NV21:
	case ASVL_PAF_NV21  | MPAF_BT601_YCBCR:
		bitmap->dwPixelArrayFormat = MPAF_OTHERS_NV21;
		break;

	case ASVL_PAF_NV12:
		bitmap->dwPixelArrayFormat = MPAF_OTHERS_NV12;
		break;

	case ASVL_PAF_I422V:
		bitmap->dwPixelArrayFormat = MPAF_I422V;
		break;
		
	case ASVL_PAF_I422H:
		bitmap->dwPixelArrayFormat = MPAF_I422H;
		break;
		
	case ASVL_PAF_I444:
		bitmap->dwPixelArrayFormat = MPAF_I444;
		break;
				
	case ASVL_PAF_YUV:
		bitmap->dwPixelArrayFormat = MPAF_YUV;
		break;
		
	case ASVL_PAF_YVU:
		bitmap->dwPixelArrayFormat = MPAF_YVU;
		break;
		
	case ASVL_PAF_UVY:
		bitmap->dwPixelArrayFormat = MPAF_UVY;
		break;
		
	case ASVL_PAF_VUY:
		bitmap->dwPixelArrayFormat = MPAF_VUY;
		break;

	default:
		return MERR_UNSUPPORTED;
	}

	switch (offscreen->u32PixelArrayFormat)
	{
	case ASVL_PAF_NV21:
		bitmap->pPlane[0]	= (MByte*)offscreen->ppu8Plane[0];
		bitmap->pPlane[2]    = (MByte*)offscreen->ppu8Plane[1];
		bitmap->pPlane[1]	= bitmap->pPlane[2] + 1;
		break;

	case ASVL_PAF_NV12:
		bitmap->pPlane[0] = (MByte*)offscreen->ppu8Plane[0];
		bitmap->pPlane[1] = (MByte*)offscreen->ppu8Plane[1];
		bitmap->pPlane[2] = (MByte*)bitmap->pPlane[1] + 1;
		break;

	default:
		bitmap->pPlane[0] = (MByte*)offscreen->ppu8Plane[0];
		bitmap->pPlane[1] = (MByte*)offscreen->ppu8Plane[1];
		bitmap->pPlane[2] = (MByte*)offscreen->ppu8Plane[2];
		break;
	}

	return MOK;
}

MRESULT MBitmapAlloc(MBITMAP* bitmap)
{
    MLong   lSize = 0;
    MLong   lBufferH = 0;
    MLong   lSubSampleV = 1;
    MByte*  pData = MNull;
    MMemSet(bitmap->lPitch, 0, MPAF_MAX_PLANES * sizeof(MInt32));
    MMemSet(bitmap->pPlane, MNull, MPAF_MAX_PLANES * sizeof(MByte*));

    switch(bitmap->dwPixelArrayFormat)
    {
    case MPAF_RGB32_B8G8R8A8:
    case MPAF_RGB32_A8R8G8B8:
    case MPAF_RGB32_R8G8B8:
    case MPAF_RGB32_B8G8R8:
    case MPAF_RGB32_A8B8G8R8:
    case MPAF_RGB32_R8G8B8A8:
        bitmap->lPitch[0] = bitmap->lWidth * 4;//LINE_BYTES(bitmap->lWidth, 32);
        lBufferH = bitmap->lHeight;
        lSize = MBitmapSize(*bitmap);
        printf("MBitmapSize 32bit lSize = %ld\n" , lSize);
        break;

    case MPAF_RGB24_R8G8B8:
    case MPAF_RGB24_B8G8R8:
        bitmap->lPitch[0] = bitmap->lWidth * 3;//LINE_BYTES(bitmap->lWidth, 24);
        lBufferH = bitmap->lHeight;
        lSize = MBitmapSize(*bitmap);
        break;

    case MPAF_RGB16_B5G6R5:
    case MPAF_RGB16_R5G6B5:
        bitmap->lPitch[0] = bitmap->lWidth * 2;//LINE_BYTES(bitmap->lWidth, 16);
        lBufferH = bitmap->lHeight;
        lSize = MBitmapSize(*bitmap);
        break;

    case MPAF_YUYV:
    case MPAF_YVYU:
    case MPAF_UYVY:
    case MPAF_VYUY:
        bitmap->lPitch[0] = bitmap->lWidth * 2;//LINE_BYTES(bitmap->lWidth, 16);
        lBufferH = bitmap->lHeight;
        lSize = MBitmapSize(*bitmap);
        break;

    case MPAF_I420:
    case MPAF_YV12:
        bitmap->lPitch[0] = bitmap->lWidth;//LINE_BYTES(bitmap->lWidth, 8);
        bitmap->lPitch[1] = bitmap->lPitch[0]>>1;
        bitmap->lPitch[2] = bitmap->lPitch[0]>>1;
        lBufferH = bitmap->lHeight;
        lSubSampleV = 2;
        lSize = MBitmapSize(*bitmap);
        break;

    case MPAF_OTHERS_NV21:
    case MPAF_OTHERS_NV12:
    case MPAF_OTHERS_NV21 | MPAF_BT601_YCBCR:
    	{
    		bitmap->lPitch[0] = bitmap->lWidth;//LINE_BYTES(bitmap->lWidth, 8);
			bitmap->lPitch[1] = bitmap->lPitch[0];
			bitmap->lPitch[2] = bitmap->lPitch[0];
			lBufferH = (bitmap->lHeight + 1) >> 1 <<1;
			lSubSampleV = 2;
			lSize = bitmap->lPitch[0] * lBufferH + bitmap->lPitch[1] * (lBufferH/lSubSampleV);
			printf("MBitmapSize lSize = %ld\n" , lSize);
        }
        break;

    case MPAF_I422V:
        bitmap->lPitch[0] = bitmap->lWidth;//LINE_BYTES(bitmap->lWidth, 8);
        bitmap->lPitch[1] = bitmap->lPitch[0];
        bitmap->lPitch[2] = bitmap->lPitch[0];
        lBufferH = bitmap->lHeight;
        lSubSampleV = 2;
        lSize = MBitmapSize(*bitmap);
        break;

    case MPAF_I422H:
        bitmap->lPitch[0] = bitmap->lWidth;//LINE_BYTES(bitmap->lWidth, 8);
        bitmap->lPitch[1] = bitmap->lPitch[0]>>1;
        bitmap->lPitch[2] = bitmap->lPitch[0]>>1;
        lBufferH = (bitmap->lHeight);
        lSize = MBitmapSize(*bitmap);
        break;

    case MPAF_I444:
        bitmap->lPitch[0] = bitmap->lWidth;//LINE_BYTES(bitmap->lWidth, 8);
        bitmap->lPitch[1] = bitmap->lPitch[0];
        bitmap->lPitch[2] = bitmap->lPitch[0];
        lBufferH = (bitmap->lHeight);
        lSize = MBitmapSize(*bitmap);
        break;

    case MPAF_YUV:
        bitmap->lPitch[0] =  bitmap->lWidth * 3;//LINE_BYTES(bitmap->lWidth, 24);
        lBufferH = (bitmap->lHeight);
        lSize = MBitmapSize(*bitmap);
        break;
    case MPAF_GRAY8:
        bitmap->lPitch[0] =  bitmap->lWidth;//LINE_BYTES(bitmap->lWidth, 24);
        lBufferH = (bitmap->lHeight);
        lSize = MBitmapSize(*bitmap);
        break;

    default:
        return MERR_UNSUPPORTED;
    }

    pData = (MByte*)MMemAlloc(MNull, lSize );
    if( MNull == pData )
        return MERR_NO_MEMORY;

    if(bitmap->pPlane[0])
        MMemFree( MNull, bitmap->pPlane[0]);
    bitmap->pPlane[0] = pData;
    MMemSet(bitmap->pPlane[0], 0, lSize);

    switch (bitmap->dwPixelArrayFormat)
    {
    case MPAF_OTHERS_NV21:
    case MPAF_OTHERS_NV21 | MPAF_BT601_YCBCR:
        if (bitmap->lPitch[2])
            bitmap->pPlane[2] = bitmap->pPlane[0] + bitmap->lPitch[0] * lBufferH;
        if (bitmap->lPitch[1])
            bitmap->pPlane[1] = bitmap->pPlane[2] + 1 ;
        break;

    case MPAF_OTHERS_NV12:
        if (bitmap->lPitch[1])
            bitmap->pPlane[1] = bitmap->pPlane[0] + bitmap->lPitch[0] * lBufferH;
        if (bitmap->lPitch[2])
            bitmap->pPlane[2] = bitmap->pPlane[1] + 1 ;
        break;

    default:
        if (bitmap->lPitch[1])
            bitmap->pPlane[1] = bitmap->pPlane[0] + bitmap->lPitch[0] * lBufferH;
        if (bitmap->lPitch[2])
            bitmap->pPlane[2] = bitmap->pPlane[1] + bitmap->lPitch[1] * (lBufferH/lSubSampleV);
        break;
    }
    return MOK;
}

MRESULT MBitmapFree(MBITMAP bitmap)
{
    if (bitmap.pPlane[0])
    {
        MMemFree(MNull, bitmap.pPlane[0]);
        bitmap.pPlane[0] = MNull;
    }
    return MOK;
}

MLong MBitmapSize(MBITMAP bitmap)
{
    MLong   lSize = 0;
    MLong   lBufferH = 0;
    MLong   lSubSampleV = 1;
    switch(bitmap.dwPixelArrayFormat)
        {
        case MPAF_RGB32_B8G8R8A8:
        case MPAF_RGB32_A8R8G8B8:
        case MPAF_RGB32_R8G8B8:
        case MPAF_RGB32_B8G8R8:
        case MPAF_RGB32_A8B8G8R8:
        case MPAF_RGB32_R8G8B8A8:
            bitmap.lPitch[0] = bitmap.lWidth * 4;//LINE_BYTES(bitmap.lWidth, 32);
            lBufferH = bitmap.lHeight;
            break;

        case MPAF_RGB24_R8G8B8:
        case MPAF_RGB24_B8G8R8:
            bitmap.lPitch[0] = bitmap.lWidth * 3;//LINE_BYTES(bitmap.lWidth, 24);
            lBufferH = bitmap.lHeight;
            break;

        case MPAF_RGB16_B5G6R5:
        case MPAF_RGB16_R5G6B5:
            bitmap.lPitch[0] = bitmap.lWidth * 2;//LINE_BYTES(bitmap.lWidth, 16);
            lBufferH = bitmap.lHeight;
            break;

        case MPAF_YUYV:
        case MPAF_YVYU:
        case MPAF_UYVY:
        case MPAF_VYUY:
            bitmap.lPitch[0] = bitmap.lWidth * 2;//LINE_BYTES(bitmap.lWidth, 16);
            lBufferH = bitmap.lHeight;
            break;

        case MPAF_I420:
        case MPAF_YV12:
            bitmap.lPitch[0] = bitmap.lWidth;//LINE_BYTES(bitmap.lWidth, 8);
            bitmap.lPitch[1] = bitmap.lPitch[0]>>1;
            bitmap.lPitch[2] = bitmap.lPitch[0]>>1;
            lBufferH = bitmap.lHeight;
            lSubSampleV = 2;
            break;

        case MPAF_OTHERS_NV21:
        case MPAF_OTHERS_NV21 | MPAF_BT601_YCBCR:
        case MPAF_OTHERS_NV12:
            bitmap.lPitch[0] = (bitmap.lWidth + 1) >> 1 <<1;//LINE_BYTES(bitmap.lWidth, 8);
            bitmap.lPitch[1] = bitmap.lPitch[0];
            bitmap.lPitch[2] = bitmap.lPitch[0];
            lBufferH = (bitmap.lHeight + 1) >> 1 <<1;// NV21 format width & height must be 2 rat
            lSubSampleV = 4;
            break;

        case MPAF_I422V:
            bitmap.lPitch[0] = bitmap.lWidth;//LINE_BYTES(bitmap.lWidth, 8);
            bitmap.lPitch[1] = bitmap.lPitch[0];
            bitmap.lPitch[2] = bitmap.lPitch[0];
            lBufferH = bitmap.lHeight;
            lSubSampleV = 2;
            break;

        case MPAF_I422H:
            bitmap.lPitch[0] = bitmap.lWidth;//LINE_BYTES(bitmap.lWidth, 8);
            bitmap.lPitch[1] = bitmap.lPitch[0]>>1;
            bitmap.lPitch[2] = bitmap.lPitch[0]>>1;
            lBufferH = (bitmap.lHeight);
            break;

        case MPAF_I444:
            bitmap.lPitch[0] = bitmap.lWidth;//LINE_BYTES(bitmap.lWidth, 8);
            bitmap.lPitch[1] = bitmap.lPitch[0];
            bitmap.lPitch[2] = bitmap.lPitch[0];
            lBufferH = (bitmap.lHeight);
            break;

        case MPAF_YUV:
            bitmap.lPitch[0] = bitmap.lWidth * 3;//LINE_BYTES(bitmap.lWidth, 24);
            lBufferH = (bitmap.lHeight);
            break;
        case MPAF_GRAY8:
            bitmap.lPitch[0] = bitmap.lWidth;
            lBufferH = (bitmap.lHeight);
            break;

        default:
            return 0;
        }

    lSize = (bitmap.lPitch[0] * lBufferH + bitmap.lPitch[1] * lBufferH/lSubSampleV + bitmap.lPitch[2] * lBufferH/lSubSampleV);
    return lSize ;
}

int jpeg_2_nv21(unsigned char* jpeg, int jpegSize, unsigned char** nv21)
{
	MHandle m_hAMCM;//copy from other project for quick accomplish this testbed//warning
    MRESULT res = AMCM_Create(MNull, &m_hAMCM);//destroy //warning
    if (MERR_NONE != res)
    {
        printf("AMCM_Create is wrong!\n");
        return -1;
    }

    AMCM_BEGIN_REGISTER(m_hAMCM);
    
    REGISTER_AMCM_AJLENCODER2(AM_CODEC_PRIORITY_NORMAL); //ArcSoft Advanced JPEG encoder
    REGISTER_AMCM_AJLDECODER2(AM_CODEC_PRIORITY_NORMAL);
    
    AMCM_END_REGISTER();

	MDIMGFILEINFO info = { 0 };
	MInt32 lWidth = 0;//platform sucks for typedef signed		long		MInt32;//warning
	MInt32 lHeight = 0;
	
	HMSTREAM hImgStream = MStreamOpenFromStream(jpeg, jpegSize, STREAM_READ);//close missing//warning
	if(hImgStream)
	{
		MdGetImgFileInfo(m_hAMCM, hImgStream, &info);
		lWidth = info.lWidth;
		lHeight = info.lHeight;
	}

	printf("width :%d, height :%d\n", lWidth, lHeight);

	MBITMAP bitmap = { 0 };
	bitmap.lWidth = lWidth;
	bitmap.lHeight = lHeight;
	//test
	bitmap.dwPixelArrayFormat = MPAF_OTHERS_NV21 | MPAF_BT601_YCBCR;
	res = MBitmapAlloc(&bitmap);//not check return value and free //warning
	printf("MBitmapAlloc res :%ld\n", res);
	res = MdBitmapLoad(m_hAMCM, hImgStream, AM_JPEG, &bitmap);//warning
	printf("MdBitmapLoad2 res :%ld\n", res);
	
#if 1 //test

    FILE* fp = fopen("/sdcard/4000x3000.nv21", "w");

    fwrite(bitmap.pPlane[0], 1, bitmap.lWidth*bitmap.lHeight*3/2, fp);
    fclose(fp);

#endif

    *nv21 = bitmap.pPlane[0];
    
    return 0;
}

int main___(int argc, char** argv)
{
    printf("xxxxx \n");
    int size = 4219125;
    unsigned char* p = (unsigned char*)malloc(size);
    
    printf("5555555 \n");
    FILE* fp = fopen("/sdcard/IMG_19700201152832.jpg", "r");
    
    printf("fp :%p\n", fp);
    fread(p, size, 1, fp);
    printf("fp2 :%p\n", fp);
        
    unsigned char* nv21;
    jpeg_2_nv21(p ,size, &nv21);
    
    free(p);
    free(nv21);
    
    return 0;
}
