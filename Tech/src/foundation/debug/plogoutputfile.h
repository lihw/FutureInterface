// plogoutputfile.h
// The log output to file
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PLOGOUTPUTFILE_H
#define PLOGOUTPUTFILE_H

#include <PFoundation/pabstractlogoutput.h>

#include <PFoundation/pcrt.h>

class P_DLLEXPORT PLogOutputFile : public PAbstractLogOutput
{
public:
    PLogOutputFile(const pchar *filename);
    virtual ~PLogOutputFile();

    virtual void print(const pchar *text, puint32 length);

private:
    PFile *m_fhandle;
};


#endif // !PLOGOUTPUTFILE_H




