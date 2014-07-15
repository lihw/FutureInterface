// pandroidpreference.h
// The preference interfaces of FI Android application
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#ifndef PANDROIDPREFERENCE_H
#define PANDROIDPREFERENCE_H

#include <PFoundation/pstring.h>

#if defined P_ANDROID

#include <jni.h>

class P_DLLEXPORT PAndroidPreferences
{
public:
    static jobject preferences();
    static pint32 value(const pchar *name, const pint32 defaultValue);
    static PString value(const pchar *name, const pchar *defaultValue);
    static pbool value(const pchar *name, const pbool defaultValue);
    static pfloat32 value(const pchar *name, const pfloat32 defaultValue);
    static long value(const pchar *name, const long defaultValue);
};

#endif // !P_ANDROID

#endif // !PANDROIDPREFERENCE_H
