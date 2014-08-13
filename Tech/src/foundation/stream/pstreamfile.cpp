// p_stream_file.cpp
// Read/write to file as a stream.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
// 
// Hongwei Li lihw81@gmail.com
// 

#include "pstreamobject_private.h"


#include <PFoundation/pnew.h>
#include <PFoundation/passert.h>
#include <PFoundation/pcrt.h>


PStreamFile::PStreamFile(const pchar* filePath, pbool writable)
    : m_filePath(filePath)
{
    m_file = P_NULL;
    m_writable = writable;
    m_fileSize = 0;
}

PStreamFile::~PStreamFile()
{
    if (m_file != P_NULL)
    {
        close();
    }
}

pbool PStreamFile::read(puint32 nbytes, puint32& bytesRead, puint8* buffer)
{
    if (m_file == P_NULL)
    {
        PLOG_ERROR("Unable to open file for reading");
        return false;
    }

    bytesRead = pfread(buffer, sizeof(puint8), nbytes, m_file);
    return true;
}

pbool PStreamFile::readLine(puint32 length, puint8* buffer)
{
    if (m_file == P_NULL)
    {
        PLOG_ERROR("Unable to open file for reading");
        return false;
    }

    pchar* p = pfgets((pchar*)buffer, length, m_file);
    if (p == P_NULL)
    {
        return false;
    }

    return true;
}

pbool PStreamFile::write(puint32 nbytes, puint32& bytesWritten, const puint8* buffer)
{
    if (m_file == P_NULL)
    {
        return false;
    }

    puint32 tempBytesWritten = 0;
    puint32 bytesRemaining = nbytes;
    const puint8* p = buffer;

    do
    {
        tempBytesWritten = pfwrite(p, sizeof(puint8), bytesRemaining, m_file);
        bytesRemaining -= tempBytesWritten;
        p += tempBytesWritten;
    } while (bytesRemaining && tempBytesWritten);

    bytesWritten = nbytes - bytesRemaining;

    return true;
}

void PStreamFile::seek(puint32 position)
{
    // we don't have seek when writing.
    if (m_writable)
    {
        return ;
    }

    PASSERT(m_file != P_NULL);
    if (m_file == P_NULL)
    {
        PLOG_ERROR("Unable to open file for reading");
        return ;
    }

    pfseek(m_file, position, P_FILE_SEEK_FROM_BEGINNING);
}

void PStreamFile::open()
{
    PASSERT(m_file == P_NULL);

    if (m_writable)
    {
        m_file = pfopen(m_filePath.c_str(), P_FILE_WRITEONLY_BINARY);
    }
    else
    {
        m_file = pfopen(m_filePath.c_str(), P_FILE_READONLY_BINARY);
    }

    // Get file size.
    if (m_file != P_NULL)
    {
        pfseek(m_file, 0, P_FILE_SEEK_FROM_END);
        m_fileSize = pftell(m_file);
        pfseek(m_file, 0, P_FILE_SEEK_FROM_BEGINNING);
    }
}

void PStreamFile::close()
{
    if (m_file != P_NULL)
    {
        pfclose(m_file);
        m_file = P_NULL;

        m_fileSize = 0;
    }
}

void PStreamFile::skip(puint32 bytes)
{
    PASSERT(m_file != P_NULL);
    if (m_file == P_NULL)
    {
        PLOG_ERROR("Unable to open file for reading");
        return ;
    }

    pfseek(m_file, bytes, P_FILE_SEEK_FROM_HERE);
}
    
pbool PStreamFile::isEndOfStream()
{
    return pfeof(m_file) || (getPosition() == m_fileSize);
}

void PStreamFile::flush()
{
    pfflush(m_file);
}

puint32 PStreamFile::getPosition()
{
    return (puint32)pftell(m_file);
}

puint32 PStreamFile::getSize()
{
    return m_fileSize;
}

pbool PStreamFile::isValid() const
{
    return m_file != P_NULL;
}

