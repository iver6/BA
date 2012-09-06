package com.iver.bochsandroid;

import java.util.concurrent.locks.*;
import 	android.os.Handler;
import android.os.Message;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Color;
import java.io.*;

public class vmThread extends Thread {
	private String PackageName;
	private Lock BitMapLock;
	private vmView vv;
	private Handler handler;

	public vmThread() {
		// TODO Auto-generated constructor stub
	}

	public vmThread(Runnable runnable) {
		super(runnable);
		// TODO Auto-generated constructor stub
	}

	public vmThread(String threadName,String pPackageName,Lock pBitMapLock,vmView pvv,Handler phandler) {
		super(threadName);
		// TODO Auto-generated constructor stub
		PackageName = pPackageName;
		BitMapLock = pBitMapLock;
		vv = pvv;
		handler = phandler;
	}
	
	public void newBitMap(int [] pOrgColors,int pWidth,int pHeight)
	{
		Bitmap bmp;
		Message msg = handler.obtainMessage();
		
		int [] colors = new int[pHeight * pWidth];
		for(int i = 0; i < pHeight * pWidth; i += pWidth)
		{
			for(int j = 0; j < pWidth; j++)
			{
				colors[i + j] = Color.rgb((pOrgColors[i + j] >> 16 & 0xFF),(pOrgColors[i + j] >> 8 & 0xFF),(pOrgColors[i + j] & 0xFF));
			}
		}
		bmp = Bitmap.createBitmap(colors, pWidth, pHeight, Config.ARGB_8888);
		//Bitmap i;
		vv.newBitmap(bmp);
		//SaveBMPToFile(bmp);
		//bmp.recycle();
		
		msg.what = 1;
		msg.sendToTarget();
		
	}

	public vmThread(Runnable runnable, String threadName) {
		super(runnable, threadName);
		// TODO Auto-generated constructor stub
	}

	public vmThread(ThreadGroup group, Runnable runnable) {
		super(group, runnable);
		// TODO Auto-generated constructor stub
	}

	public vmThread(ThreadGroup group, String threadName) {
		super(group, threadName);
		// TODO Auto-generated constructor stub
	}

	public vmThread(ThreadGroup group, Runnable runnable, String threadName) {
		super(group, runnable, threadName);
		// TODO Auto-generated constructor stub
	}

	public vmThread(ThreadGroup group, Runnable runnable, String threadName,
			long stackSize) {
		super(group, runnable, threadName, stackSize);
		// TODO Auto-generated constructor stub
	}
	
	
	public void run() { 
		runbx(PackageName);
    }  
	
	
	
	public native void runbx(String PackageName);
    
    static {
        System.loadLibrary("vmcore");
    }

    private void SaveBMPToFile(Bitmap bmp){
    		File f = new File("/data/data/" + PackageName + "/abc.png");
    		FileOutputStream fOut = null;
    		try {
		        
		        f.createNewFile();
		        
    		} catch (IOException e) {
                e.printStackTrace();
    		}
	        try {
	                fOut = new FileOutputStream(f);
	        } catch (FileNotFoundException e) {
	                e.printStackTrace();
	        }
	        bmp.compress(Bitmap.CompressFormat.PNG, 100, fOut);
	        try {
	                fOut.flush();
	        } catch (IOException e) {
	                e.printStackTrace();
	        }
	        try {
	                fOut.close();
	        } catch (IOException e) {
	                e.printStackTrace();
	        }
	}
}
