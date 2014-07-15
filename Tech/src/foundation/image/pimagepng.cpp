// pimagepng.cpp
// Read and write PNG file using libpng.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pimagepng_private.h"

extern "C" {
#include <png.h>
}

#include <PFoundation/pinputstream.h>
#include <PFoundation/poutputstream.h>

#include <PFoundation/passert.h>
#include <PFoundation/plog.h>
#include <PFoundation/pnew.h>

static void pImagePNGReadFunction(png_structp png_ptr, png_bytep data, png_size_t length)
{
    PInputStream* inputStream = (PInputStream*)png_get_io_ptr(png_ptr);

    pbool ret = inputStream->readBytes(length, (puint8*)data);
    PASSERT(ret);
}

static void pImagePNGWriteFunction(png_structp png_ptr, png_bytep data, png_size_t length)
{
    POutputStream* outputStream = (POutputStream*)png_get_io_ptr(png_ptr);

    pbool ret = outputStream->writeBytes(length, (puint8*)data);
    PASSERT(ret);
}

static void pImagePNGFlushFunction(png_structp png_ptr)
{
    // This function has been intentionally left empty. 
}

static png_voidp pImagePNGMallocFunction(png_structp png_ptr, png_size_t size)
{
    png_voidp allocatedBlock = PNEWARRAY(puint8, size);
    PASSERT(allocatedBlock != P_NULL);

    return allocatedBlock;
}

static void pImagePNGFreeFunction(png_structp png_ptr, png_voidp data)
{
    puint8* p = (puint8*)data;
    PDELETEARRAY(p);
}

