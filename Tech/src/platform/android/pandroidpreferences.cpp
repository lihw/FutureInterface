// pandroidpreference.h
// The preference interfaces of FI Android application
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "pandroidpreferences.h"

#include <PFoundation/pjnihelper.h>


#if (P_ENABLE_LOGGING)
#define  LOG_TAG    "PAndroidPreferences"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#else
#define  LOGD(...) 
#endif

jobject PAndroidPreferences::preferences()
{
    JNIEnv *env;
    PJniHelper::getEnv(&env);
    jclass classID = PJniHelper::getClassID("android/preference/PreferenceManager");
    jmethodID methodID = env->GetStaticMethodID(classID, 
        "getDefaultSharedPreferences", 
        "(Landroid/content/Context;)Landroid/content/SharedPreferences;");
    if (!methodID)
    {
        LOGD("Failed to find method id of getDefaultSharedPreferences");
        return P_NULL;
    }
   
    jobject sharedPreferences = env->CallStaticObjectMethod(classID, methodID, g_pContext);
    return sharedPreferences;
}

pint32 PAndroidPreferences::value(const pchar *name, const pint32 defaultValue)
{
    JNIEnv *env = 0;
    pint32 ret = defaultValue;
    do
    {
        if (!PJniHelper::getEnv(&env))
        {
            break;
        }
        jobject preferences = PAndroidPreferences::preferences();
        jclass classID = PJniHelper::getClassID(preferences, env);
        jmethodID methodID = PJniHelper::getMethodID(classID, "getInt", "(Ljava/lang/String;I)I", env);
        if (!methodID)
        {
            env->DeleteLocalRef(classID);
            env->DeleteLocalRef(preferences);
            break;
        }
        jstring jstrMSG = env->NewStringUTF(name);
        ret = (jint)(env->CallIntMethod(preferences, methodID, jstrMSG, defaultValue));
        env->DeleteLocalRef(classID);
        env->DeleteLocalRef(preferences);
        env->DeleteLocalRef(jstrMSG);
    } 
    while (0);
    return ret;
}

PString PAndroidPreferences::value(const pchar *name, const pchar *defaultValue)
{
    JNIEnv *env = 0;
    PString ret = PString(defaultValue);
    do
    {
        if (!PJniHelper::getEnv(&env))
        {
            break;
        }
        jobject preferences = PAndroidPreferences::preferences();
        jclass classID = PJniHelper::getClassID(preferences, env);
        jmethodID methodID = PJniHelper::getMethodID(classID, "getString", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;", env);
        if (!methodID)
        {
            env->DeleteLocalRef(classID);
            env->DeleteLocalRef(preferences);
            break;
        }
        jstring jstrMsg = env->NewStringUTF(name);
        jstring jstrDefault = env->NewStringUTF(defaultValue);
        jstring jstrRet = (jstring)(env->CallIntMethod(preferences, methodID, jstrMsg, jstrDefault));
        ret = PJniHelper::jstring2string(jstrRet);
        env->DeleteLocalRef(classID);
        env->DeleteLocalRef(preferences);
        env->DeleteLocalRef(jstrMsg);
        env->DeleteLocalRef(jstrDefault);
        env->DeleteLocalRef(jstrRet);
    }
    while (0);
    return ret;
}

pbool PAndroidPreferences::value(const pchar *name, const pbool defaultValue)
{
    JNIEnv *env = 0;
    pbool ret = defaultValue;
    do
    {
        if (!PJniHelper::getEnv(&env))
        {
            break;
        }
        jobject preferences = PAndroidPreferences::preferences();
        jclass classID = PJniHelper::getClassID(preferences, env);
        jmethodID methodID = PJniHelper::getMethodID(classID, "getBoolean", "(Ljava/lang/String;Z)Z", env);
        if (!methodID)
        {
            env->DeleteLocalRef(classID);
            env->DeleteLocalRef(preferences);
            break;
        }
        jstring jstrMSG = env->NewStringUTF(name);
        ret = (jboolean)(env->CallIntMethod(preferences, methodID, jstrMSG, defaultValue));
        env->DeleteLocalRef(classID);
        env->DeleteLocalRef(preferences);
        env->DeleteLocalRef(jstrMSG);
    }
    while (0);
    return ret;
}

pfloat32 PAndroidPreferences::value(const pchar *name, const pfloat32 defaultValue)
{
    JNIEnv *env = 0;
    pfloat32 ret = defaultValue;
    do
    {
        if (!PJniHelper::getEnv(&env))
        {
            break;
        }
        jobject preferences = PAndroidPreferences::preferences();
        jclass classID = PJniHelper::getClassID(preferences, env);
        jmethodID methodID = PJniHelper::getMethodID(classID, "getFloat", "(Ljava/lang/String;F)F", env);
        if (!methodID)
        {
            env->DeleteLocalRef(classID);
            env->DeleteLocalRef(preferences);
            break;
        }
        jstring jstrMSG = env->NewStringUTF(name);
        ret = (jfloat)(env->CallIntMethod(preferences, methodID, jstrMSG, defaultValue));
        env->DeleteLocalRef(classID);
        env->DeleteLocalRef(preferences);
        env->DeleteLocalRef(jstrMSG);
    }
    while (0);
    return ret;
}

long PAndroidPreferences::value(const pchar *name, const long defaultValue)
{
    JNIEnv *env = 0;
    pbool ret = defaultValue;
    do
    {
        if (!PJniHelper::getEnv(&env))
        {
            break;
        }
        jobject preferences = PAndroidPreferences::preferences();
        jclass classID = PJniHelper::getClassID(preferences, env);
        jmethodID methodID = PJniHelper::getMethodID(classID, "getLong", "(Ljava/lang/String;J)J", env);
        if (!methodID)
        {
            env->DeleteLocalRef(classID);
            env->DeleteLocalRef(preferences);
            break;
        }
        jstring jstrMSG = env->NewStringUTF(name);
        ret = (jlong)(env->CallIntMethod(preferences, methodID, jstrMSG, defaultValue));
        env->DeleteLocalRef(classID);
        env->DeleteLocalRef(preferences);
        env->DeleteLocalRef(jstrMSG);
    }
    while (0);
    return ret;
}

