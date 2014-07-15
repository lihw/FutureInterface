// main.cpp
// The main entry of FI Android application
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
// 

#include <PPlatform/pandroid_module.h>
#include <PFoundation/pruntime_module.h>
#include <PFoundation/pjnihelper.h>

#include <android/log.h>
#include <jni.h>

P_EXTERN void pMain(int argc, char* argv[]);

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    __android_log_write(ANDROID_LOG_INFO, "JNI_OnLoad", "On entry of shared library");

    // Set global java vm.
    PJniHelper::setJavaVM(vm);

    PNEW(PActivity(0, P_NULL));
    
    // Calling this is needed so the pandroidmain.cpp is included in the final .so as well.. 
    pAndroidDoNothing();

    return JNI_VERSION_1_4;
}

void JNI_OnUnload(JavaVM* vm, void* reserved)
{
    __android_log_write(ANDROID_LOG_INFO, "JNI_OnUnload", "uninit STARTEDED");
    
    PDELETE(PActivity::s_activity);

    __android_log_write(ANDROID_LOG_INFO, "JNI_OnUnload", "uninit DONE");
}