pbool P_APIENTRY pImagePNGRead(PInputStream& inputStream, puint32& width, puint32& height, 
    PImagePixelFormatEnum& format, puint8*& data)
{
    puint8** rowPointers = P_NULL;

    // LibPNG specific structures. 
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_infop end_ptr = NULL; 
    png_byte pbSig[8];
    png_uint_32 png_result;

    if (!inputStream.readBytes(8, (puint8*)pbSig))
    {
        return false;
    }

    // Check that we're reading valid PNG file. 
    if (png_sig_cmp(pbSig, 0, 8))
    {
        PLOG_ERROR("not a valid PNG image (incorrect signature)");
        return false;
    }

    // Our png_ptr is used by the libpng library to maintain, basically, state
    // information about the PNG file as it is being read in by the library.
    // It's used for housekeeping by the libpng library.  The other two
    // pointers, info_ptr and end_ptr, are used to help us extract data from
    // the PNG file. 
    png_ptr = png_create_read_struct_2(
            PNG_LIBPNG_VER_STRING, 
            NULL, // error function pointer 
            NULL, // error printing function
            NULL, // warning function
            NULL, // memory pointer
            pImagePNGMallocFunction, // memory allocation function pointer 
            pImagePNGFreeFunction); // memory deallocation function pointer
    if (png_ptr == P_NULL)
    {
        PLOG_ERROR("can't create PNG reading structure");
        return false;
    }
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
        PDELETEARRAY(data);
        PDELETEARRAY(rowPointers);
        return false;
    }

    // Create png info structure.
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        png_destroy_read_struct(&png_ptr, (png_infopp)P_NULL, (png_infopp)P_NULL);
        PLOG_ERROR("can't create PNG info structure");
        return false;
    }

    end_ptr = png_create_info_struct(png_ptr);
    if (end_ptr == NULL) 
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        PLOG_ERROR("can't create PNG info structure");
        return false;
    }

    png_set_read_fn(png_ptr, (png_voidp)&inputStream, pImagePNGReadFunction);
    png_set_sig_bytes(png_ptr, 8);

    // Read png info. 
    png_read_info(png_ptr, info_ptr);

    // Read image info (width, height, color format) and data. 
    pint32 bitDepth;
    pint32 colorType;
    puint32 channelCount;

    // Fetch image width, height, bit depth, color type. 
    png_result = png_get_IHDR(png_ptr, info_ptr, (png_uint_32*)&width, 
            (png_uint_32*)&height, &bitDepth, &colorType, NULL, NULL, NULL);
    if (png_result == 0)
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
        PLOG_ERROR("PNG image has invalid header");
        return false;
    }

    // See how many channels this png contains. 
    channelCount = (puint32)png_get_channels(png_ptr, info_ptr);

    // Non-paletted image. 
    if (colorType != PNG_COLOR_TYPE_PALETTE)
    {
        // Support only for 8 bit depth pixels. 
        if (bitDepth == 8)
        {
            switch (channelCount)
            {
                case 1: format = P_IMAGE_PIXELFORMAT_R8; break;
                case 2: format = P_IMAGE_PIXELFORMAT_RA88; break;
                case 3: format = P_IMAGE_PIXELFORMAT_RGB888; break;
                case 4: format = P_IMAGE_PIXELFORMAT_RGBA8888; break;
                default: 
                         png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
                         PLOG_ERROR("Unsupported PNG pixel format");
                         return false;
            }
        }
        else
        {
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
            PLOG_ERROR("Unsupported PNG pixel format");
            return false;
        }
    }
    else
    {
        // Paletted image. 
        format = P_IMAGE_PIXELFORMAT_RGB888;
    }

    if (bitDepth == 16)
    {
        png_set_strip_16(png_ptr);
        bitDepth = 8;
    }

    puint32 rowSize = width * channelCount * (puint32)bitDepth / 8;

    // Create row pointers for libpng and unpack the data to new buffers, finally assigning to image. 
    data = PNEWARRAY(puint8, rowSize * height);
    PASSERT(data != P_NULL);
    if (data == P_NULL)
    {
        PLOG_ERROR("new puint8 [%d] failed", rowSize * height);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
        return false;
    }

    // Assign pointers for each row. 
    rowPointers = PNEWARRAY(puint8*, height);
    PASSERT(rowPointers != P_NULL);
    if (rowPointers == P_NULL)
    {
        PDELETEARRAY(data);
        PLOG_ERROR("rowPointers = new puint8* [%d] failed", height);
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
        return false;
    }

    puint8* datap = data;
    for (puint32 i = 0; i < height; ++i)
    {
        rowPointers[i] = datap;

        datap += rowSize;
    }

    // Read image data using libpng.
    png_read_image(png_ptr, (png_bytepp)rowPointers);
    png_read_end(png_ptr, NULL);

    PDELETEARRAY(rowPointers);

    /* Convert to RGB data in case of paletted png. */
    if (colorType == PNG_COLOR_TYPE_PALETTE)
    {
        png_colorp palette;
        pint32 numPalette;

        png_result = png_get_PLTE(png_ptr, info_ptr, &palette, &numPalette);
        if(png_result == 0)
        {
            PDELETEARRAY(data);
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
            PLOG_ERROR("invalid PNG palette");
            return false;
        }

        puint8* convertedImage = PNEWARRAY(puint8, width * height * 3);
        PASSERT(convertedImage != P_NULL);
        if (convertedImage == P_NULL)
        {
            PDELETEARRAY(data);
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
            PLOG_ERROR("convertedImage = new puint8 [%d] failed", height * width * 3);
            return false;
        }

        // 4-bit palette, assign 2 pixels at a time. 
        if (bitDepth == 4)
        {
            puint32 offset = 0;
            puint32 imageSize = width * channelCount * (puint32)bitDepth * height / 8;

            // If both width and height are odd, no support for loading. 
            if ((width & 1) != 0 && (height & 1) != 0)
            {
                PDELETEARRAY(data);
                PDELETEARRAY(convertedImage);
                png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
                PLOG_ERROR("4bit paletted png image with width and height having odd values not supported");
                return false;
            }

            for (puint32 i = 0; i < imageSize; ++i)
            {
                puint32 highByte = (puint32)(data[i] >> 4);
                puint32 lowByte = (puint32)(data[i] & 0x0F);
                convertedImage[offset++] = (puint8)palette[highByte].red;
                convertedImage[offset++] = (puint8)palette[highByte].green;
                convertedImage[offset++] = (puint8)palette[highByte].blue;
                convertedImage[offset++] = (puint8)palette[lowByte].red;
                convertedImage[offset++] = (puint8)palette[lowByte].green;
                convertedImage[offset++] = (puint8)palette[lowByte].blue;
            }
        }
        // 8 bit palette, assign colors by fetching from palette. 
        else if (bitDepth == 8)
        {
            puint32 offset = 0;
            puint32 imageSize = rowSize * height;
            for (puint32 i = 0; i < imageSize; i++)
            {
                puint8 colorValue = data[i];
                convertedImage[offset++] = (puint8)palette[colorValue].red;
                convertedImage[offset++] = (puint8)palette[colorValue].green;
                convertedImage[offset++] = (puint8)palette[colorValue].blue;
            }
        }
        else
        {
            PDELETEARRAY(data);
            PDELETEARRAY(convertedImage);
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);
            PLOG_ERROR("Invalid paletted image format");
            return false;
        }

        PDELETEARRAY(data);
        data = convertedImage;
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, &end_ptr);

    return true;
}

