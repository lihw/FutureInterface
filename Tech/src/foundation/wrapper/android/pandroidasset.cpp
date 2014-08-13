// pandroidasset.cpp 
// Assets manager functions wrapper
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "../passet.h"

#include <PFoundation/passert.h>
#include <PFoundation/penvironment.h>
#include <PFoundation/pnew.h>


PAsset P_APIENTRY pAssetOpen(const pchar *fileName)
{
    PAsset ret;
    ret.pHandle = (void*)AAssetManager_open(g_pAssetManager, fileName, AASSET_MODE_STREAMING); 
    ret.pData = P_NULL;
    return ret;
}

void P_APIENTRY pAssetClose(PAsset *asset)
{
    PASSERT(asset != P_NULL);
    if (asset != P_NULL)
    {
        AAsset_close((AAsset*)asset->pHandle);
        asset->pHandle = P_NULL;

        PDELETEARRAY(asset->pData);
    }
}

const void* P_APIENTRY pAssetGetBuffer(PAsset *asset)
{
    PASSERT(asset != P_NULL && asset->pData == P_NULL);
    if (asset != P_NULL)
    {
        // Two way to get the content of the asset
        //
        // 1) to fetch the entire content of the asset with AAsset_read(). 
        // 2) use AAset_getBuffer()
        //
        // Using the first method we explicitly own the buffer memory, which
        // may be safer than using AAset_getBuffer. AAsset_getBuffer allocates
        // the memory for holding the resource content inside Asset. When
        // user calls AAsset_close, the memory buffer will be release if
        // exists.

        pint32 len = pAssetGetSize(asset);
        PASSERT(len > 0);
        asset->pData = PNEWARRAY(puint8, len);
        
        AAsset_seek((AAsset*)(asset->pHandle), 0, P_FILE_SEEK_FROM_BEGINNING);
        puint32 readLen = AAsset_read((AAsset*)(asset->pHandle), (void *)asset->pData, len);
        if (readLen != len)
        {
            PDELETEARRAY(asset->pData);
        }

        // FIXME: leave this line here for backup
        // asset->pData = AAsset_getBuffer((AAsset*)(asset->pHandle));
        
        return asset->pData;
     }

    return P_NULL;
}

pint32 P_APIENTRY pAssetGetSize(PAsset *asset)
{
    PASSERT(asset != P_NULL);
    if (asset != P_NULL)
    {
        return AAsset_getLength((AAsset*)(asset->pHandle));  
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
    if (asset != P_NULL && asset->pHandle != P_NULL)
    {
        return AAsset_read((AAsset*)(asset->pHandle), buffer, count);  
    }

    return 0;
}

pint32 P_APIENTRY pAssetSeek(PAsset *asset, pint32 offset, PFileOriginEnum whence)
{
    PASSERT(asset != P_NULL);
    if (asset != P_NULL && asset->pHandle != P_NULL)
    {
        return AAsset_seek((AAsset*)(asset->pHandle), offset, where);     
    }

    return 0;
}

pint32 P_APIENTRY pAssetgetRemainingLength(PAsset *asset)
{
    PASSERT(asset != P_NULL);
    if (asset != P_NULL && asset->pHandle != P_NULL)
    {
        return AAsset_getRemainingLength((AAsset*)(asset->pHandle));     
    }

    return 0;
}
