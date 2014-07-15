// plogoutputconsole.cpp
// The log output to console
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "plogoutputconsole.h"

#include <PFoundation/pcrt.h>
   
PLogOutputConsole::PLogOutputConsole()
    : PAbstractLogOutput()
{
}

PLogOutputConsole::~PLogOutputConsole()
{
}

void PLogOutputConsole::print(const pchar *text, puint32 length)
{
    pprintf("%s\n", text);
}



