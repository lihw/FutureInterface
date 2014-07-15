// plogoutputdebug.cpp
// The log output to debug output (only effective
// in msvc and has the same effect as plogconsole
// in android) 
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "plogoutputdebug.h"

#include <PFoundation/pdebugutility.h>

PLogOutputDebug::PLogOutputDebug()
    : PAbstractLogOutput()
{
}

PLogOutputDebug::~PLogOutputDebug()
{
}

void PLogOutputDebug::print(const pchar *text, puint32 length)
{
    pDebugOutputString(const_cast<pchar *>(text));
}
