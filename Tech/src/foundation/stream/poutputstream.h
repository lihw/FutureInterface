// poutputstream.h
// Write to file/memory/other.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
// 
// 
// Hongwei Li lihw81@gmail.com
// 

#ifndef POUTPUTSTREAM_H
#define POUTPUTSTREAM_H

#include <PFoundation/pabstractstream.h>

#include <PFoundation/pstring.h>

class PStreamObject;

class P_DLLEXPORT POutputStream : public PAbstractStream
{
public:
    POutputStream();
    virtual ~POutputStream();

    pbool createFromFile(const pchar* filePath, PStreamEndianessEnum endianness);
    pbool createFromMemory(puint8* buffer, puint32 size, PStreamEndianessEnum endianness);

    // How many bytes have been written to the stream so far.
    // \return the number of bytes that have been written.
    puint32 getWrittenBytes() const;

    pbool writeBytes(puint32 nbytes, const puint8* buffer);
    pbool write(puint32 nbytes, puint32& bytesWritten, const puint8* buffer);
    void flush();

    pbool writeUint8(puint8 value);
    pbool writeInt8(pint8 value);
    pbool writeUint16(puint16 value);
    pbool writeInt16(pint16 value);
    pbool writeUint32(puint32 value);
    pbool writeInt32(pint32 value);
    pbool writeFloat32(pfloat32 value);
    pbool writeBoolean(pbool value);
    pbool writeString(const PString& value);
    pbool writeString(const pchar* value);

    void skipBytes(puint32 nbytes);

 private:
    POutputStream(const POutputStream& other) {}
    void operator=(const POutputStream& other) {}
	
private:
    // The number of bytes written so far
    puint32 m_writtenBytes;
};


#endif // !POUTPUTSTREAM_H
