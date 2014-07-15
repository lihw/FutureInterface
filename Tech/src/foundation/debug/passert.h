// passert.h
// Assert used in debug.
//
// Copyright 2012 - 2014 Future Interface Li. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PASSERT_H
#define PASSERT_H

#include <PFoundation/pglobal.h>

//
// Assert that will be checked at compile time.
// the predicate needs to be decidable at compile time.
// The call can only be made were a C statement is expected.
//
#define P_COMPILE_TIME_ASSERT(pred) switch(0) { case 0: case (pred): break; }

// The styles of the assert.
enum PAssertStyleEnum
{
    P_ASSERT_STYLE_LOGANDEXIT,     ///< write to stdout/logcat and exit.
    P_ASSERT_STYLE_POPUP_BOX,      ///< use popup dialog box.
    P_ASSERT_STYLE_DEBUG_BREAK,    ///< OutputDebugString + debug break
    P_ASSERT_STYLE_DEBUG_INFO,     ///< outputDebugString

    P_ASSERT_STYLE_DEFAULT = P_ASSERT_STYLE_DEBUG_BREAK,
};

//
// Assert function.
// \param file in which file this assert is triggered.
// \param line at which line this assert is.
// \param style how to present this assert to user.
// \param ... the condition strings, and end with 0.
P_EXTERN void P_APIENTRY pAssertFunction(const pchar* file, puint32 line, const PAssertStyleEnum style, ...);

extern PAssertStyleEnum g_assertStyle;

// Set the assert style in the application-wise scope. The default style is P_ASSERT_STYLE_DEBUG_BREAK.
P_EXTERN void P_APIENTRY pAssertSetStyle(PAssertStyleEnum style);

#if defined P_DEBUG

#define PASSERT(condition) P_MULTILINE_MACRO_BEGIN \
    if (!(condition)) \
    { \
        pAssertFunction(__FILE__, __LINE__, g_assertStyle, (#condition), 0); \
    } \
P_MULTILINE_MACRO_END

#define PASSERTINFO(condition, text) P_MULTILINE_MACRO_BEGIN \
    if (!(condition)) \
    { \
        pAssertFunction(__FILE__, __LINE__, g_assertStyle, text, 0); \
    } \
P_MULTILINE_MACRO_END

#define PASSERT_NOTREACHABLE(text) \
        pAssertFunction(__FILE__, __LINE__, g_assertStyle, text, 0); 

#define PASSERT_NOTIMPLEMENTED() \
        pAssertFunction(__FILE__, __LINE__, g_assertStyle, __FUNCTION__, " has not been implemented", 0); 

#else

#define PASSERT(condition)
#define PASSERTINFO(condition, text)
#define PASSERT_NOTREACHABLE(text) 
#define PASSERT_NOTIMPLEMENTED() 

#endif // P_DEBUG


#endif // !pASSERT_H
