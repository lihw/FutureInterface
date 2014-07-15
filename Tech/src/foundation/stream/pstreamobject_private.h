// pstreamobject_private.h
// stream object 
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
// 
// Hongwei Li lihw81@gmail.com
// 

#ifndef PSTREAMOBJECT_PRIVATE_H
#define PSTREAMOBJECT_PRIVATE_H


#include <PFoundation/pstring.h>

#include <PFoundation/pcrt.h>
#include <PFoundation/passet.h>

class PStreamObject
{
public:
    PStreamObject() {};
    virtual ~PStreamObject() {};

    // read to the stream.
    virtual pbool read(puint32 nbytes, puint32& bytesRead, puint8* buffer) = 0;

    // read till the new line.
    virtual pbool readLine(puint32 size, puint8* buffer) = 0;
    
    // write to the stream.
    virtual pbool write(puint32 nbytes, puint32& bytesWritten, const puint8* buffer) = 0;

    // reposition the read/write in the stream.
    virtual void seek(puint32 position) = 0;

    // open the stream.
    virtual void open() = 0;
    
    // close the stream.
    virtual void close() = 0;

    // skip the a stream a few bytes.
    virtual void skip(puint32 position) = 0;

    // reaches the end of the stream.
    virtual pbool isEndOfStream() = 0;

    // Flush cached buffer to output.
    virtual void flush() = 0;
    
    // Return how many bytes that have been read from the beginning.
    virtual puint32 getPosition() = 0;

    // Return stream size.
    virtual puint32 getSize() = 0;

    virtual pbool isValid() const = 0;
};

class PStreamFile : public PStreamObject
{
public:
    PStreamFile(const pchar* filePath, pbool writable);
    virtual ~PStreamFile();

    virtual pbool read(puint32 nbytes, puint32& bytesRead, puint8* buffer);
    virtual pbool readLine(puint32 size, puint8* buffer);
    virtual pbool write(puint32 nbytes, puint32& bytesWritten, const puint8* buffer);
    virtual void seek(puint32 position);
    virtual void open();
    virtual void close();
    virtual void skip(puint32 bytes);
    virtual pbool isEndOfStream();
    virtual void flush();
    virtual puint32 getPosition();
    virtual puint32 getSize();
    virtual pbool isValid() const;

private:
    PString     m_filePath;
    PFile      *m_file;
    pbool        m_writable;
    puint32     m_fileSize;
};

class PStreamMemory : public PStreamObject
{
public:
    PStreamMemory(puint8* buffer, puint32 bufferSize);
    virtual ~PStreamMemory();

    virtual pbool read(puint32 nbytes, puint32& bytesRead, puint8* buffer);
    virtual pbool readLine(puint32 length, puint8* buffer);
    virtual pbool write(puint32 nbytes, puint32& bytesWritten, const puint8* buffer);
    virtual void seek(puint32 position);
    virtual void open();
    virtual void close();
    virtual void skip(puint32 bytes);
    virtual pbool isEndOfStream();
    virtual void flush();
    virtual puint32 getPosition();
    virtual puint32 getSize();
    virtual pbool isValid() const;

private:
    puint8   *m_buffer;
    puint32   m_bufferSize;
    puint32   m_position;
};

class PStreamAsset : public PStreamObject
{
public:
    PStreamAsset(const PAsset& asset);
    virtual ~PStreamAsset();

    virtual pbool read(puint32 nbytes, puint32& bytesRead, puint8* buffer);
    virtual pbool readLine(puint32 length, puint8* buffer);
    virtual pbool write(puint32 nbytes, puint32& bytesWritten, const puint8* buffer);
    virtual void seek(puint32 position);
    virtual void open();
    virtual void close();
    virtual void skip(puint32 bytes);
    virtual pbool isEndOfStream();
    virtual void flush();
    virtual puint32 getPosition();
    virtual puint32 getSize();
    virtual pbool isValid() const;

private:
    PAsset         m_asset;
};


#endif // !PSTREAMOBJECT_PRIVATE_H
