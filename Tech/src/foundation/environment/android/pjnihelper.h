// pjnihelper.h
// The JNI helper of FI Android application
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#ifndef PJNIHELPER_H
#define PJNIHELPER_H

#include <jni.h>

#include <android/log.h>

#include <PFoundation/pstring.h>
#include <PFoundation/penvironment.h>

class P_DLLEXPORT PJniHelper
{
public:
    static JavaVM* getJavaVM();
    static void setJavaVM(JavaVM *javaVM);
    static pbool getEnv(JNIEnv **env);
    // Notice: The return value jclass need to call DeleteLocalRef() to release.
    static jclass getClassID(const char *className, JNIEnv *env = 0);
    static jclass getClassID(jobject obj, JNIEnv *env = 0);
    // Notice: The return value jmethodID need to call DeleteLocalRef() to release.
    static jmethodID getMethodID(jclass classID, const pchar *name, const pchar *sig, JNIEnv *env = 0);
    static jmethodID getStaticMethodID(jclass classID, const pchar *name, const pchar *sig, JNIEnv *env = 0);
    static PString jstring2string(jstring jstr);
    // Notice: The return value "chars" need to call PDELETEARRAY() to release.
    static pchar *jstring2chars(jstring jstr);
    
private:
    static JavaVM *s_pJavaVM;
};

#endif // !PJNIHELPER_H
