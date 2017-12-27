// filename: OpenFileDemo.java
package com.lma.openfiledemo;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import android.os.Bundle;
import android.app.Activity;
import android.app.Dialog;
import android.util.DisplayMetrics;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.RelativeLayout.LayoutParams;

public class OpenFileDemo extends Activity {
	
	private OpenGLView mOpenGLView;
    private static int SCREEN_SIZE_WIDTH = 1280;
	private static int SCREEN_SIZE_HEIGHT = 720;
	private static int PREVIEW_SIZE_WIDTH = 1920;
	private static int PREVIEW_SIZE_HEIGHT = 1080;
	
	static private int openfileDialogId = 0;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //setContentView(R.layout.activity_open_file_demo);
        
	    setContentView(R.layout.activity_open_file_demo);
	    mOpenGLView = (OpenGLView)findViewById(R.id.opengl);
	    
	    
	    Button mPlay = (Button)findViewById(R.id.btn_play);
	    Button mSelect = (Button)findViewById(R.id.btn_openfile);
	    
//	    mOpenGLView = find;
//		mMainLayout.addView(mOpenGLView, param);
//		
//		Button btn = new Button(this);
//		Button btn2 = new Button(this);
//		param = new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
//		param.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM, 1);
//		btn.setText("select nv21 file");
//    	mMainLayout.addView(btn, param);
//    	
//    	
//		param = new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
//		param.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM|RelativeLayout.ALIGN_PARENT_RIGHT, 1);
//		
//		btn2.setText("play nv21 file");
//    	mMainLayout.addView(btn2, param);
    	
		int width = 640;
		int height = 480;
		rendernv21.native_Init(width, height);
		
		mSelect.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				showDialog(openfileDialogId);
			}
		});
    	
    	
		mPlay.setOnClickListener(new OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				rendernv21.native_ProcessDataByName(OpenFileDemo.this, str);
			}
		});
    	
    }
    
    private String str;
    public void render4c()
    {
    	mOpenGLView.requestRender();	
    }
    
	@Override
	protected Dialog onCreateDialog(int id) {
		if(id==openfileDialogId){
			Map<String, Integer> images = new HashMap<String, Integer>();
			// 下面几句设置各文件类型的图标， 需要你先把图标添加到资源文件夹
			images.put(OpenFileDialog.sRoot, R.drawable.filedialog_root);	// 根目录图标
			images.put(OpenFileDialog.sParent, R.drawable.filedialog_folder_up);	//返回上一层的图标
			images.put(OpenFileDialog.sFolder, R.drawable.filedialog_folder);	//文件夹图标
			images.put("nv21", R.drawable.filedialog_nv21file);	//nv21文件图标
			images.put(OpenFileDialog.sEmpty, R.drawable.filedialog_root);
			Dialog dialog = OpenFileDialog.createDialog(id, this, "open file", new CallbackBundle() {
				@Override
				public void callback(Bundle bundle) {
					String filepath = bundle.getString("path");
					setTitle(filepath); // 把文件路径显示在标题上
					
					//int width = rendernv21.native_getWidth(filepath);
					//int height = rendernv21.native_getHeight(filepath);
					
					//rendernv21.native_ResetPreviewSize(width, height);
					
					str = filepath;
					
					
					
				}
			}, 
			".nv21;",
			images);
			return dialog;
		}
		return null;
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
		
		if (null != mOpenGLView) {
			mOpenGLView.onPause();
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
