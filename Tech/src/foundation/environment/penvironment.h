// penvironment.h
// The environment of the system
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#ifndef PSYSTEMENVIRONMENT_H
#define PSYSTEMENVIRONMENT_H

#include <PFoundation/pglobal.h>


#if defined P_ANDROID
#  include "android/pandroidenvironment.h"
#elif defined P_IOS
# include "ios/piosenvironment.h"
#endif


#endif
