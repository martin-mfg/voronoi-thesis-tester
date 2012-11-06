#include <jni.h>
#include <stdio.h>
#include "jni_JNITest.h"

JNIEXPORT void JNICALL Java_jni_JNITest_callnative(JNIEnv *env,
                                                  jobject obj)
   {
   printf("HelloWorld\n");
   return;
   }