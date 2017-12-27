package com.arcsoft.rendernv21;

import android.graphics.Bitmap;

public class rendernv21 {
	
	static public native void native_Init(int previewWidth, int previewHeight);
	static public native void native_Release();
	
	static public native void native_ProcessData(byte[] previewData);
	
	static public native void native_ResetPreviewSize(int previewWidth, int previewHeight);
	static public native void native_onSurfaceCreated();
	static public native void native_onSurfaceChanged(int surfaceWidth, int surfaceHeight);
	static public native void native_onDrawFrame();
	static public native void native_setMirror(boolean bMirror);
	
}
