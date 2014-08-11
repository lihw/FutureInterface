// passet.h
// android assets manager library wrapper
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PASSET_H
#define PASSET_H

#include <PFoundation/pglobal.h>

#include <PFoundation/pcrt.h>

struct PAsset
{
    void *pHandle;
    const puint8 *pData;
};

P_EXTERN PAsset P_APIENTRY pAssetOpen(const pchar *fileName);
P_EXTERN void P_APIENTRY pAssetClose(PAsset *asset);
P_EXTERN const void* P_APIENTRY pAssetGetBuffer(PAsset *asset);
P_EXTERN pint32 P_APIENTRY pAssetGetSize(PAsset *asset); // return the number of bytes.
P_EXTERN pbool P_APIENTRY pAssetIsValid(PAsset *asset);
P_EXTERN pint32 P_APIENTRY pAssetRead(PAsset *asset, void *buffer, puint32 count);
P_EXTERN pint32 P_APIENTRY pAssetSeek(PAsset *asset, pint32 offset, PFileOriginEnum where);
P_EXTERN pint32 P_APIENTRY pAssetGetRemainingLength(PAsset *asset);

#endif // !PASSET_H
