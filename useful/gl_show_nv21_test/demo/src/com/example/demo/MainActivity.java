package com.example.demo;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

import com.arcsoft.rendernv21.rendernv21;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.SurfaceTexture;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.RelativeLayout.LayoutParams;
import android.widget.Toast;

public class MainActivity extends Activity
{
	private OpenGLView mOpenGLView;
	private SurfaceTexture mSurfaceTexture;
    private static int SCREEN_SIZE_WIDTH = 1280;
	private static int SCREEN_SIZE_HEIGHT = 720;
	private static int PREVIEW_SIZE_WIDTH = 1920;
	private static int PREVIEW_SIZE_HEIGHT = 1080;
	private final static byte[] mSync = new byte[0];
    
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		RelativeLayout mMainLayout = new RelativeLayout(this);
		LayoutParams param = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
	    mMainLayout.setLayoutParams(param);
	    setContentView(mMainLayout);
	    
	    
	    mOpenGLView = new OpenGLView(this);
		mMainLayout.addView(mOpenGLView, param);
		
		Button btn = new Button(this);
		param = new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
		param.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, 1);
		btn.setText("show");
    	mMainLayout.addView(btn, param);
    	
		int width = 1680;
		int height = 1050;
		rendernv21.native_Init(width, height);

    	btn.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				byte[] previewData = File2byte("/sdcard/1680x1050_out.nv21");
				rendernv21.native_ProcessData(previewData);

				mOpenGLView.requestRender();
				
				//rendernv21.native_Release();
			}
		});
		setScreenSize();
	}
	
	public static byte[] File2byte(String filePath)  
    {  
        byte[] buffer = null;  
        try  
        {  
            File file = new File(filePath);  
            FileInputStream fis = new FileInputStream(file);  
            ByteArrayOutputStream bos = new ByteArrayOutputStream();  
            byte[] b = new byte[1024];  
            int n;  
            while ((n = fis.read(b)) != -1)  
            {  
                bos.write(b, 0, n);  
            }  
            fis.close();  
            bos.close();  
            buffer = bos.toByteArray();  
        }  
        catch (FileNotFoundException e)  
        {  
            e.printStackTrace();  
        }  
        catch (IOException e)  
        {  
            e.printStackTrace();  
        }  
        return buffer;  
    }
	@Override
	protected void onResume() 
	{
		super.onResume();
		if (null == mSurfaceTexture)
		{
			mSurfaceTexture = new SurfaceTexture(0);
		}
		
		
		if (null != mOpenGLView) 
		{
			mOpenGLView.onResume();
		}
		
		setSurfaceViewSize(PREVIEW_SIZE_WIDTH, PREVIEW_SIZE_HEIGHT);
	}
	
	@Override
	protected void onPause()
	{
		super.onPause();
		releaseSurfaceTexture();
		
		if (null != mOpenGLView) {
			mOpenGLView.onPause();
		}
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();
	}
	
	@SuppressLint("NewApi")
	private void releaseSurfaceTexture()
	{
		if (null != mSurfaceTexture)
		{
			mSurfaceTexture.release();
			mSurfaceTexture = null;
		}
	}
	
	private void setScreenSize()
	{
		DisplayMetrics metrics = new DisplayMetrics();
    	metrics = getResources().getDisplayMetrics();
    	SCREEN_SIZE_WIDTH = metrics.widthPixels;
    	SCREEN_SIZE_HEIGHT = metrics.heightPixels;
	}
	
	private void setSurfaceViewSize(int previewWidth, int previewHeight)
	{
		if (null == mOpenGLView)
			return;
		
		LayoutParams param = (LayoutParams) mOpenGLView.getLayoutParams();
		float scaleW = (float)previewWidth / SCREEN_SIZE_WIDTH;
		float scaleH = (float)previewHeight / SCREEN_SIZE_HEIGHT;
		if (scaleW > scaleH) {
			param.width = SCREEN_SIZE_WIDTH;
			param.height = (int)(previewHeight / scaleW);
		} else {
			param.width = (int)(previewWidth / scaleH);
			param.height = SCREEN_SIZE_HEIGHT;
		}
		param.addRule(RelativeLayout.CENTER_IN_PARENT, 1);
		mOpenGLView.setLayoutParams(param);
	}
	
	static {
	     System.loadLibrary("RenderNV21");
	}
	
}
