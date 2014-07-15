// pglerror.h
// To detect and return the graphics error.
// 
// Copyright 2012 - 2014 Future Interface Li. 
// This software is licensed under the terms of the MIT license.
// 
// Hongwei Li lihw81@gmail.com
// 

#ifndef P_GLERROR_H
#define P_GLERROR_H

#include <PFoundation/plog.h>

P_EXTERN pbool P_APIENTRY pGlErrorCheck(const pchar* file, puint32 line, PLogVerbosityEnum logLevel);
#define pGlErrorCheckError() pGlErrorCheck(__FILE__, __LINE__, P_LOG_ERROR)
#define pGlErrorCheckWarning() pGlErrorCheck(__FILE__, __LINE__, P_LOG_WARNING)
#define pGlErrorCheckAbort() if (!pGlErrorCheck(__FILE__, __LINE__, P_LOG_ERROR)) { return false; }

#endif  // P_GLERROR_H


