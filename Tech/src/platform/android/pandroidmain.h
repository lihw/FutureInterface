// p_android_main.h
// The JNI interface of FI Android application
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#ifndef P_ANDROID_MAIN_H
#define P_ANDROID_MAIN_H

#include <PFoundation/pglobal.h>

#if defined P_ANDROID

#include <jni.h>

P_EXTERNC_BEGIN

JNIEXPORT void JNICALL Java_com_fi_tech_PContext_createContextNative(JNIEnv* env, jobject obj, jstring name);
JNIEXPORT jint JNICALL Java_com_fi_tech_PContext_getContextIdByNameNative(JNIEnv* env, jobject obj, jstring contextName);
JNIEXPORT void JNICALL Java_com_fi_tech_PContext_initializeContextNative(JNIEnv* env, jobject obj, jint contextId ,jint width, jint height);
JNIEXPORT void JNICALL Java_com_fi_tech_PContext_uninitializeContextNative(JNIEnv* env, jobject obj, jint contextId);
JNIEXPORT void JNICALL Java_com_fi_tech_PContext_pauseNative(JNIEnv* env, jobject obj, jint contextId);
JNIEXPORT void JNICALL Java_com_fi_tech_PContext_resumeNative(JNIEnv* env, jobject obj, jint contextId);
JNIEXPORT void JNICALL Java_com_fi_tech_PContext_resizeNative(JNIEnv* env, jobject obj, jint contextId, jint width, jint height);
JNIEXPORT void JNICALL Java_com_fi_tech_PContext_orientationChangeNative(JNIEnv* env, jobject obj, jint contextId, jint orientation);
// update returns JNI_TRUE on success, JNI_FALSE if P_APPLICATION_STATE_QUITTING 
JNIEXPORT jboolean JNICALL Java_com_fi_tech_PContext_updateNative(JNIEnv* env, jobject obj, jint contextId);
JNIEXPORT jlong JNICALL Java_com_fi_tech_PContext_touchEventNative(JNIEnv* env, jobject obj, jint contextId, jint cursorCount, jint state);
JNIEXPORT void JNICALL Java_com_fi_tech_PContext_touchCursorNative(JNIEnv* env, jobject obj, jint contextId, jlong touchEvent, 
	                                                              jint index, jint id, jfloat x, jfloat y, 
	                                                              jfloat pressure, jfloat area, int state,jlong timeStamp);
JNIEXPORT void JNICALL Java_com_fi_tech_PContext_focusEventNative(JNIEnv* env, jobject obj, jint contextId, jboolean focusState);
JNIEXPORT void JNICALL Java_com_fi_tech_PContext_keyEventNative(JNIEnv* env, jobject obj, jint contextId, jint keyCode, jint state, jlong timestamp);
JNIEXPORT void JNICALL Java_com_fi_tech_PContext_setFreeCameraNative(JNIEnv* env, jobject obj, jint contextId, jboolean onoff);
JNIEXPORT void JNICALL Java_com_fi_tech_PContext_setHUDNative(JNIEnv* env, jobject obj, jint contextId, jboolean onoff);

JNIEXPORT jboolean JNICALL Java_com_fi_tech_PNativeLibrary_initializeLibraryNative(JNIEnv* env, jobject obj, jobject context);

void pAndroidDoNothing();

P_EXTERNC_END

#endif // !P_ANDROID

#endif // !P_ANDROID_MAIN_H
