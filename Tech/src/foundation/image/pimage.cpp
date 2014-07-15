// pimage.cpp
// The image object.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pimage.h"

#include "pimagepng_private.h"
#include "pimagetga_private.h"

#include <PFoundation/pinputstream.h>
#include <PFoundation/poutputstream.h>

#include <PFoundation/pcrt.h>
#include <PFoundation/ptime.h>


PImage::PImage()
{
    m_width = 0;
    m_height = 0;
    m_data = P_NULL;
    m_pixelFormat = P_IMAGE_PIXELFORMAT_UNKNOWN;
}
    
PImage::PImage(const pchar *path, PInputStream &inputStream)
{
    m_data = P_NULL;
    m_width = 0;
    m_height = 0;
    m_pixelFormat = P_IMAGE_PIXELFORMAT_UNKNOWN;

    // Find the type of image by checking the prefix of the path.
    const pchar* suffix = pstrrchr(path, '.');
    if (suffix == P_NULL)
    {
        PLOG_ERROR("Fail to recognize image format");
    } 
    else
    {
        suffix += 1;

#if P_ENABLE_IMAGE_PNG == 1
        if (pstrcmp(suffix, "png") == 0)
        {
            if (!pImagePNGRead(inputStream, m_width, m_height, m_pixelFormat, m_data))
            {
                PLOG_ERROR("Failed to read PNG image %s", path);

                m_width = 0;
                m_height = 0;
                m_pixelFormat = P_IMAGE_PIXELFORMAT_UNKNOWN;
                m_data = P_NULL;
            }
        }
#endif // P_ENABLE_IMAGE_PNG == 1

#if P_ENABLE_IMAGE_TGA == 1
        if (pstrcmp(suffix, "tga") == 0)
        {
            if (!pImageTGARead(inputStream, m_width, m_height, m_pixelFormat, m_data))
            {
                PLOG_ERROR("Failed to read TGA image %s", path);

                m_width = 0;
                m_height = 0;
                m_pixelFormat = P_IMAGE_PIXELFORMAT_UNKNOWN;
                m_data = P_NULL;
            }
        }
#endif // P_ENABLE_IMAGE_TGA == 1

        // TODO: other image formats

        if (m_data != P_NULL)
        {
            PLOG_DEBUG("Load image %s succeeded", path);
        }
        else
        {
            PLOG_DEBUG("Unsupported image format");
            PLOG_DEBUG("Only supports");
#if P_ENABLE_IMAGE_PNG == 1
            PLOG_DEBUG("png");
#endif
#if P_ENABLE_IMAGE_TGA == 1
            PLOG_DEBUG("tga");
#endif
        }
    }
}


PImage::PImage(const puint8 *data, puint32 width, puint32 height, 
    PImagePixelFormatEnum pixelFormat)
{
    puint32 b = bpp(pixelFormat);
    puint32 size = width * height * b;
    m_data = PNEWARRAY(puint8, size);
    if (data != P_NULL)
    {
        pmemcpy(m_data, data, size);
    }

    m_width = width;
    m_height = height;
    m_pixelFormat = pixelFormat;
}

PImage::PImage(const PImage& other)
{
    m_width = other.m_width;
    m_height = other.m_height;
    m_pixelFormat = other.m_pixelFormat;
    m_data = P_NULL;

    if (other.m_data != P_NULL)
    {
        puint32 b = bpp(m_pixelFormat);
        puint32 size = m_width * m_height * b;
        m_data = PNEWARRAY(puint8, size);
        pmemcpy(m_data, other.m_data, size);
    }
}

PImage::~PImage()
{
    PDELETEARRAY(m_data);
}
    
const PImage& PImage::operator=(const PImage& other)
{
    if (this != &other)
    {
        m_width = other.m_width;
        m_height = other.m_height;
        m_pixelFormat = other.m_pixelFormat;
        m_data = P_NULL;

        if (other.m_data != P_NULL)
        {
            puint32 b = bpp(m_pixelFormat);
            puint32 size = m_width * m_height * b;
            m_data = PNEWARRAY(puint8, size);
            pmemcpy(m_data, other.m_data, size);
        }
    }

    return *this;
}

puint32 PImage::bpp(PImagePixelFormatEnum pixelFormat)
{
    switch (pixelFormat)
    {
        case P_IMAGE_PIXELFORMAT_RGBA8888: return 4;
        case P_IMAGE_PIXELFORMAT_RGB888: return 3;
        case P_IMAGE_PIXELFORMAT_R8: return 1;
        case P_IMAGE_PIXELFORMAT_LUMINANCE: return 1;
        case P_IMAGE_PIXELFORMAT_COLOR_INDEX: return 1;
        case P_IMAGE_PIXELFORMAT_UNKNOWN: return 0;
        default:
            PLOG_ERROR("Unknown image pixel format");
            return 0;
    }

    return 0;
}

pbool PImage::save(const pchar *path, POutputStream &os)
{
    // Find the type of image by checking the prefix of the path.
    const pchar* suffix = pstrrchr(path, '.');
    if (pstrcmp(suffix, ".png") == 0)
    {
        return pImagePNGWrite(os, 
                this->width(), this->height(), 
                this->pixelFormat(), this->data());
    }
    else if (pstrcmp(suffix, ".tga") == 0)
    {
        return pImageTGAWrite(os, 
                this->width(), this->height(), 
                this->pixelFormat(), this->data());
    }
    return false;
}
