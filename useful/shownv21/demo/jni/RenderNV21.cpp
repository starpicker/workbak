#include <jni.h>

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