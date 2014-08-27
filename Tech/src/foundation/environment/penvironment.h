// penvironment.h
// enviroment specifics
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#ifndef PSYSTEMENVIRONMENT_H
#define PSYSTEMENVIRONMENT_H

#include <PFoundation/pglobal.h>

#if defined P_ANDROID
#  include "android/pandroidenvironment.h"
#elif defined P_IOS
#  include "ios/piosenvironment.h"
#elif defined P_WIN32
#  include "win32/pwin32environment.h"
#else
#  error "Unsupported platform" 
#endif

// The external storage on the device only applicable to android now.
P_EXTERN const pchar     *g_pExternalStoragePath;
// The path of working directory of executable.
P_EXTERN const pchar     *g_pApplicationPath;
// The path of permanent document storage.
P_EXTERN const pchar     *g_pDocumentPath;

P_EXTERN void pEnvironmentInitialize(void *data);
P_EXTERN void pEnvironmentUninitialize();

#endif
