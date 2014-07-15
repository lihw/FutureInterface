// pandroidenvironment.h
// Android global variables.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//


#ifndef PANDROIDENVIRONMENT_H
#define PANDROIDENVIRONMENT_H

#include <PFoundation/pglobal.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

// Instance of the user-defined Android activity or view class
extern jobject          g_pContext;
extern AAssetManager   *g_pAssetManager;
extern const pchar     *g_pSDCardPath;
extern const pchar     *g_pSystemPath;

P_EXTERN void P_APIENTRY pEnvironmentClear();

P_EXTERN void P_APIENTRY pEnvironmentSetContext(jobject context);

P_EXTERN void P_APIENTRY pEnvironmentSetJavaVM(JavaVM *vm);

#endif // !PANDROIDENVIRONMENT_H
