// pandroidenvironment.cpp
// Android environment
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "../penvironment.h"

#include <PFoundation/plog.h>
#include <PFoundation/pcrt.h>
#include <PFoundation/pjnihelper.h>

#include <stdio.h>

jobject         g_pContext = 0;
AAssetManager  *g_pAssetManager = 0;
const pchar    *g_pSDCardPath = P_NULL;
const pchar    *g_pSystemPath = P_NULL;

void P_APIENTRY pEnvironmentSetJavaVM(JavaVM *vm)
{
    PJniHelper::setJavaVM(vm);
}

void P_APIENTRY pEnvironmentSetContext(jobject context)
{
    g_pContext = context;

    JNIEnv *env;
    PJniHelper::getEnv(&env);

    // get asset manager.
    jclass cls = PJniHelper::getClassID(context);
    jmethodID mid = env->GetMethodID(cls, "getAssets", "()Landroid/content/res/AssetManager;");
    jobject assetManager = env->CallObjectMethod(context, mid);

    // get ndk asset manager.
    g_pAssetManager = AAssetManager_fromJava(env, assetManager);
    env->DeleteLocalRef(assetManager);

    // Get system directory path.
    PDELETEARRAY(g_pSystemPath);
    mid = env->GetMethodID(cls, "getPackageName", "()Ljava/lang/String;");
    jstring jstr = (jstring)(env->CallObjectMethod(context, mid));
    PString packageName = PJniHelper::jstring2string(jstr);
    pchar buffer[1024];
    psprintf(buffer, 1024, "/data/data/%s/files", packageName.c_str());
    g_pSystemPath = pstrdup(buffer);
    env->DeleteLocalRef(jstr);
    env->DeleteLocalRef(cls);

    // Get sdcard path.
    PDELETEARRAY(g_pSDCardPath);
    cls = env->FindClass("android/os/Environment");
    mid = env->GetStaticMethodID(cls, "getExternalStorageState", "()Ljava/lang/String;");
    jstr = (jstring)(env->CallStaticObjectMethod(cls, mid));
    PString storageState = PJniHelper::jstring2string(jstr);
    env->DeleteLocalRef(jstr);

    jfieldID fid = env->GetStaticFieldID(cls, "MEDIA_MOUNTED", "Ljava/lang/String;");
    jstr = (jstring)(env->GetStaticObjectField(cls, fid));
    PString MEDIA_MOUNTED = PJniHelper::jstring2string(jstr);
    env->DeleteLocalRef(jstr);
    if (storageState == MEDIA_MOUNTED)
    {
        mid = env->GetStaticMethodID(cls, "getExternalStorageDirectory", "()Ljava/io/File;");
        jobject file = env->CallStaticObjectMethod(cls, mid);
        env->DeleteLocalRef(cls);
        cls = env->GetObjectClass(file);
        mid = env->GetMethodID(cls, "getAbsolutePath", "()Ljava/lang/String;");
        jstr = (jstring)(env->CallObjectMethod(file, mid));        
        env->DeleteLocalRef(file);
        g_pSDCardPath = PJniHelper::jstring2chars(jstr);
        env->DeleteLocalRef(jstr);
    }
    else
    {
        g_pSDCardPath = P_NULL;
    }
    env->DeleteLocalRef(cls);
}

void P_APIENTRY pEnvironmentClear()
{
    g_pContext = 0;
    g_pAssetManager = 0;
    PDELETEARRAY(g_pSDCardPath);
    PDELETEARRAY(g_pSystemPath);

    // Tell java library is uninitialized.
    JNIEnv *env;
    PJniHelper::getEnv(&env);
    
    jclass cls = PJniHelper::getClassID("com/fi/tech/PNativeLibrary", env);
    jmethodID mid = PJniHelper::getStaticMethodID(cls, "getInstance", "()Lcom/fi/tech/PNativeLibrary;", env);
    jobject instance = env->CallStaticObjectMethod(cls, mid);
    
    mid = PJniHelper::getMethodID(cls, "uninitialize", "()V", env);
    env->CallVoidMethod(instance, mid);

    env->DeleteLocalRef(cls);
    env->DeleteLocalRef(instance);
}

