// piosasset.m
// ios assets manager library wrapper
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#import "../passet.h"

PAsset P_APIENTRY pAssetOpen(const pchar *fileName)
{
}

void P_APIENTRY pAssetClose(PAsset *asset)
{
}

const void* P_APIENTRY pAssetGetBuffer(PAsset *asset)
{
}

pint32 P_APIENTRY pAssetGetSize(PAsset *asset)
{
}

pbool P_APIENTRY pAssetIsValid(PAsset *asset)
{
}

pint32 P_APIENTRY pAssetRead(PAsset *asset, void *buffer, puint32 count)
{
}

pint32 P_APIENTRY pAssetSeek(PAsset *asset, pint32 offset, PFileOriginEnum where)
{
}

pint32 P_APIENTRY pAssetGetRemainingLength(PAsset *asset)
{
}
