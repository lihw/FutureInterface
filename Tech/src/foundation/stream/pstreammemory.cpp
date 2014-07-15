// pstreammemory.cpp
// Read/write to memory buffer as stream.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
// 
// Hongwei Li lihw81@gmail.com
// 

#include "pstreamobject_private.h"

#include <PFoundation/pnew.h>
#include <PFoundation/plog.h>
#include <PFoundation/passert.h>

PStreamMemory::PStreamMemory(puint8* buffer, puint32 bufferSize)
{
    m_position = 0;
    m_buffer = buffer;
    m_bufferSize = bufferSize;
}

PStreamMemory::~PStreamMemory()
{
    close();
}

pbool PStreamMemory::read(puint32 nbytes, puint32& bytesRead, puint8* buffer)
{
    bytesRead = pMin(nbytes, m_bufferSize - m_position);
    pmemcpy(buffer, m_buffer + m_position, bytesRead);
    m_position += bytesRead;
    return true;
}

pbool PStreamMemory::readLine(puint32 length, puint8* buffer)
{
    pint32 l = pMin((pint32)length - 1, (pint32)m_bufferSize - (pint32)m_position);
    if (l <= 0)
    {
        return false;
    }

    pint32 i;
    for (i = 0; i < l; i++)
    {
        buffer[i] = m_buffer[m_position + i];

        if (buffer[i] == '\n')
        {
            break;
        }
    }
    buffer[i + 1] = 0;
    m_position += i;
    // When we meet the '\n', we need to move the position one byte more.
    if (i != l)
    {
        m_position += 1;
    }
    return true;
}

pbool PStreamMemory::write(puint32 nbytes, puint32& bytesWritten, const puint8* buffer)
{
    bytesWritten = pMin(nbytes, m_bufferSize - m_position);
    pmemcpy(m_buffer + m_position, buffer, bytesWritten);
    m_position += bytesWritten;
    return true;
}

void PStreamMemory::seek(puint32 position)
{
    m_position = pMin(position, m_bufferSize);
}

void PStreamMemory::open()
{
}

void PStreamMemory::close()
{
    PDELETEARRAY(m_buffer);
    m_position = 0;
    m_bufferSize = 0;
}

void PStreamMemory::skip(puint32 bytes)
{
    puint32 bytesSkipped = pMin(bytes, m_bufferSize - m_position);
    m_position += bytesSkipped;
}

pbool PStreamMemory::isEndOfStream()
{
    return m_bufferSize == m_position; 
}

puint32 PStreamMemory::getPosition()
{
    return m_position;
}

puint32 PStreamMemory::getSize()
{
    return m_bufferSize;
}

void PStreamMemory::flush()
{

}

pbool PStreamMemory::isValid() const
{
    // memory stream always return true.
    return true;
}
