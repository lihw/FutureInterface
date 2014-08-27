// pwin32asset.cpp 
// Assets manager functions wrapper
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "../passet.h"

#include <PFoundation/passert.h>
#include <PFoundation/pnew.h>

PAsset P_APIENTRY pAssetOpen(const pchar *fileName)
{
    // The asset resources are in the same folder of
    // executable/working directory.
    const pchar *path = fileName;

    PAsset ret;
    ret.pHandle = pfopen(path, "rb"); 
    ret.pData = P_NULL;
    return ret;
}
    
void P_APIENTRY pAssetClose(PAsset *asset)
{
    PASSERT(asset != P_NULL);
    if (asset != P_NULL)
    {
        PFile *fp = (PFile *)asset->pHandle;
        pfclose(fp);
        asset->pHandle = P_NULL;

        PDELETEARRAY(asset->pData);
    }
}

const void * P_APIENTRY pAssetGetBuffer(PAsset *asset)
{
    PASSERT(asset != P_NULL);
    if (asset != P_NULL)
    {
        puint32 len = pAssetGetSize(asset);
        PFile *fp = (PFile *)asset->pHandle;
        
        puint8 *buffer = PNEWARRAY(puint8, len);
        pfseek(fp, 0, P_FILE_SEEK_FROM_BEGINNING);
        puint32 readLen = pfread(buffer, sizeof(puint8), len, fp);
        PASSERT(readLen == len);

        PASSERT(asset->pData == P_NULL);
        asset->pData = buffer;

        return asset->pData;
    }

    return P_NULL;
}

pint32 P_APIENTRY pAssetGetSize(PAsset *asset)
{
    PASSERT(asset != P_NULL);
    if (asset != P_NULL)
    {
        PFile *fp = (PFile *)asset->pHandle;
        pfseek(fp, 0, P_FILE_SEEK_FROM_END);
        return pftell(fp);
    }

    return 0;
}

pbool P_APIENTRY pAssetIsValid(PAsset *asset)
{
    return asset != P_NULL && asset->pHandle != P_NULL;
}

pint32 P_APIENTRY pAssetRead(PAsset *asset, void *buffer, puint32 count)
{
    PASSERT(asset != P_NULL);
    if (asset != P_NULL)
    {
        PFile *fp = (PFile *)asset->pHandle;
        
        puint32 readLen = pfread(buffer, sizeof(puint8), count, fp);
        PASSERT(readLen == count);
        
        return readLen;
    }

    return 0;
}
pint32 P_APIENTRY pAssetSeek(PAsset *asset, pint32 offset, PFileOriginEnum whence)
{
    PASSERT(asset != P_NULL);
    if (asset != P_NULL)
    {
        PFile *fp = (PFile *)asset->pHandle;
        
        return pfseek(fp, offset, whence);
        
    }

    return 0;
}

pint32 P_APIENTRY pAssetGetRemainingLength(PAsset *asset)
{
    PASSERT(asset != P_NULL);
    if (asset != P_NULL)
    {
        PFile *fp = (PFile *)asset->pHandle;
        pint32 currentPosition = pftell(fp);
        pfseek(fp, 0, P_FILE_SEEK_FROM_END);
        return (pftell(fp) - currentPosition);
    }

    return 0;
}
