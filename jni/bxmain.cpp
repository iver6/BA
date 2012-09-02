#include <jni.h>
#include <android/log.h>
#include <stddef.h>
#include <stdio.h>


#include "bochs.h"
//#include "siminterface.h"

void bxmain();

extern bx_startup_flags_t bx_startup_flags;

bool GetrcFilePath(JNIEnv * env,jstring packagename,char * rcFilePath);


extern "C" JNIEXPORT void JNICALL Java_com_iver_bochsandroid_MainActivity_runbx(JNIEnv * env, jobject obj,
		jstring packagename)
{

	char rcFilePath[PATH_MAX];
	if(!GetrcFilePath(env,packagename,rcFilePath))
	{
		return;
	}

	char *argv[] = {"bochs","-q","-r",rcFilePath};
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

	sprintf(rcFilePath,"/data/data/%s",str);

	env->ReleaseStringUTFChars(packagename,str);

	return true;
}
