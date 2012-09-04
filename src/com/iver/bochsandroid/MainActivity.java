package com.iver.bochsandroid;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import android.os.Bundle;
import android.app.Activity;
import android.content.Context;
import android.util.AttributeSet;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

public class MainActivity extends Activity {

	vmView vv;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //setContentView(R.layout.activity_main);
        
        vv = new vmView(this);
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
			runbx(getPackageName());
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
    
    public native void runbx(String PackageName);
    
    static {
        System.loadLibrary("vmcore");
    }
    
    
}
