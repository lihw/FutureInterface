// pjnihelper.cpp
// The JNI helper of Tech Android application
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "pjnihelper.h"
#include <android/log.h>


#if (P_ENABLE_LOGGING)
#define  LOG_TAG    "PJniHelper"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#else
#define  LOGD(...) 
#endif

JavaVM* PJniHelper::s_pJavaVM = P_NULL;

JavaVM* PJniHelper::getJavaVM()
{
    return s_pJavaVM;
}

void PJniHelper::setJavaVM(JavaVM *javaVM)
{
    s_pJavaVM = javaVM;
}

pbool PJniHelper::getEnv(JNIEnv **env)
{
    pbool ret = false;
    
    do
    {
        if (PJniHelper::getJavaVM()->GetEnv((void**)env, JNI_VERSION_1_4) != JNI_OK)
        {
            LOGD("Failed to get the environment using GetEnv()");
            break;
        }
        
        if (PJniHelper::getJavaVM()->AttachCurrentThread(env, 0) < 0)
        {
            LOGD("Failed to get the environment using AttachCurrentThread()");
            break;
        }
        
        ret = true;
    } while (0);
    
    return ret;
}

jclass PJniHelper::getClassID(const char *className, JNIEnv *env)
{
    JNIEnv *pEnv = env;
    jclass ret = 0;

    do 
    {
        if (!pEnv)
        {
            if (!PJniHelper::getEnv(&pEnv))
            {
                break;
            }
        }
        
        ret = pEnv->FindClass(className);
        if (!ret)
        {
            LOGD("Failed to find class of %s", className);
            break;
        }
    } while (0);

    return ret;
}

jclass PJniHelper::getClassID(jobject obj, JNIEnv *env)
{
    JNIEnv *pEnv = env;
    jclass ret = 0;

    do 
    {
        if (!pEnv)
        {
            if (!PJniHelper::getEnv(&pEnv))
            {
                break;
            }
        }
        
        ret = pEnv->GetObjectClass(obj);
        if (!ret)
        {
            LOGD("Failed to get class of 0x%x", (unsigned int)obj);
            break;
        }
    } while (0);

    return ret;
}

jmethodID PJniHelper::getMethodID(jclass classID, const pchar *name, const pchar *sig, JNIEnv *env)
{
    JNIEnv *pEnv = env;
    jmethodID ret = 0;

    do 
    {
        if (!pEnv)
        {
            if (!PJniHelper::getEnv(&pEnv))
            {
                break;
            }
        }
        
        ret = pEnv->GetMethodID(classID, name, sig);
        if (!ret)
        {
            LOGD("Failed to get method %s(%s) of 0x%x", name, sig, (unsigned int)classID);
            break;
        }
    } while (0);

    return ret;
}

jmethodID PJniHelper::getStaticMethodID(jclass classID, const pchar *name, const pchar *sig, JNIEnv *env)
{
    JNIEnv *pEnv = env;
    jmethodID ret = 0;

    do 
    {
        if (!pEnv)
        {
            if (!PJniHelper::getEnv(&pEnv))
            {
                break;
            }
        }
        
        ret = pEnv->GetStaticMethodID(classID, name, sig);
        if (!ret)
        {
            LOGD("Failed to get method %s(%s) of 0x%x", name, sig, (unsigned int)classID);
            break;
        }
    } while (0);

    return ret;
}

PString PJniHelper::jstring2string(jstring jstr)
{
    if (jstr == NULL)
    {
        return PString("");
    }
    
    JNIEnv *env = 0;

    if (!PJniHelper::getEnv(&env))
    {
        return PString("");
    }

    const pchar* chars = env->GetStringUTFChars(jstr, NULL);
    PString ret(chars);
    env->ReleaseStringUTFChars(jstr, chars);

    return ret;
}

pchar *PJniHelper::jstring2chars(jstring jstr)
{
    if (jstr == NULL)
    {
        return P_NULL;
    }
    
    JNIEnv *env = 0;

    if (!PJniHelper::getEnv(&env))
    {
        return P_NULL;
    }

    const pchar* chars = env->GetStringUTFChars(jstr, NULL);
    pchar *ret = pstrdup(chars);
    env->ReleaseStringUTFChars(jstr, chars);

    return ret;
}

