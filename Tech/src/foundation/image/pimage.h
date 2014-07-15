// pimage.h
// The image object.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PIMAGE_H
#define PIMAGE_H

#include <PFoundation/pglobal.h>

class PInputStream;
class POutputStream;

enum PImagePixelFormatEnum 
{
    P_IMAGE_PIXELFORMAT_RGBA8888,
    P_IMAGE_PIXELFORMAT_RGB888,
    P_IMAGE_PIXELFORMAT_RA88,
    P_IMAGE_PIXELFORMAT_R8,
    P_IMAGE_PIXELFORMAT_LUMINANCE,
    P_IMAGE_PIXELFORMAT_COLOR_INDEX,
    P_IMAGE_PIXELFORMAT_UNKNOWN,

    P_IMAGE_PIXELFORMAT_FIRST = P_IMAGE_PIXELFORMAT_RGB888,
    P_IMAGE_PIXELFORMAT_LAST = P_IMAGE_PIXELFORMAT_UNKNOWN,
    P_IMAGE_PIXELFORMAT_NUMBER = P_IMAGE_PIXELFORMAT_LAST - P_IMAGE_PIXELFORMAT_LAST + 1,
};

class P_DLLEXPORT PImage
{
public:
    PImage();
    PImage(const pchar *path, PInputStream &inputStream);
    PImage(const puint8 *data, puint32 width, puint32 height, 
        PImagePixelFormatEnum pixelFormat);
    PImage(const PImage& other);
    ~PImage();
    
    const PImage& operator=(const PImage& other);

    P_INLINE puint8* data() const                      { return m_data; }
    P_INLINE puint32 width() const                     { return m_width; }
    P_INLINE puint32 height() const                    { return m_height; }
    P_INLINE PImagePixelFormatEnum pixelFormat() const { return m_pixelFormat; }
    P_INLINE puint32 bpp() const                       { return bpp(m_pixelFormat); }

    static puint32 bpp(PImagePixelFormatEnum pixelFormat);

    pbool save(const pchar *path, POutputStream &os);

private:
    puint8                *m_data;
    puint32                m_width;
    puint32                m_height;
    PImagePixelFormatEnum  m_pixelFormat;
};

#endif // !PIMAGE_H
