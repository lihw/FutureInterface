// piosasset.mm
// ios assets manager library wrapper
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#import "../passet.h"

#import <UIKit/UIKit.h>

#include <PFoundation/passert.h>
#include <PFoundation/pnew.h>
#include <PFoundation/ppath.h>
#include <PFoundation/pcrt.h>
#include <PFoundation/plog.h>


PAsset P_APIENTRY pAssetOpen(const pchar *fileName)
{
    pchar path[4096];
    pstrncpy(path, pPathGetApplicationDirectory(), 4096);
    pstrcat(path, pPathGetDelimiter());
    pstrcat(path, fileName);

    PAsset ret;
    
    ret.pHandle = (void *)fopen(path, "rb");
    ret.pData   = P_NULL;
    
    return ret;
}

void P_APIENTRY pAssetClose(PAsset *asset)
{
    if (asset != P_NULL)
    {
        fclose((FILE *)(asset->pHandle));
    }
}

const void * P_APIENTRY pAssetGetBuffer(PAsset *asset)
{
    if (asset != P_NULL && asset->pHandle != P_NULL)
    {
        FILE *fp = (FILE *)asset->pHandle;
        fseek(fp, 0, SEEK_END);
        size_t numBytes = ftell(fp);
        
        asset->pData = PNEWARRAY(puint8, numBytes);
        
        fseek(fp, 0, SEEK_SET);
        size_t readNumBytes = fread((void *)(asset->pData), 1, numBytes, fp);
        if (readNumBytes != numBytes)
        {
            PDELETEARRAY(asset->pData);
        }
        
        return asset->pData;
    }
    
    return P_NULL;
}

pint32 P_APIENTRY pAssetGetSize(PAsset *asset)
{
    if (asset != P_NULL && asset->pHandle != P_NULL)
    {
        FILE *fp = (FILE *)asset->pHandle;
        fseek(fp, 0, SEEK_END);
        size_t numBytes = ftell(fp);
        
        return (pint32)numBytes;
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
        return fread(buffer, 1, count, (FILE *)(asset->pHandle));
    }
    
    return 0;
}

pint32 P_APIENTRY pAssetSeek(PAsset *asset, pint32 offset, PFileOriginEnum where)
{
    PASSERT(asset != P_NULL);
    if (asset != P_NULL && asset->pHandle != P_NULL)
    {
        int pos[] =
        {
            SEEK_SET,
            SEEK_CUR,
            SEEK_END,
        };
        
        return fseek((FILE *)(asset->pHandle), offset, pos[where]);
    }
    
    return 0;
}

pint32 P_APIENTRY pAssetGetRemainingLength(PAsset *asset)
{
    if (asset != P_NULL && asset->pHandle != P_NULL)
    {
        FILE *fp = (FILE *)(asset->pHandle);
        size_t pos = ftell(fp);
        fseek(fp, 0, SEEK_END);
        size_t totalLength = ftell(fp);
        
        pint32 ret = (pint32)(totalLength - pos);
        
        // Restore the current file I/O pointer.
        fseek(fp, pos, SEEK_SET);
        
        return ret;
    }
    
    return 0;
}
