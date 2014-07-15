// pabstractlogoutput.h
// The log output
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PABSTRACTLOGOUTPUT_H
#define PABSTRACTLOGOUTPUT_H

#include <PFoundation/pglobal.h>

class P_DLLEXPORT PAbstractLogOutput
{
public:
    PAbstractLogOutput();
    virtual ~PAbstractLogOutput();

    void setName(const pchar *name);

    virtual void print(const pchar *text, puint32 length) = 0;

private:
    pchar m_name[128];
};


#endif // !PABSTRACTLOGOUTPUT_H

