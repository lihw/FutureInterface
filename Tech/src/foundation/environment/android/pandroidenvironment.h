// pandroidenvironment.h
// Android environment specifics
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
P_EXTERN jobject          g_pContext;
P_EXTERN AAssetManager   *g_pAssetManager;

P_EXTERN void P_APIENTRY pEnvironmentSetJavaVM(JavaVM *vm);

#endif // !PANDROIDENVIRONMENT_H
