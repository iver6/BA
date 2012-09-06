#include <jni.h>
#include <android/log.h>
#include <stddef.h>
#include <stdio.h>


#include "bochs.h"
//#include "siminterface.h"

void bxmain();

extern bx_startup_flags_t bx_startup_flags;

bool GetrcFilePath(JNIEnv * env,jstring packagename,char * rcFilePath);
JNIEnv * g_env;
jobject g_vmThreadobj;

extern "C" JNIEXPORT void JNICALL Java_com_iver_bochsandroid_vmThread_runbx(JNIEnv * env, jobject obj,
		jstring packagename)
{
	g_env = env;
	g_vmThreadobj =  obj;

	char rcFilePath[PATH_MAX];
	if(!GetrcFilePath(env,packagename,rcFilePath))
	{
		return;
	}

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



void CallJNI_vmThread_newBitMap(int * ScreenBitMap,int pWidth,int pHeight)
{

	jclass vmThread_cls;
	jmethodID newBitMap_mid;
	jintArray newIntArray ;
	jint * buff;

	buff = new jint[pWidth * pHeight];


	newIntArray = g_env->NewIntArray(pWidth * pHeight);

	if(newIntArray == NULL)
	{
		delete [] buff;
		__android_log_print(ANDROID_LOG_INFO,"CallJNI_vmThread_newBitMap","create newIntArray error.");
		return;
	}


	for(int i = 0; i < pHeight * pWidth; i += pWidth)
	{
		for(int j = 0; j < pWidth; j++)
		{
			buff[i + j] = (jint)ScreenBitMap[i + j];
		}
	}


	g_env->SetIntArrayRegion(newIntArray, 0, pHeight * pWidth, buff);
	//g_env->SetObjectArrayElement(result, i, newIntArray);

	delete [] buff;


	vmThread_cls= g_env->FindClass("com/iver/bochsandroid/vmThread");

	if(vmThread_cls == NULL)
	{
		//delete [] buff;
		__android_log_print(ANDROID_LOG_INFO,"CallJNI_vmThread_newBitMap","com/iver/bochsandroid/vmThread class not found.");
		return;
	}

	newBitMap_mid = g_env->GetMethodID(vmThread_cls,"newBitMap","([III)V");

	if(newBitMap_mid == NULL)
	{
		//delete [] buff;
		__android_log_print(ANDROID_LOG_INFO,"CallJNI_vmThread_newBitMap","class vmThread member func newBitMap() not found.");
		return;
	}

	g_env->CallVoidMethod(g_vmThreadobj,newBitMap_mid,newIntArray,(jint)pWidth,(jint)pHeight);

	g_env->DeleteLocalRef(newIntArray);
	return;
}
