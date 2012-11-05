#include <jni.h>
#include <stdio.h>
#include "jni_JNITest.h"

JNIEXPORT void JNICALL Java_jnitest_callnative(JNIEnv *env,
                                                  jobject obj)
   {
   printf("HelloWorld\n");
   return;
   }