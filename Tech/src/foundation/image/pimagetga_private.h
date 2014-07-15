// pimagetga_private.h
// The header file of tga decoder.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PIMAGETGA_PRIVATE_H
#define PIMAGETGA_PRIVATE_H

#include <PFoundation/pimage.h>

class PInputStream;
class POutputStream;


struct TgaHeader
{
    puint8 idLength;
    puint8 colorMapType;

    // The image type.
#define TGA_TYPE_MAPPED     1
#define TGA_TYPE_COLOR      2
#define TGA_TYPE_GRAY       3
#define TGA_TYPE_MAPPED_RLE 9
#define TGA_TYPE_COLOR_RLE  10
#define TGA_TYPE_GRAY_RLE   11
    puint8 imageType;

    // Color Map Specification.
    // We need to separately specify high and low bytes to avoid endianness
    // and alignment problems.
    puint8 colorMapIndexLo, colorMapIndexHi;
    puint8 colorMapLengthLo, colorMapLengthHi;
    puint8 colorMapSize;

    // Image Specification.
    puint8 xOriginLo, xOriginHi;
    puint8 yOriginLo, yOriginHi;

    puint8 widthLo, widthHi;
    puint8 heightLo, heightHi;

    puint8 bpp;

    // Image descriptor.
    // 3-0: attribute bpp
    // 4:   left-to-right ordering
    // 5:   top-to-bottom ordering
    // 7-6: zero
#define TGA_DESC_ABITS      0x0f
#define TGA_DESC_HORIZONTAL 0x10
#define TGA_DESC_VERTICAL   0x20
    puint8 descriptor;
};

struct TgaFooter
{
    puint32 extensionAreaOffset;
    puint32 developerDirectoryOffset;
#define TGA_SIGNATURE "TRUEVISION-XFILE"
#define TGA_SIGNATURE_SIZE (26)
    pint8 signature[16];
    pint8 dot;
    pint8 null;
};

#define RLE_PACKETSIZE 0x80
struct RLEstate 
{
  puint8   *statebuf;
  pint32    statelen;
  pint32    laststate;
};

P_EXTERN pbool P_APIENTRY pImageTGARead(PInputStream& inputStream, puint32& width, puint32& height, 
    PImagePixelFormatEnum& format, puint8*& data);

P_EXTERN pbool P_APIENTRY pImageTGAWrite(POutputStream& outputStream, puint32 width, puint32 height, 
    PImagePixelFormatEnum format, const puint8* data);

#endif // !PIMAGETGA_PRIVATE_H
