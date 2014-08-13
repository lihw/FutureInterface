// pstreamasset.cpp
// Read to asset  as stream.
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

PStreamAsset::PStreamAsset(const PAsset &asset)
{
    m_asset = asset;
}

PStreamAsset::~PStreamAsset()
{
    close();
}

pbool PStreamAsset::read(puint32 nbytes, puint32& bytesRead, puint8* buffer)
{
    bytesRead = pAssetRead(&m_asset, buffer, nbytes);
    return true;
}

pbool PStreamAsset::readLine(puint32 length, puint8* buffer)
{
    PASSERT_NOTIMPLEMENTED();
    return true;
}

pbool PStreamAsset::write(puint32 nbytes, puint32& bytesWritten, const puint8* buffer)
{
    PASSERT_NOTIMPLEMENTED();
    return true;
}

void PStreamAsset::seek(puint32 position)
{
    pAssetSeek(&m_asset, position, P_FILE_SEEK_FROM_BEGINNING);
}

void PStreamAsset::open()
{
}

void PStreamAsset::close()
{
    if (pAssetIsValid(&m_asset))
    {
        pAssetClose(&m_asset);
    }
}

void PStreamAsset::skip(puint32 bytes)
{
    pAssetSeek(&m_asset, bytes, P_FILE_SEEK_FROM_HERE);
}

pbool PStreamAsset::isEndOfStream()
{
    return (pAssetGetRemainingLength(&m_asset) == 0);
}

puint32 PStreamAsset::getPosition()
{
    return (puint32)(getSize() - pAssetGetRemainingLength(&m_asset));
}

puint32 PStreamAsset::getSize()
{
    return pAssetGetSize(&m_asset);
}

void PStreamAsset::flush()
{
    PASSERT_NOTIMPLEMENTED();
}

pbool PStreamAsset::isValid() const
{
    return pAssetIsValid(const_cast<PAsset*>(&m_asset));
}
