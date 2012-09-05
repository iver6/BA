#include <jni.h>
#include <android/log.h>
#include <stddef.h>
#include <stdio.h>


#include "bochs.h"
//#include "siminterface.h"

void bxmain();

extern bx_startup_flags_t bx_startup_flags;

bool GetrcFilePath(JNIEnv * env,jstring packagename,char * rcFilePath);

jobject gbitmaplock;

extern "C" JNIEXPORT void JNICALL Java_com_iver_bochsandroid_vmThread_runbx(JNIEnv * env, jobject obj,
		jstring packagename,jobject bitmaplock)
{

	gbitmaplock = bitmaplock;

	char rcFilePath[PATH_MAX];
	if(!GetrcFilePath(env,packagename,rcFilePath))
	{
		return;
	}

	gbitmaplock = bitmaplock;

	char *argv[] = {"bochs","-q","-f",rcFilePath};
	bx_startup_flags.argc = 4;
	bx_startup_flags.argv = argv;
	bxmain();
	return;
}



bool GetrcFilePath(JNIEnv * env,jstring packagename,char * rcFilePath)
{
	const char *str;

	str = env->GetStringUTFChars(packagename,NULL);
	if(str == NULL)
	{
		return false;
	}

	sprintf(rcFilePath,"/data/data/%s/rcconfig",str);

	env->ReleaseStringUTFChars(packagename,str);

	return true;
}
