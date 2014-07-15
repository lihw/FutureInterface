// pnew.cpp
// When memory debugging is enabled, we override global new and delete
// operator. Note that this memory debuggin is only for engine development,
// we should not release it to application dev.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//


#include "pnew.h"

#include <PFoundation/pmemoryfunction.h>

#if P_ENABLE_MEMORY_DEBUGGING == 1

void * operator new(size_t size, const pchar *file, const pint32 line)
{
    return pDebugMalloc(size, file, line);
}

void operator delete(void *ptr)
{
    pDebugFree(ptr);
}

void operator delete(void *ptr, const pchar *file, const pint32 line)
{
    pDebugFree(ptr);
}

void * operator new[](size_t size, const pchar *file, const pint32 line)
{
    return pDebugMalloc(size, file, line, true);
}

void operator delete[](void *ptr)
{
    pDebugFree(ptr, true);
}

void operator delete[](void *ptr, const pchar *file, const pint32 line)
{
    pDebugFree(ptr, true);
}

#endif // P_ENABLE_MEMORY_DEBUGGING == 1


