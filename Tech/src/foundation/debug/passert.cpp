// passert.cpp
// Assert used in debug.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "passert.h" 

#include <PFoundation/pcrt.h>
#include <PFoundation/pdebugutility.h>
#include <PFoundation/plog.h>

#include <stdarg.h>

PAssertStyleEnum g_assertStyle = P_ASSERT_STYLE_DEFAULT;

void P_APIENTRY pAssertFunction(const pchar* file, puint32 line, const PAssertStyleEnum style, ...)
{
    pchar buffer[8192];

    // Combine assert condition strings.
    va_list args;
    va_start(args, style);
    pstrncpy(buffer, "Assert: ", sizeof("Assert: "));
    const char *str = va_arg(args, const pchar *);
    while (str)
    {
        pstrcat(buffer, str);
        str = va_arg(args, const pchar *);
    }
    va_end(args);
    puint32 len = pstrlen(buffer);

    switch (style) 
    {
        case P_ASSERT_STYLE_LOGANDEXIT:
            {
                PLOG_INFO("%s\nPosition: %s(%d)\n", buffer, file, line);
                pDebugPrintStack(2);
                pabort();
                break;
            }
        case P_ASSERT_STYLE_DEBUG_BREAK:
            psprintf(buffer + len, 1024 - len, "\nPosition: %s(%d)\n", file, line);
            pDebugOutputString(buffer);
            pDebugBreak();
            break;
        case P_ASSERT_STYLE_DEBUG_INFO:
            psprintf(buffer + len, 1024 - len, "\nPosition: %s(%d)\n", buffer, file, line);
            pDebugOutputString(buffer);
            break;
        default:
            // ??
            break;
    }
}

void P_APIENTRY pAssertSetStyle(PAssertStyleEnum style)
{
    g_assertStyle = style;
}

