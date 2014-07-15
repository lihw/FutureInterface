// plogoutputfile.cpp
// The log output to file
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "plogoutputfile.h"


PLogOutputFile::PLogOutputFile(const pchar *filename)
    : PAbstractLogOutput()
{
    m_fhandle = pfopen(filename, P_FILE_WRITEONLY_BINARY);
    if (m_fhandle == P_NULL)
    {
        // FIXME: the logging system may not be initialized yet, we need to
        // use old printf()
        pprintf("(warning)%s:%d,failed to open %s for logging",
                __FILE__, __LINE__, filename);
    }
}

PLogOutputFile::~PLogOutputFile()
{
    if (m_fhandle != P_NULL)
    {
        pfflush(m_fhandle);
        pfclose(m_fhandle);
    }
}

void PLogOutputFile::print(const pchar *text, puint32 length)
{
    if (m_fhandle != P_NULL)
    {
        pfwrite(text, length, 1, m_fhandle);
        pfwrite("\n", 1, 1, m_fhandle);
    }
}