pbool P_APIENTRY pImagePNGWrite(POutputStream& outputStream, puint32 width, puint32 height, 
    PImagePixelFormatEnum format, const puint8* data)
{
    PASSERT(format == P_IMAGE_PIXELFORMAT_RGB888 ||
             format == P_IMAGE_PIXELFORMAT_RGBA8888);
    if (format != P_IMAGE_PIXELFORMAT_RGBA8888 &&
        format != P_IMAGE_PIXELFORMAT_RGB888)
    {
        PLOG_ERROR("unsupported pixel format by PNG");
        return false;
    }

    /* Create the write struct for png. */
    png_structp png_ptr = png_create_write_struct_2(
            PNG_LIBPNG_VER_STRING, 
            NULL, 
            NULL, 
            NULL, 
            NULL, 
            pImagePNGMallocFunction, 
            pImagePNGFreeFunction);
    if (png_ptr == NULL)
    {
        PLOG_ERROR("error in writing PNG image");
        return false;
    }

    // Create the info struct for png. Info struct contains info such as width,
    // height, bit depth and other attributes of image. 
    png_infop info_ptr = png_create_info_struct (png_ptr);
    if(info_ptr == NULL)
    {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        PLOG_ERROR("Error in writing PNG image");
        return false;
    }

    png_set_write_fn(png_ptr, (png_voidp)&outputStream, pImagePNGWriteFunction, pImagePNGFlushFunction);

    // Set png to non-interlaced, non-compressed RGB mode. 
    png_set_IHDR(png_ptr, info_ptr, (png_uint_32)width, (png_uint_32)height, 8,
                 format == P_IMAGE_PIXELFORMAT_RGB888 ? PNG_COLOR_TYPE_RGB : PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    // Write the image data. 
    puint32 rowSize = width * PImage::bpp(format);

    puint8** rowPointers = PNEWARRAY(puint8*, height);
    PASSERT(rowPointers != P_NULL);
    if (rowPointers == P_NULL)
    {
        PLOG_ERROR("rowPointers = new puint8* [%d] failed", height);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return false;
    }

    puint8* datap = const_cast<puint8*>(data);

    for (puint32 i = 0; i < height; ++i)
    {
        rowPointers[i] = datap;
        datap += rowSize;
    }

    png_write_image(png_ptr, (png_bytepp)rowPointers);
    png_write_end(png_ptr, NULL);

    png_destroy_write_struct(&png_ptr, &info_ptr);

    PDELETEARRAY(rowPointers);

    return true;
}

