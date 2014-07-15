// plogoutputconsole.h
// The log output to console
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PLOGOUTPUTCONSOLE_H
#define PLOGOUTPUTCONSOLE_H

#include <PFoundation/pabstractlogoutput.h>

class P_DLLEXPORT PLogOutputConsole : public PAbstractLogOutput
{
public:
    PLogOutputConsole();
    virtual ~PLogOutputConsole();

    virtual void print(const pchar *text, puint32 length);
};


#endif // !PLOGOUTPUTCONSOLE_H


