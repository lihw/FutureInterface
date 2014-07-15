// pmemorymanager.cpp
// The header file of memory manager.
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//
// 

#include "pmemoryfunction.h"

#include <PFoundation/pmemorydebugger.h>
#include <PFoundation/pcrt.h>

#if P_ENABLE_MEMORY_DEBUGGING == 1

void * P_APIENTRY pDebugMalloc(size_t size, const pchar *file, const pint32 line, pbool isArray)
{
    if (PMemoryDebugger::getInstance())
    {
        return PMemoryDebugger::getInstance()->malloc(size, file, line, isArray);
    }
    else
    {
        return pmalloc(size);
    }
}

void P_APIENTRY pDebugFree(void *ptr, pbool isArray)
{
    if (PMemoryDebugger::getInstance())
    {
        PMemoryDebugger::getInstance()->free(ptr, isArray);
    }
    else
    {
        pfree(ptr);
    }
}

#endif // P_ENABLE_MEMORY_DEBUGGING == 1
