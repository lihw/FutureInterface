// pandroidcrt.cpp 
// standard C functions and system functions wrapper
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "../pcrt.h"

#include <PFoundation/pmemory_module.h>
#include <PFoundation/passert.h>
#include <PFoundation/penvironment.h>

#include <stdio.h>
#include <unistd.h> // usleep

#include <android/log.h>


puint32 P_CCONV pprintf(const pchar* format, ...)
{
    va_list arguments;

    va_start(arguments, format);   

    return __android_log_vprint(ANDROID_LOG_INFO, "FI Tech", format, arguments);
}

puint32 P_CCONV pvsprintf(pchar* buffer, puint32 size, const pchar* format, va_list arguments)
{
    return vsnprintf(buffer, size, format, arguments);
}

puint32 P_CCONV psprintf(pchar* buffer, puint32 size, const pchar* format, ...)
{
    va_list arguments;

    va_start(arguments, format);   

    return vsnprintf(buffer, size, format, arguments);
}

#include <string.h>

void P_APIENTRY pstrncpy(pchar* dst, const pchar* src, puint32 n)
{
    if (n == 0)
    {
        return ;
    }
    
    const pchar *ret = strncpy(dst, src, n - 1);
    // If dst is not terminated with '\0', we manually terminate it.
    if (ret == &dst[n - 1])
    {
        dst[n - 1] = 0;
    }
}

const pchar* P_APIENTRY pstrstr(const pchar* str1, const pchar *str2)
{
    return strstr(str1, str2);
}

pint32 P_APIENTRY pstrcmp(const pchar* left, const pchar* right)
{
    if (left == P_NULL && right != P_NULL)
    {
        return 1;
    }
    else if (left != P_NULL && right == P_NULL)
    {
        return -1;
    }
    else if (left == P_NULL && right == P_NULL)
    {
        return 0;
    }

    return strcmp(left, right);
}

pint32 P_APIENTRY pstrncmp(const pchar* left, const pchar* right, puint32 n)
{
    if (left == P_NULL && right != P_NULL)
    {
        return 1;
    }
    else if (left != P_NULL && right == P_NULL)
    {
        return -1;
    }
    else if (left == P_NULL && right == P_NULL)
    {
        return 0;
    }

    return strncmp(left, right, n);
}

puint32 P_APIENTRY pstrlen(const pchar* str)
{
    return (puint32)strlen(str);
}

pchar* P_APIENTRY pstrdup(const pchar* str)
{
    if (str == P_NULL)
    {
        return P_NULL;
    }

    puint32 l = (puint32)strlen(str);

    pchar* ret = PNEWARRAY(pchar, l + 1);
    strncpy(ret, str, l);
    ret[l] = 0;

    return ret;
}

void P_APIENTRY pstrcat(pchar* dst, const pchar* src)
{
    PASSERT(dst != src);
    strcat(dst, src);
}

const pchar* P_APIENTRY pstrrchr(const pchar* str, pchar d)
{
    return strrchr(str, d);
}

const pchar* P_APIENTRY pstrchr(const pchar* str, pchar d)
{
    return strchr(str, d);
}

#include <stdlib.h>

void* P_APIENTRY pmalloc(puint32 size)
{
    return malloc((size_t)size);
}

void P_APIENTRY pfree(void* pointer)
{
    free(pointer);
}

void* P_APIENTRY prealloc(void* pointer, puint32 newSize)
{
    return realloc(pointer, newSize);
}

void P_APIENTRY pexit(pint32 exitCode)
{
    exit(exitCode);
}

void P_APIENTRY pmemset(void* pointer, puint8 c, puint32 nbytes)
{
    memset(pointer, c, nbytes);
}

void P_APIENTRY pmemcpy(void* dst, const void* src, puint32 nbytes)
{
    memcpy(dst, src, nbytes);
}

pint32 P_APIENTRY pmemcmp(const void *buf1, const void *buf2, puint32 nbytes)
{
    return memcmp(buf1, buf2, nbytes);
}

pint32 P_APIENTRY patoi(const pchar* text)
{
    return atoi(text);
}

pfloat32 P_APIENTRY patof(const pchar* text)
{
    return (pfloat32)atof(text);
}

static const pint32 P_FILE_SEEK_TABLE[] = {SEEK_SET, SEEK_CUR, SEEK_END};


PFile* P_APIENTRY pfopen(const pchar* fileName, const pchar* mode)
{
    FILE* file;
    file = fopen(fileName, mode);
    if ((file != P_NULL))
    {
        return (PFile*)(file);
    }
    return P_NULL;    
}

pint32 P_APIENTRY pfclose(PFile* file)
{
    return (pint32)fclose((FILE*)file);
}

bool P_APIENTRY pfeof(PFile* file)
{
    return feof((FILE*)file) != 0;
}

pint32 P_APIENTRY pferror(PFile* file)
{
    return (pint32)ferror((FILE*)file);
}

pint32 P_APIENTRY pfseek(PFile* file, pint32 offset, PFileOriginEnum origin)
{
    return (pint32)fseek((FILE*)file, (pint32)offset, P_FILE_SEEK_TABLE[origin]);
}

pint32 P_APIENTRY pftell(PFile* file)
{
    return (pint32)ftell((FILE*)file);
}

pint32 P_APIENTRY pfflush(PFile* file)
{
    return (pint32)fflush((FILE*)file);
}

pint32 P_APIENTRY pfgetc(PFile* file)
{
    return (pint32)fgetc((FILE*)file);
}

pint32 P_APIENTRY pfputc(pint32 character, PFile* file)
{
    return (pint32)fputc(character, (FILE*)file);
}

puint32 P_APIENTRY pfread(void* buffer, puint32 elementSize, puint32 elementCount, PFile* file)
{
    return (puint32)fread(buffer, elementSize, elementCount, (FILE*)file);
}

puint32 P_APIENTRY pfwrite(const void* buffer, puint32 elementSize, puint32 elementCount, PFile* file)
{
    return (puint32)fwrite(buffer, elementSize, elementCount, (FILE*)file);
}

pchar* P_APIENTRY pfgets(pchar* buffer, puint32 size, PFile* file)
{
    return fgets(buffer, size, (FILE*)file);
}

pint32 P_APIENTRY pfputs(pchar* buffer, PFile* file)
{
    return fputs(buffer, (FILE*)file);
}

void P_APIENTRY pabort()
{
    abort();
}

pint32 P_APIENTRY pfremove(const pchar* fileName)
{
    return remove(fileName);
}

void P_APIENTRY psleep(pfloat32 milliseconds)
{
    usleep((puint32)(milliseconds * 1000.0f));
}

