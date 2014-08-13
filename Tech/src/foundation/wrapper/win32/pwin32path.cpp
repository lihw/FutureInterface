// ppath.cpp
// Path operations
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "../ppath.h"

#include <windows.h>

#include <PFoundation/pcrt.h>

const pchar * P_APIENTRY pPathGetApplicationDirectory()
{
    // FIXME: as in win32, we only have one context per activity, so it is
    // safe to use a local static variable and return its address.
    static char fullPath[512 + 1];
    ::GetCurrentDirectoryA(512 + 1, fullPath);
    return fullPath;
}

const pchar * P_APIENTRY pPathGetDocumentDirectory()
{
    // FIXME: as in win32, we only have one context per activity, so it is
    // safe to use a local static variable and return its address.
    static char fullPath[512 + 1];
    ::GetCurrentDirectoryA(512 + 1, fullPath);
    return fullPath;
}

const pchar * P_APIENTRY pPathGetDelimiter()
{
    return "\\";
}

P_EXTERN const pchar * P_APIENTRY pPathGetExternalStoragePath()
{
    return P_NULL;
}

