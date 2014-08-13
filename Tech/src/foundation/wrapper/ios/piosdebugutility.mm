// piosdebugutility.mm
// Debug helper functions under iOS
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "../pdebugutility.h"

#include <PFoundation/passert.h>
#include <PFoundation/pcrt.h>

// Standard library 
#include <stdio.h>

// iOS API 
#import <UIKit/UIKit.h>

void P_APIENTRY pDebugOutputString(const pchar *message)
{
    NSLog([NSString stringWithUTF8String:message]);
}

void P_APIENTRY pDebugBreak()
{
    kill(getpid(), SIGINT) ; 
}

void P_APIENTRY pDebugPrintStack(pint32 skip)
{
    PASSERT_NOTIMPLEMENTED();
}


