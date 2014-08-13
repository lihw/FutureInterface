// pcrt.h
// C runtime library wrapper
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PCRT_H
#define PCRT_H

#include <PFoundation/pglobal.h>

#include <stdarg.h>


//
// <cstdio>
//

P_EXTERN puint32 P_CCONV pprintf(const pchar* format, ...);

P_EXTERN puint32 P_CCONV pvsprintf(pchar* buffer, puint32 size, const pchar* format, va_list arguments); 

P_EXTERN puint32 P_CCONV psprintf(pchar* buffer, puint32 size, const pchar* format, ...);

//
// <cstring>
//
// Copy at most n characters of src to dst.
// If src has more than n characters excluding the ending '\0', only
// the first n - 1 characters are copied to dst. The dst is always '\0' terminated
// One needs to make sure dst has n + 1 characters long.
P_EXTERN void P_APIENTRY pstrncpy(pchar* dst, const pchar* src, puint32 n);

P_EXTERN pint32 P_APIENTRY pstrcmp(const pchar* left, const pchar* right);

P_EXTERN pint32 P_APIENTRY pstrncmp(const pchar* left, const pchar* right, puint32 n);

P_EXTERN void P_APIENTRY pstrcat(pchar* dst, const pchar* src);

P_EXTERN puint32 P_APIENTRY pstrlen(const pchar* str);

P_EXTERN const pchar* P_APIENTRY pstrrchr(const pchar* str, pchar d);

P_EXTERN const pchar* P_APIENTRY pstrchr(const pchar* str, pchar d);

P_EXTERN const pchar* P_APIENTRY pstrstr(const pchar* str1, const pchar *str2);

P_EXTERN pchar* P_APIENTRY pstrdup(const pchar* str);

P_EXTERN void P_APIENTRY pstrcat(pchar* dst, const pchar* src);

//
// <cstdlib>
//
P_EXTERN void* P_APIENTRY pmalloc(puint32 size);

P_EXTERN void P_APIENTRY pfree(void* pointer);

P_EXTERN void* P_APIENTRY prealloc(void* pointer, puint32 newSize);

P_EXTERN void P_APIENTRY pexit(pint32 exitCode);

P_EXTERN void P_APIENTRY pmemset(void* pointer, puint8 c, puint32 nbytes);

P_EXTERN void P_APIENTRY pmemcpy(void* dst, const void* src, puint32 nbytes);

P_EXTERN pint32 P_APIENTRY pmemcmp(const void *buf1, const void *buf2, puint32 nbytes);

P_EXTERN pint32 P_APIENTRY patoi(const pchar* text);

P_EXTERN pfloat32 P_APIENTRY patof(const pchar* text);

P_EXTERN void P_APIENTRY pabort();


// <cstdio>
typedef void PFile;

#define P_FILE_WRITEONLY          "w"
#define P_FILE_WRITEONLY_BINARY   "wb"
#define P_FILE_READONLY           "r"
#define P_FILE_READONLY_BINARY    "rb"

enum PFileOriginEnum
{
    P_FILE_SEEK_FROM_BEGINNING,
    P_FILE_SEEK_FROM_HERE,
    P_FILE_SEEK_FROM_END,
};

// \return when some errors happen, it returns P_NULL.
P_EXTERN PFile* P_APIENTRY pfopen(const pchar* fileName, const pchar* mode);
P_EXTERN pint32 P_APIENTRY pfclose(PFile* file);
P_EXTERN pint32 P_APIENTRY pferror(PFile* file);
P_EXTERN pbool P_APIENTRY pfeof(PFile* file);
P_EXTERN pbool P_APIENTRY pfeof(PFile* file);
P_EXTERN pint32 P_APIENTRY pfseek(PFile* file, pint32 offset, PFileOriginEnum origin);
P_EXTERN pint32 P_APIENTRY pftell(PFile* file);
P_EXTERN pint32 P_APIENTRY pfflush(PFile* file);
P_EXTERN pint32 P_APIENTRY pfgetc(PFile* file);
P_EXTERN pint32 P_APIENTRY pfputc(PFile* file);
P_EXTERN pchar* P_APIENTRY pfgets(pchar* buffer, puint32 size, PFile* file);
P_EXTERN pint32 P_APIENTRY pfputs(pchar* buffer, PFile* file);
P_EXTERN puint32 P_APIENTRY pfread(void* buffer, puint32 elementSize, puint32 elementCount, PFile* file);
P_EXTERN puint32 P_APIENTRY pfwrite(const void* buffer, puint32 elementSize, puint32 elementCount, PFile* file);
P_EXTERN pint32 P_APIENTRY pfremove(const pchar* fileName);

// thread sleep function.
P_EXTERN void P_APIENTRY psleep(pfloat32 milliseconds);

#endif // !PCRT_H
