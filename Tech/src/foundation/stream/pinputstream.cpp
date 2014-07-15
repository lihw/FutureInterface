// pinputstream.cpp
// Read file/memory/other streams.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
// 
// Hongwei Li lihw81@gmail.com
// 

#include "pinputstream.h"

#include <PFoundation/pnew.h>
#include <PFoundation/plog.h>
#include <PFoundation/passert.h>

#include "pstreamobject_private.h"


PInputStream::PInputStream()
    : PAbstractStream()
{
}

PInputStream::~PInputStream()
{
    if (m_object != P_NULL)
        m_object->close();
}

pbool PInputStream::createFromFile(const pchar* filePath, 
    PStreamEndianessEnum endianness)
{
    if (m_object != P_NULL)
    {
        PLOG_WARNING("Reopen the stream for reading");
        PDELETE(m_object);
    }

    m_object = PNEW(PStreamFile(filePath, false));

    m_object->open();

    m_endianness = endianness;

    return m_object->isValid();
}

pbool PInputStream::createFromMemory(puint8* buffer, puint32 size, 
    PStreamEndianessEnum endianness)
{
    if (m_object != P_NULL)
    {
        PLOG_WARNING("Reopen the stream for reading");
        PDELETE(m_object);
    }

    m_object = PNEW(PStreamMemory(buffer, size));

    m_object->open();

    m_endianness = endianness;

    return true;
}
pbool PInputStream::createFromAsset(const PAsset &asset,PStreamEndianessEnum endianness)
{
    if (m_object != P_NULL)
    {
        PLOG_WARNING("Reopen the stream for reading");
        PDELETE(m_object);
    }

    m_object = PNEW(PStreamAsset(asset));

    m_object->open();

    m_endianness = endianness;

    return true;
}


pbool PInputStream::isEndOfStream() const
{
    return m_object->isEndOfStream();
}

pbool PInputStream::readBytes(puint32 nbytes, puint8* buffer)
{
    if (m_object == P_NULL)
    {
        PLOG_ERROR("Intput stream is unavailable");
        return false;
    }

    puint32 bytesRead;
    if (!m_object->read(nbytes, bytesRead, buffer))
    {
        return false;
    }

    if (nbytes != bytesRead)
    {
        PLOG_ERROR("The number of bytes read is not as expected.");
        return false;
    }

    return true;
}

pbool PInputStream::read(puint32 nbytes, puint32& bytesRead, puint8* buffer)
{
    if (m_object == P_NULL)
    {
        PLOG_ERROR("Intput stream is unavailable");
        return false;
    }

    return m_object->read(nbytes, bytesRead, buffer);
}

pbool PInputStream::readUint8(puint8* value)
{
    puint8 buffer[1];
    if (!readBytes(1, buffer))
    {
        return false;
    }
    
    *value = buffer[0];

    return true;
}

pbool PInputStream::readInt8(pint8* value)
{
    puint8 buffer[1];
    if (!readBytes(1, buffer))
    {
        return false;
    }
    
    *value = (pint8)buffer[0];

    return true;
}

pbool PInputStream::readUint16(puint16* value)
{
    puint8 buffer[2];
    if (!readBytes(2, buffer))
    {
        return false;
    }
    
    switch (m_endianness)
    {
        case P_STREAM_ENDIANNESS_BIG:
            *value = ((puint16)(buffer[1]) | ((puint16)(buffer[0]) << 8));
            break;
        case P_STREAM_ENDIANNESS_LITTLE:
            *value = ((puint16)(buffer[0]) | ((puint16)(buffer[1]) << 8));
            break;
        case P_STREAM_ENDIANNESS_PLATFORM:
            *value = *(puint16*)buffer;
            break;
        default:
            PLOG_ERROR("Invalid endianness");
            return false;
    }

    return true;
}

pbool PInputStream::readInt16(pint16* value)
{
    puint16 v;
    if (!readUint16(&v))
    {
        return false;
    }

    *value = (pint16)v;
    return true;
}

pbool PInputStream::readUint32(puint32* value)
{
    puint8 buffer[4];
    if (!readBytes(4, buffer))
    {
        return false;
    }
    
    switch (m_endianness)
    {
        case P_STREAM_ENDIANNESS_BIG:
            *value = ((puint32)(buffer[3]) | 
                     ((puint32)(buffer[2]) << 8) |
                     ((puint32)(buffer[1]) << 16) |
                     ((puint32)(buffer[0]) << 24));
            break;
        case P_STREAM_ENDIANNESS_LITTLE:
            *value = ((puint32)(buffer[0]) | 
                     ((puint32)(buffer[1]) << 8) |
                     ((puint32)(buffer[2]) << 16) |
                     ((puint32)(buffer[3]) << 24));
            break;
        case P_STREAM_ENDIANNESS_PLATFORM:
            *value = *(puint32*)buffer;
            break;
        default:
            PLOG_ERROR("Invalid endianness");
            return false;
    }

    return true;
}

