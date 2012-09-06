package com.iver.bochsandroid;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import android.os.Bundle;
import android.os.Message;
import android.app.Activity;
import android.view.Menu;
import android.view.MenuItem;
import java.util.concurrent.locks.*;
import 	android.os.Handler;

public class MainActivity extends Activity {

	private vmView vv;
	private vmThread vt;
	
	Lock BitMapLock;
	
	
	private Handler mhandler = new Handler()	{
		public void handleMessage(Message msg)
		{
			if(msg.what == 1)
			{
				vv.invalidate();
			}
		}
	};
	

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //setContentView(R.layout.activity_main);
        BitMapLock = new ReentrantLock();
        vv = new vmView(this,BitMapLock);
        vt = new vmThread("vmThread",getPackageName(),BitMapLock,vv,mhandler);
                
        setContentView(vv);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_main, menu);
        return true;
    }
    
    

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		// TODO Auto-generated method stub
		
		if(item.getItemId() == R.id.menu_start)
		{
			CopyFile();
			if(vt.getState() == Thread.State.NEW) 
			{
				vt.start();
				
			}
			
		}
		
		
		
		return super.onOptionsItemSelected(item);
	}
    
    private void CopyFile()
    {
    	byte buf[] = new byte[1024];
		String sPathOfrcfile;
		int readcount;
		FileOutputStream fos = null;
		InputStream fis = null;
		try{
        
        	fis = getAssets().open("rcconfig");
        	
        	sPathOfrcfile = "/data/data/" + getPackageName();
        	
        	fos = new FileOutputStream ( sPathOfrcfile + "/rcconfig");
        	
        	while( (readcount = fis.read(buf)) > 0)
        	{
        		fos.write(buf, 0, readcount);
        		
        		//System.out.print(new String(buf,0,readcount));
        	}
        	fos.close();
        	fis.close();
        	
        	
        	fis = getAssets().open("BIOS-bochs-latest");
        	
        	sPathOfrcfile = "/data/data/" + getPackageName();
        	
        	fos = new FileOutputStream ( sPathOfrcfile + "/BIOS-bochs-latest");
        	
        	while( (readcount = fis.read(buf)) > 0)
        	{
        		fos.write(buf, 0, readcount);
        		
        		//System.out.print(new String(buf,0,readcount));
        	}
        	fos.close();
        	fis.close();
        	
        	fis = getAssets().open("VGABIOS-lgpl-latest");
        	
        	sPathOfrcfile = "/data/data/" + getPackageName();
        	
        	fos = new FileOutputStream ( sPathOfrcfile + "/VGABIOS-lgpl-latest");
        	
        	while( (readcount = fis.read(buf)) > 0)
        	{
        		fos.write(buf, 0, readcount);
        		
        		//System.out.print(new String(buf,0,readcount));
        	}
        	fos.close();
        	fis.close();
        }
        catch(IOException e)
        {
        	e.printStackTrace();
        }
		
		
		
	
    }
    
    
    
    
}
