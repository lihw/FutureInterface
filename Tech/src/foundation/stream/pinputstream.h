    // pinputstream.h
// Read file/memory/other streams.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
// 
// Hongwei Li lihw81@gmail.com
// 

#ifndef PINPUTSTREAM_H
#define PINPUTSTREAM_H

#include <PFoundation/pabstractstream.h>
#include <PFoundation/pstring.h>

#include <PFoundation/passet.h>

class P_DLLEXPORT PInputStream : public PAbstractStream
{
public:
    PInputStream();
    virtual ~PInputStream();

    pbool createFromFile(const pchar* filePath, PStreamEndianessEnum endianness);
    pbool createFromMemory(puint8* buffer, puint32 size, PStreamEndianessEnum endianness);
    pbool createFromAsset(const PAsset &asset, PStreamEndianessEnum endianness);
    
    pbool isEndOfStream() const;

    pbool readBytes(puint32 nbytes, puint8* buffer);
    pbool read(puint32 nbytes, puint32& bytesRead, puint8* buffer);

    pbool readUint8(puint8* value);
    pbool readInt8(pint8* value);
    pbool readUint16(puint16* value);
    pbool readInt16(pint16* value);
    pbool readUint32(puint32* value);
    pbool readInt32(pint32* value);
    pbool readFloat32(pfloat32* value);
    pbool readBoolean(pbool* value);
    pbool readString(PString& value);
    pbool readString(puint32 length, pchar* value);

    // return the number of bytes read or -1 when fails.
    pint32 readAllBytes(puint8*& buffer);
    pint32 readAllBytesAsString(puint8*& buffer);
    pbool readLine(puint32 length, pchar* buffer);

    void skipBytes(puint32 nbytes);

    puint32 getReadBytes();
    puint32 getSize();
    void seek(puint32 position);

private:
    PInputStream(const PInputStream& other) {}
    void operator=(const PInputStream& other) {}
};


#endif // !PINPUTSTREAM_H
