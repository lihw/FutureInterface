// pconststring.h
// The C string manipulation
//
// Copyright 2012 - 2014 Future Interface . 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PCONSTSTRING_H
#define PCONSTSTRING_H

#include <PFoundation/pglobal.h>

// a -> A
P_EXTERN pchar P_APIENTRY pCharToLower(pchar c);
// A -> a
P_EXTERN pchar P_APIENTRY pCharToUpper(pchar c);
// AbC -> abc
P_EXTERN const pchar * P_APIENTRY pStringToLower(const pchar *in, puint32 bufferSize, pchar *out);
// AbC -> ABC
P_EXTERN const pchar * P_APIENTRY pStringToUpper(const pchar *in, puint32 bufferSize, pchar *out);
// Abc -> Abc
P_EXTERN const pchar * P_APIENTRY pStringToTitle(const pchar *in, puint32 bufferSize, pchar *out);
// Trim the leading spaces, including \t and ' '
P_EXTERN const pchar * P_APIENTRY pStringTrim(const pchar *in);
// Convert a string to a float and return the possible starting position of next float. E.g., 3.2,2.1 will output
// 3.2 and return the pointers to the second '2'. P_NULL will be returned if there is any error or the '\0' is seen. A valid float number can contain a lead '-' which followed by a number of digits and one '.', e.g., -1.0
// 0.123, 1. and 10
P_EXTERN const pchar * P_APIENTRY pStringUnpackFloat(const pchar *in, pfloat32 *out);
// Ditto
P_EXTERN const pchar * P_APIENTRY pStringUnpackInt(const pchar *in, pint32 *out);
// Ditto
P_EXTERN const pchar * P_APIENTRY pStringUnpackString(const pchar *in, pchar *out);
// Trim off the leading alphabetic letters. Say input is zfar:-1.0, the return value will be -1.0
P_EXTERN const pchar * P_APIENTRY pStringUnpackTrimAnnotation(const pchar *in);


#endif // !PCONSTSTRING_H