pbool PInputStream::readInt32(pint32* value)
{
    puint32 v;
    if (!readUint32(&v))
    {
        return false;
    }

    *value = (pint32)v;
    return true;
}

pbool PInputStream::readFloat32(pfloat32* value)
{
    puint8 buffer[4];
    if (!readBytes(4, buffer))
    {
        return false;
    }
    
    puint32 v;
    switch (m_endianness)
    {
        case P_STREAM_ENDIANNESS_BIG:
            v= ((puint32)(buffer[3]) | 
               ((puint32)(buffer[2]) << 8) |
               ((puint32)(buffer[1]) << 16) |
               ((puint32)(buffer[0]) << 24));
            break;
        case P_STREAM_ENDIANNESS_LITTLE:
            v= ((puint32)(buffer[0]) | 
               ((puint32)(buffer[1]) << 8) |
               ((puint32)(buffer[2]) << 16) |
               ((puint32)(buffer[3]) << 24));
            break;
        case P_STREAM_ENDIANNESS_PLATFORM:
            v= *(puint32*)buffer;
            break;
        default:
            PLOG_ERROR("Invalid endianness");
            return false;
    }

    *value = *(pfloat32*)&v;
    return true;
}

pbool PInputStream::readBoolean(pbool* value)
{
    puint8 buffer[1];
    if (!readBytes(1, buffer))
    {
        return false;
    }
    
    *value = (buffer[0] != 0);
    return true;
}

pbool PInputStream::readString(puint32 length, pchar* buffer)
{
    if (readBytes(length, (puint8*)buffer))
    {
        buffer[length] = 0;
        return true;
    }

    return false;
}

pbool PInputStream::readString(PString& string)
{
    pchar buffer[1024];
    puint32 index = 0;

    string = PString();

    pchar c;
    while (readInt8(&c))
    {
        if (c != 0)
        {
            buffer[index++] = c;
        }
        else
        {
            break;
        }

        if (index >= 1023)
        {
            index = 0;
            buffer[1023] = 0;
            string += PString(buffer);
        }
    }
            
    buffer[index] = 0;
    string += PString(buffer);

    return true;
}

pint32 PInputStream::readAllBytes(puint8*& buffer)
{
    // Check the stream object.
    if (m_object == P_NULL)
    {
        PLOG_ERROR("Intput stream is unavailable");
        return -1;
    }
    
    // Reach the end.
    if (m_object->getPosition() >= m_object->getSize())
    {
        return -1;
    }

    // Get the remaining byte number of input stream.
    puint32 bufferSize = m_object->getSize() - m_object->getPosition();

    buffer = PNEWARRAY(puint8, bufferSize + 1);

    puint32 bytesRead = 0;
    if (!read(bufferSize, bytesRead, buffer))
    {
        PDELETEARRAY(buffer);
        return -1;
    }

    buffer[bytesRead] = 0;
    
    return (pint32)bytesRead;
}

pint32 PInputStream::readAllBytesAsString(puint8*& buffer)
{
    // Check the stream object.
    if (m_object == P_NULL)
    {
        PLOG_ERROR("Intput stream is unavailable");
        buffer = P_NULL;
        return -1;
    }
    
    // Reach the end.
    if (m_object->getPosition() >= m_object->getSize())
    {
        buffer = P_NULL;
        return -1;
    }

    // Get the remaining byte number of input stream.
    puint32 bufferSize = m_object->getSize() - m_object->getPosition();

    buffer = PNEWARRAY(puint8, bufferSize + 1);

    puint32 bytesRead = 0;
    if (!read(bufferSize, bytesRead, buffer))
    {
        PDELETEARRAY(buffer);
        return -1;
    }

    buffer[bytesRead] = 0;
    
    return (pint32)bytesRead;
}

pbool PInputStream::readLine(puint32 length, pchar* buffer)
{
    if (m_object == P_NULL)
    {
        PLOG_ERROR("Intput stream is unavailable");
        return false;
    }

    if (!m_object->readLine(length, (puint8*)buffer))
    {
        return false;
    }

    return true;
}

void PInputStream::skipBytes(puint32 nbytes)
{
    if (m_object == P_NULL)
    {
        PLOG_ERROR("Intput stream is unavailable");
    }
    else
    {
        m_object->skip(nbytes);
    }
}

puint32 PInputStream::getReadBytes()
{
    if (m_object == P_NULL)
    {
        PLOG_ERROR("Intput stream is unavailable");
        return -1;
    }

    puint32 nbytes = m_object->getPosition();
    if (pint32(nbytes) < 0)
    {
        PLOG_ERROR("Can't obtain the number of read bytes");
    }
    return nbytes;
}

puint32 PInputStream::getSize()
{
    if (m_object == P_NULL)
    {
        PLOG_ERROR("Intput stream is unavailable");
        return 0;
    }
    else
    {
        return m_object->getSize();
    }
}

void PInputStream::seek(puint32 position)
{
    if (m_object == P_NULL)
    {
        PLOG_ERROR("Intput stream is unavailable");
    }
    else
    {
        m_object->seek(position);
    }
}

