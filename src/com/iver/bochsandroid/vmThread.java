package com.iver.bochsandroid;

import java.util.concurrent.locks.*;

public class vmThread extends Thread {
	private String PackageName;
	private Lock BitMapLock;

	public vmThread() {
		// TODO Auto-generated constructor stub
	}

	public vmThread(Runnable runnable) {
		super(runnable);
		// TODO Auto-generated constructor stub
	}

	public vmThread(String threadName,String pPackageName,Lock pBitMapLock) {
		super(threadName);
		// TODO Auto-generated constructor stub
		PackageName = pPackageName;
		BitMapLock = pBitMapLock;
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
		runbx(PackageName,BitMapLock);
    }  
	
	
	
	public native void runbx(String PackageName,Lock pBitMapLock);
    
    static {
        System.loadLibrary("vmcore");
    }

}
