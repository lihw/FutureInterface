// pdebugutility.h
// Debug utility wrapper
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PDEBUGUTILITY_H
#define PDEBUGUTILITY_H

#include <PFoundation/pglobal.h>


P_EXTERN void P_APIENTRY pDebugOutputString(const pchar* message);

P_EXTERN void P_APIENTRY pDebugBreak();

P_EXTERN void P_APIENTRY pDebugPrintStack(pint32 skip);

// FIXME: win32 needs to link dgbhelp.lib
#if defined P_MSC
# pragma comment (lib, "dbghelp.lib")
#endif 


#endif // !PDEBUGUTILITY_H
