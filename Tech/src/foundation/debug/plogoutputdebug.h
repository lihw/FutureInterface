// plogoutputdebug.h
// The log output to debug output (only effective
// in msvc and has the same effect as plogconsole
// in android) 
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PLOGOUTPUTDEBUG_H
#define PLOGOUTPUTDEBUG_H

#include <PFoundation/pabstractlogoutput.h>

class P_DLLEXPORT PLogOutputDebug : public PAbstractLogOutput
{
public:
    PLogOutputDebug();
    virtual ~PLogOutputDebug();

    virtual void print(const pchar *text, puint32 length);
};

#endif // !PLOGOUTPUTDEBUG_H
