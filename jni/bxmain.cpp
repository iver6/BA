#include <jni.h>
#include <android/log.h>
#include <stddef.h>

void bxmain();

extern "C" JNIEXPORT void JNICALL Java_com_iver_testforbochsa_MainActivity_runbx(JNIEnv * env, jobject obj)
{
	bxmain();
	return;
}
