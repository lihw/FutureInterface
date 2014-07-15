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

const pchar * P_APIENTRY pPathGetSystemDirectory()
{
    // return "C:\\Windows";
    // FIXME: as in win32, we only have one context per activity, so it is
    // safe to use a local static variable and return its address.
    static char full_path[512 + 1];
    ::GetCurrentDirectoryA(512 + 1, full_path);
    return full_path;
}

const pchar * P_APIENTRY pPathGetDelimiter()
{
    return "\\";
}

P_EXTERN const pchar * P_APIENTRY pPathGetSDCardPath()
{
    return P_NULL;
}

