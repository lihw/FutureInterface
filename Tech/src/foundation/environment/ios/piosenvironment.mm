// piosenvironment.mm
// iOS global variables.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "piosenvironment.h"

#import <UIKit/UIKit.h>

#include <PFoundation/pcrt.h>
#include <PFoundation/pnew.h>
#include "../penvironment.h"



const pchar *g_pExternalStoragePath = P_NULL;
const pchar *g_pApplicationPath     = P_NULL;
const pchar *g_pDocumentPath        = P_NULL;

void pEnvironmentInitialize(void *data)
{
    // Document path.
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    
    g_pDocumentPath = pstrdup([documentsDirectory UTF8String]);
    
    // Application path.
    NSString *bundlePath = [[NSBundle mainBundle] bundlePath];
    g_pApplicationPath = pstrdup([bundlePath UTF8String]);
}

void pEnvironmentUninitialize()
{
    PDELETEARRAY(g_pExternalStoragePath);
    PDELETEARRAY(g_pApplicationPath);
    PDELETEARRAY(g_pDocumentPath);
}
