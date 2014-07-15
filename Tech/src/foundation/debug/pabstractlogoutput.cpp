// pabstractlogoutput.cpp
// The log output
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pabstractlogoutput.h"

#include <PFoundation/pcrt.h>

PAbstractLogOutput::PAbstractLogOutput()
{
    m_name[0] = 'l';
    m_name[1] = 'o';
    m_name[2] = 'g';
    m_name[3] = 0;
}

PAbstractLogOutput::~PAbstractLogOutput()
{
}

void PAbstractLogOutput::setName(const pchar *name)
{
    pstrncpy(m_name, name, 127);
}

