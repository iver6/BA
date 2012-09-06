package com.iver.bochsandroid;

import java.util.concurrent.locks.Lock;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.Bitmap;
import android.graphics.Color;

public class vmView extends View {
	private Lock BitMapLock;
	private Bitmap bmp;
	

	public vmView(Context context,Lock pBitMapLock) {
		super(context);
		// TODO Auto-generated constructor stub
		BitMapLock = pBitMapLock;
		
		
		BitMapLock.lock();
		
		bmp = Bitmap.createBitmap( 80 * 8, 25 * 16, Config.ARGB_8888);
		bmp.eraseColor(Color.WHITE);
		
		BitMapLock.unlock();
		
		this.invalidate();
	}

	public vmView(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
	}

	public vmView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		// TODO Auto-generated constructor stub
	}
	
	public void onDraw(Canvas canvas){  
		super.onDraw(canvas);
		//canvas.drawBitmap(bitmap, matrix, paint)
		BitMapLock.lock();
		canvas.drawBitmap(bmp, 0, 0, null);
		BitMapLock.unlock();
	}
	
	public void newBitmap(Bitmap pbmp)
	{
		BitMapLock.lock();
		
		if(bmp.isRecycled() == false)
		{
			bmp.recycle();
		}
		
		bmp = Bitmap.createBitmap(pbmp);
		
		BitMapLock.unlock();
		
		//this.invalidate();
	}

}
