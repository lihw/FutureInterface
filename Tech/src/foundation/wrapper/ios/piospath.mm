// ppath.mm
// Path operations
// 
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "../ppath.h"

#include <PFoundation/penvironment.h>

const pchar * P_APIENTRY pPathGetApplicationDirectory()
{
    return g_pApplicationPath;
}

const pchar * P_APIENTRY pPathGetDocumentDirectory()
{
    return g_pDocumentPath;
}

const pchar * P_APIENTRY pPathGetDelimiter()
{
    return "/";
}

const pchar * P_APIENTRY pPathGetExternalStoragePath()
{
    return P_NULL;
}
