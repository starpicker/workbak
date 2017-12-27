package com.lma.openfiledemo;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.lma.openfiledemo.rendernv21;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.util.AttributeSet;

public class OpenGLView extends GLSurfaceView implements Renderer {
	private Context mContext;
	private final static byte[] mSync = new byte[0];
	
	public OpenGLView(Context context) {
		super(context);
		init(context);
	}

	public OpenGLView(Context context, AttributeSet attrs) {
		super(context, attrs);
		init(context);
	}
	
	public void init(Context context) {
		mContext = context;
		setEGLContextClientVersion(2);
		setRenderer(this);
		setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
	}
	
	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		rendernv21.native_onSurfaceCreated();
	}
	
	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		rendernv21.native_onSurfaceChanged(width, height);
	}
	
	@Override
	public void onDrawFrame(GL10 gl) {
		synchronized (mSync) {
			rendernv21.native_onDrawFrame();
		}
	}
}
