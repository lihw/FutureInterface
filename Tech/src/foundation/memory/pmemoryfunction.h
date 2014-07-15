// pmemoryfunction.h
// The header file of memory manager.
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//
// 

#ifndef PMEMORYFUNCTION_H
#define PMEMORYFUNCTION_H

#include <PFoundation/pglobal.h>

#if defined P_ANDROID
# include <stddef.h> // for size_t
#endif

#if P_ENABLE_MEMORY_DEBUGGING == 1

void * P_APIENTRY pDebugMalloc(size_t size, const pchar *file, const pint32 line, pbool isArray = false);
void P_APIENTRY pDebugFree(void *ptr, pbool isArray = false);

#endif // P_ENABLE_MEMORY_DEBUGGING == 1

#endif // !PMEMORYFUNCTION_H
