#include <jni.h>
#include <string>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

#include "render.h"

Render r;

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

JNIEXPORT int JNICALL native_getHeight(JNIEnv *env, jobject jobj, jstring url)
{
    const char* path;
    jboolean type = false;
    path = env->GetStringUTFChars(url, &type);
    if(path == NULL)
    {
        return 0; /* OutOfMemoryError already thrown */
    }

    string str(path);
    
    std::size_t found = str.find_last_of("x");
    if(found == std::string::npos)
    {
        return 0;
    }
    
    size_t endH = str.find("_", found);
    if(endH == std::string::npos)
        endH = str.find(".", found);
    
    const char* heistr = str.substr(found+1, endH-found-1).c_str();
        
        
    int height = atoi(heistr);
    
    
    env->ReleaseStringUTFChars(url, path);
    
    LOGE("malei test height is %d, heistr is %s", height, heistr);
    
    return height;
}

JNIEXPORT int JNICALL native_getWidth(JNIEnv *env, jobject jobj, jstring url)
{
    const char* path;
    jboolean type = false;
    path = env->GetStringUTFChars(url, &type);
    if(path == NULL)
    {
        return 0; /* OutOfMemoryError already thrown */
    }

    string str(string(path).substr(string(path).find_last_of("/") + 1));
    
    LOGE("malei test str %s", str.c_str());
    
    std::size_t found = str.find_last_of("x");
    if(found == std::string::npos)
    {
        return 0;
    }
    
    size_t startW = str.find_last_of("_", found) + 1;
    if(startW == std::string::npos)
        startW = 0;
    
    const char* widstr = str.substr(startW, found-startW).c_str();
    int width = atoi(widstr);
    
    LOGE("malei test width is %d, widstr is %s", width, widstr);
    
    env->ReleaseStringUTFChars(url, path);
    
    
    return width;
}

char g_buffer[4096*4096*3/2*3];
JNIEXPORT int JNICALL native_ProcessDataByName(JNIEnv *env, jobject jobj, jobject jobj2, jstring url)
{
    int width = native_getWidth(env, jobj, url);
    int height = native_getHeight(env, jobj, url);
    r.setSize(width, height);
    
    
    
    const char* path;
    jboolean type = false;
    path = env->GetStringUTFChars(url, &type);
    
    //struct stat buf;
    //stat(path, &buf);
    
    int framesize = width*height*3/2;
    if(framesize > sizeof(g_buffer)) return -1;
    
    //if(buf.st_size > sizeof(g_buffer)) return -1;
    
    
    
    FILE* fp = fopen(path, "r");
    //fread(g_buffer, buf.st_size, 1, fp);
    
    jclass mainclss = env->FindClass("com/lma/openfiledemo/OpenFileDemo");
    LOGE("malei test mainclss %p", mainclss);
    
    jmethodID reqrenderid = env->GetMethodID(mainclss, "render4c", "()V");
    LOGE("malei test reqrenderid %p", reqrenderid);
    
    

    
    
    
    
    
    
    
    while(fread(g_buffer, framesize, 1, fp) == 1)
    {
        LOGE("wwwwwwwwwwwwwwwwwwwwwwwwwwwwww");
         
         
        r.updateTextureFrame((unsigned char*)g_buffer);
        
        env->CallVoidMethod(jobj2, reqrenderid);
        
        
        
        usleep(200*1000);
    }
    
    fclose(fp);
    env->ReleaseStringUTFChars(url, path);
    
    return 0;
        
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
	{"native_getWidth",				    "(Ljava/lang/String;)I",		                    (void*)native_getWidth},
	{"native_getHeight",				"(Ljava/lang/String;)I",		                    (void*)native_getHeight},
	{"native_ProcessDataByName",		"(Lcom/lma/openfiledemo/OpenFileDemo;Ljava/lang/String;)I",		    (void*)native_ProcessDataByName},
};

jint Register_Demo_Function( JNIEnv * env )
{
	jclass clazz = env->FindClass("com/lma/openfiledemo/rendernv21");
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