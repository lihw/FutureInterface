// poutputstream.cpp
// Write to file/memory/other.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
// 
// Hongwei Li lihw81@gmail.com
// 
// 

#include "poutputstream.h"

#include <PFoundation/pnew.h>
#include <PFoundation/plog.h>
#include <PFoundation/passert.h>

#include "pstreamobject_private.h"

POutputStream::POutputStream()
    : PAbstractStream()
{
    m_writtenBytes = 0;
}

POutputStream::~POutputStream()
{
}

pbool POutputStream::createFromFile(const pchar *filePath, PStreamEndianessEnum endianness)
{
    PASSERT(m_object == P_NULL);
    if (m_object != P_NULL)
    {
        PLOG_ERROR("Unable to reopen the stream for writing");
        return false;
    }

    m_object = PNEW(PStreamFile(filePath, true));

    m_object->open();

    m_endianness = endianness;

    return true;
}

pbool POutputStream::createFromMemory(puint8 *buffer, puint32 size, PStreamEndianessEnum endianness)
{
    PASSERT(m_object == P_NULL);
    if (m_object != P_NULL)
    {
        PLOG_ERROR("Unable to reopen the stream for writing");
        return false;
    }

    m_object = PNEW(PStreamMemory(buffer, size));

    m_object->open();

    m_endianness = endianness;

    return true;
}

pbool POutputStream::writeBytes(puint32 nbytes, const puint8* buffer)
{
    PASSERT(m_object != P_NULL);
    if (m_object == P_NULL)
    {
        PLOG_ERROR("Output stream is unavailable");
        return false;
    }
    puint32 bytesWritten;
    if (!m_object->write(nbytes, bytesWritten, buffer))
    {
        return false;
    }
    if (bytesWritten != nbytes)
    {
        PLOG_WARNING("The written data is less than expected");
        m_writtenBytes = bytesWritten;
        return false;
    }

    m_writtenBytes += bytesWritten;
    return true;
}

pbool POutputStream::write(puint32 nbytes, puint32& bytesWritten, const puint8* buffer)
{
    return m_object->write(nbytes, bytesWritten, buffer);
}

pbool POutputStream::writeUint8(puint8 value)
{
    return writeBytes(1, &value);
}

pbool POutputStream::writeInt8(pint8 value)
{
    puint8 buffer[1] = {(puint8)value};
    return writeBytes(1, buffer);
}

pbool POutputStream::writeUint16(puint16 value)
{
    puint8 buffer[2];
    switch (m_endianness)
    {
        case P_STREAM_ENDIANNESS_BIG:
            buffer[0] = (puint8)(value);
            buffer[1] = (puint8)(value >> 8);
            break;
        case P_STREAM_ENDIANNESS_LITTLE:
            buffer[0] = (puint8)(value >> 8);
            buffer[1] = (puint8)(value);
            break;
        case P_STREAM_ENDIANNESS_PLATFORM:
            {
                puint8* valueBuffer = (puint8*)&value;
                buffer[0] = valueBuffer[0];
                buffer[1] = valueBuffer[1];
            }
            break;
        default:
            PLOG_ERROR("Invalid endianness");
            return false;
    }
    
    if (!writeBytes(2, buffer))
    {
        return false;
    }

    return true;
}

pbool POutputStream::writeInt16(pint16 value)
{
    puint16 v = (puint16)value;
    if (!writeUint16(v))
    {
        return false;
    }

    return true;
}

pbool POutputStream::writeUint32(puint32 value)
{
    puint8 buffer[4];
    
    switch (m_endianness)
    {
        case P_STREAM_ENDIANNESS_BIG:
            buffer[0] = (puint8)(value >> 24);
            buffer[1] = (puint8)(value >> 16);
            buffer[2] = (puint8)(value >> 8);
            buffer[3] = (puint8)value;
            break;
        case P_STREAM_ENDIANNESS_LITTLE:
            buffer[0] = (puint8)value;
            buffer[1] = (puint8)(value >> 8);
            buffer[2] = (puint8)(value >> 16);
            buffer[3] = (puint8)(value >> 24);
            break;
        case P_STREAM_ENDIANNESS_PLATFORM:
            {
                puint8* valueBuffer = (puint8*)&value;
                buffer[0] = valueBuffer[0];
                buffer[1] = valueBuffer[1];
                buffer[2] = valueBuffer[2];
                buffer[3] = valueBuffer[3];
            }
            break;
        default:
            PLOG_ERROR("Invalid endianness");
            return false;
    }
    
    if (!writeBytes(4, buffer))
    {
        return false;
    }

    return true;
}

pbool POutputStream::writeInt32(pint32 value)
{
    puint32 v;
    v = (puint32)value;
    if (!writeUint32(v))
    {
        return false;
    }

    return true;
}

pbool POutputStream::writeFloat32(pfloat32 value)
{
    puint32 v = *(puint32*)&value;
    return writeUint32(v);
}

pbool POutputStream::writeBoolean(pbool value)
{
    puint8 buffer[1];

    if (value)
    {
        buffer[0] = 1;
    }
    else
    {
        buffer[0] = 0;
    }
    return writeBytes(1, buffer);
}

pbool POutputStream::writeString(const PString& value)
{
    return writeBytes(value.length(), (const puint8*)value.c_str()) &&
           writeUint8(0);
}

pbool POutputStream::writeString(const pchar* value)
{
    const pchar* p = value;
    do
    {
        if (!writeInt8(*p))
        {
            return false;
        }

        p++;
    } while (*p != 0);

    return true;
}

void POutputStream::flush()
{
    PASSERT(m_object != P_NULL);
    if (m_object == P_NULL)
    {
        PLOG_ERROR("Output stream is unavailable");
        return ;
    }

    m_object->flush();
}

void POutputStream::skipBytes(puint32 nbytes)
{
    PASSERT(m_object != P_NULL);
    if (m_object == P_NULL)
    {
        PLOG_ERROR("Output stream is unavailable");
        return ;
    }

    m_object->skip(nbytes);

    m_writtenBytes += nbytes;
}

