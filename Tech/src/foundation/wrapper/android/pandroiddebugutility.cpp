// pandroiddebugutility.cpp 
// Debug helper functions under Android
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "../pdebugutility.h"

#include <PFoundation/passert.h>
#include <PFoundation/pcrt.h>

// Standard library 
#include <stdio.h>

// Android API 
#include <jni.h>
#include <android/log.h>

void P_APIENTRY pDebugOutputString(const pchar* message)
{
    __android_log_print(ANDROID_LOG_DEBUG, "FI Tech", "%s",  message);
}

void P_APIENTRY pDebugBreak()
{
    __android_log_assert("assert", "FI Tech", "ASSERT in '%s' at %d.", __FILE__, __LINE__); 
}

void P_APIENTRY pDebugPrintStack(pint32 skip)
{
    PASSERT_NOTIMPLEMENTED();
}

