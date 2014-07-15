// pimagetga.cpp
// Read tga file.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//
#include "pimagetga_private.h"

#include <PFoundation/pinputstream.h>
#include <PFoundation/poutputstream.h>
#include <PFoundation/pimage.h>

#include <PFoundation/passert.h>
#include <PFoundation/plog.h>
#include <PFoundation/pnew.h>

static pbool pImageTGAVerbose = false;

// fixed header values for the subset of TGA we use for writing.
const puint8 TGAHeaderColor[12] = 
{ 
    0,// 0 ID length = no id
    0,// 1 color map type = no color map
    2,// 2 image type = uncompressed true color
    0, 0, 0, 0, 0,// color map spec = empty
    0, 0,  // x origin of image 
    0, 0   // y origin of image
};

const puint8 TGAHeaderBW[12] = 
{ 
    0,// 0 ID length = no id
    0,// 1 color map type = no color map
    3,// 3 image type = uncompressed black and white
    0, 0, 0, 0, 0,// color map spec = empty
    0, 0,  // x origin of image 
    0, 0   // y origin of image
};

const puint8 TGAFooter[TGA_SIGNATURE_SIZE] = 
{ 
    0, 0, 0, 0, // extensionAreaOffset
    0, 0, 0, 0, // extensionAreaOffset
    'T', 'R', 'U', 'E', 'V', 'I', 'S', 'I', 'O', 'N', '-', 'X', 'F', 'I', 'L', 'E', // signature
    0, // dot
    0, // null
};

void pImageTGALog(const pchar *format, ...)
{
    if (pImageTGAVerbose)
    {
        va_list arguments;
        va_start(arguments, format);
        
        pchar message[3000]; 
        puint32 nchars;
        nchars = pvsprintf(message, 3000, format, arguments);
        PLOG_INFO("%s", message);
    }
}

pbool pImageStdRead(RLEstate *rleInfo, puint8 *buffer, puint32 datasize, puint32 nelems, PInputStream &inputStream)
{
    return inputStream.readBytes(datasize * nelems, buffer);
}

pbool pImageRleRead(RLEstate *rleInfo, puint8 *buffer, puint32 datasize, puint32 nelems, PInputStream &inputStream)
{
    puint8 *buf = buffer;
    pint32 j, k;
    pint32 buflen, bytes;
    puint8 count;
    puint8 *p;

    // Scale the buffer length.
    buflen = nelems * datasize;

    j = 0;
    while (j < buflen) 
    {
        if (rleInfo->laststate < rleInfo->statelen) 
        {
            // Copy bytes from our previously decoded buffer.
            bytes = pMin(buflen - j, rleInfo->statelen - rleInfo->laststate);
            pmemcpy(buf + j, rleInfo->statebuf + rleInfo->laststate, bytes);
            j += bytes;
            rleInfo->laststate += bytes;

            // If we used up all of our state bytes, then reset them.
            if (rleInfo->laststate >= rleInfo->statelen)
            {
                rleInfo->laststate = 0;
                rleInfo->statelen = 0;
            }

            // If we filled the buffer, then exit the loop.
            if (j >= buflen) break;
        }

        // Decode the next packet.
        inputStream.readUint8(&count);
        if (inputStream.isEndOfStream())
        {
            PLOG_ERROR("TGA: hit EOF while looking for count\n");
            return false;
        }

        // Scale the byte length to the size of the data.
        bytes = ((count & ~RLE_PACKETSIZE) + 1) * datasize;

        if (j + bytes <= buflen)
        {
            // We can copy directly into the image buffer.
            p = buf + j;
        }
        else 
        {
            pImageTGALog("TGA: needed to use statebuf for %d bytes\n", buflen - j);
            // Allocate the state buffer if we haven't already.
            if (!rleInfo->statebuf)
            {
                rleInfo->statebuf = PNEWARRAY(puint8, RLE_PACKETSIZE * datasize);
            }
            p = rleInfo->statebuf;
        }

        if (count & RLE_PACKETSIZE) 
        {
            // Fill the buffer with the next value.
            if (!inputStream.readBytes(datasize, p))
            {
                PLOG_ERROR("TGA: EOF while reading %d/%d element RLE packet\n",
                bytes, datasize);
                return false;
            }

            // Optimized case for single-byte encoded data.
            if (datasize == 1)
            {
                pmemset(p + 1, *p, bytes - 1);
            }
            else
            {
                for (k = datasize; k < bytes; k += datasize)
                {
                    pmemcpy(p + k, p, datasize);
                }
            }
        } 
        else 
        {
            // Read in the buffer.
            if (!inputStream.readBytes(bytes, p))
            {
                PLOG_ERROR("TGA: EOF while reading %d/%d element raw packet\n",
                bytes, datasize);
                return false;
            }
        }

        // We may need to copy bytes from the state buffer.
        if (p == rleInfo->statebuf) 
        {
            rleInfo->statelen = bytes;
        }
        else
        {
            j += bytes;
        }
    }
    
    return true;
}


pbool P_APIENTRY pImageTGARead(PInputStream& inputStream, puint32& width, puint32& height, 
    PImagePixelFormatEnum& format, puint8*& data)
{
    TgaFooter tgaFooter;
    TgaHeader tgaHeader;
    puint32 bpp;
    
    // Check the footer.
    inputStream.seek(inputStream.getSize() - TGA_SIGNATURE_SIZE);
    if (!inputStream.readBytes(TGA_SIGNATURE_SIZE, (puint8 *)&tgaFooter))
    {
        PLOG_ERROR("TGA: Cannot read tga footer from inputstream.\n");
        return false;
    }  
    
    // Check the signature.
    if (pmemcmp(tgaFooter.signature, TGA_SIGNATURE, sizeof(tgaFooter.signature)) != 0) 
    {
        PLOG_ERROR("TGA: not a valid TGA image (incorrect signature).\n");
        return false;
    }

    // Read the header.
    inputStream.seek(0);
    if (!inputStream.readBytes(sizeof(tgaHeader), (puint8 *)&tgaHeader))
    {
        PLOG_ERROR("TGA: Cannot read tga header from inputstream.\n");
        return false;
    }

    // Image ID filed.
    if (pImageTGAVerbose && tgaHeader.idLength)
    {
        puint8 *idString = PNEWARRAY(puint8, tgaHeader.idLength);
        inputStream.readBytes(tgaHeader.idLength, idString);
        pImageTGALog("TGA: ID field: \"%*s\"\n", tgaHeader.idLength, idString);
    }
    else
    {
        // Skip the image ID field.
        if (tgaHeader.idLength)
        {
            inputStream.skipBytes(tgaHeader.idLength);
        }
    }

    // Reassemble the multi-byte values correctly, regardless of 
    // host endianness.
    width = (tgaHeader.widthHi << 8) | tgaHeader.widthLo;
    height = (tgaHeader.heightHi << 8) | tgaHeader.heightLo;
    bpp = tgaHeader.bpp;
    pImageTGALog("TGA: width = %d, height = %d, bpp = %d\n", width, height, bpp);

    // Get reversal direction.
    pbool horzrev;
    pbool vertrev;
    horzrev = ((tgaHeader.descriptor & TGA_DESC_HORIZONTAL) != 0);
    vertrev = ((!(tgaHeader.descriptor & TGA_DESC_VERTICAL)) != 0);
    if (horzrev) 
    {
        pImageTGALog("TGA: horizontal reversed\n");
    }
    if (vertrev) 
    {
        pImageTGALog("TGA: vertical reversed\n");
    }

    pbool rle = false;
    puint32 components = 0;
    switch (tgaHeader.imageType)
    {
        case TGA_TYPE_MAPPED_RLE:
            rle = true;
            pImageTGALog("TGA: run-length encoded\n");
        case TGA_TYPE_MAPPED:
            format = P_IMAGE_PIXELFORMAT_COLOR_INDEX;
            components = 1;
            pImageTGALog("TGA: %d bit indexed image (%d bit palette)\n", 
                tgaHeader.colorMapSize, bpp);
            break;

        case TGA_TYPE_GRAY_RLE:
            rle = true;
            pImageTGALog("TGA: run-length encoded\n");
        case TGA_TYPE_GRAY:
            format = P_IMAGE_PIXELFORMAT_LUMINANCE;
            components = 1;
            pImageTGALog("TGA: %d bit grayscale image\n", bpp);
            break;
            
        case TGA_TYPE_COLOR_RLE:
            rle = true;
            pImageTGALog("TGA: run-length encoded\n");
        case TGA_TYPE_COLOR:
            if (bpp == 32)
            {
                format = P_IMAGE_PIXELFORMAT_RGBA8888;
                components = 4;
                pImageTGALog("TGA: %d bit color image with alpha channel\n", bpp);
            }
            else
            {
                format = P_IMAGE_PIXELFORMAT_RGB888;
                components = 3;
                pImageTGALog("TGA: %d bit color image\n", bpp);
            }
			break;

        default:
            PLOG_ERROR("TGA: unrecognized image type %d\n", tgaHeader.imageType);
            return false;
            break;
    }

    // FIXME: We haven't implemented bit-packed fields yet.
    if ((format == P_IMAGE_PIXELFORMAT_RGBA8888 && bpp != 32) ||
        (format == P_IMAGE_PIXELFORMAT_RGB888 && bpp != 24) ||
        ((format == P_IMAGE_PIXELFORMAT_LUMINANCE || format == P_IMAGE_PIXELFORMAT_COLOR_INDEX) && bpp != 8))
    {
        PLOG_ERROR("TGA: channel sizes other than 8 bits are unimplemented");
        return false;
    }

    // Check that we have a color map only when we need it.
    if (format == P_IMAGE_PIXELFORMAT_COLOR_INDEX)
    {
        if (tgaHeader.colorMapType != 1)
        {
            PLOG_ERROR("TGA: indexed image has invalid color map type %d\n",
                tgaHeader.colorMapType);
            return false;
        }
    }
    else if (tgaHeader.colorMapType != 0)
    {
        PLOG_ERROR("TGA: non-indexed image has invalid color map type %d\n",
            tgaHeader.colorMapType);
        return false;
    }

    puint8 *cmap;
    puint32 colors;
    puint32 pelbytes;
    if (tgaHeader.colorMapType == 1)
    {
        // We need to read in the colormap.
        puint32 index = (tgaHeader.colorMapIndexHi << 8) | tgaHeader.colorMapIndexLo;
        puint32 length = (tgaHeader.colorMapLengthHi << 8) | tgaHeader.colorMapLengthLo;
        
        pImageTGALog("TGA: reading color map (%d + %d) * (%d / 8)\n",
            index, length, tgaHeader.colorMapSize);

        if (length == 0)
        {
            PLOG_ERROR("TGA: invalid color map length %d", length);
            return false;
        }
        
        if (tgaHeader.colorMapSize != 24) 
        {
            // We haven't implemented bit-packed fields yet.
            PLOG_ERROR("TGA: channel sizes other than 8 bits are unimplemented");
            return false;
        }
        
        pelbytes = tgaHeader.colorMapSize / 8;
        colors = length + index;
        cmap = PNEWARRAY(puint8, colors * pelbytes);
        
        // Zero the entries up to the beginning of the map.
        pmemset(cmap, 0, index * pelbytes);

        // Read in the rest of the colormap.
        if (!inputStream.readBytes(length * pelbytes, cmap))
        {
            PLOG_ERROR("TGA: error reading colormap (position == %ld)\n", 
                inputStream.getReadBytes());
            return false;
        }

        if (pelbytes >= 3) 
        {
            // Rearrange the colors from BGR to RGB.
            puint8 tmp;
            for (puint32 j = index; j < length * pelbytes; j += pelbytes) 
            {
                tmp = cmap[j];
                cmap[j] = cmap[j + 2];
                cmap[j + 2] = tmp;
            }
        }
    } 
    else 
    {
        colors = 0;
        cmap = P_NULL;
    }
    
    // Allocate the data.
    pelbytes = bpp / 8;
    data = PNEWARRAY(puint8, width * height * pelbytes);

    // Set rle info and read function.
    RLEstate rleRec;
    RLEstate *rleInfo;
    pbool (*myread)(RLEstate *rleInfo, puint8 *, puint32, puint32, PInputStream &);
    if (rle)
    {
        rleRec.statebuf = 0;
        rleRec.statelen = 0;
        rleRec.laststate = 0;
        rleInfo = &rleRec;
        myread = pImageRleRead;
    }
    else 
    {
        rleInfo = P_NULL;
        myread = pImageStdRead;
    }
    
    puint32 wbytes = width * pelbytes;
    pint32 start, end, dir;
    if (!vertrev) 
    {
        start = 0;
        end = height;
        dir = 1;
    }
    else
    {
        // We need to reverse loading order of rows.
        start = height - 1;
        end = -1;
        dir = -1;
    }

    puint8 *p;
    for (pint32 i = start; i != end; i += dir) 
    {
        p = data + i * wbytes;

        // Suck in the data one row at a time.
        if (!myread(rleInfo, p, pelbytes, width, inputStream))
        {
            // Probably premature end of file.
            pImageTGALog("TGA: error reading (position == %ld, width=%d)\n",
                inputStream.getReadBytes(), width);
            return false;
        }  

        if (pelbytes >= 3)
        {
            // Rearrange the colors from BGR to RBG.
            puint8 tmp;
            for (puint32 j = 0; j < width * pelbytes; j += pelbytes) 
            {
                tmp = p[j];
                p[j] = p[j + 2];
                p[j + 2] = tmp;
            }
        }

        if (horzrev) 
        {
            // We need to mirror row horizontally.
            for (puint32 j = 0; j < width/2; j++) 
            {
                puint8 tmp;
                for (puint32 k = 0; k < pelbytes; k++) 
                {
                    tmp = p[j * pelbytes + k];
                    p[j * pelbytes + k] = p[(width - j - 1) * pelbytes + k];
                    p[(width - j - 1) * pelbytes + k] = tmp;
                }
            }
        }
    }

    if (rle)
    {
        PDELETEARRAY(rleInfo->statebuf);
    }

    // FROMv01: not finish reading.
    // Read the footer.
    if (!inputStream.readBytes(TGA_SIGNATURE_SIZE, (puint8 *)&tgaFooter))
    {
        PLOG_ERROR("TGA: Cannot read tga footer from inputstream.\n");
    }

    if (!inputStream.isEndOfStream())
    {
        PLOG_ERROR("TGA: too much input data, ignoring extra...\n");
    }

    // FIXME: we can not return a color map.
    PASSERT(cmap == P_NULL);
    PDELETEARRAY(cmap);
    
    return true;
}

pbool P_APIENTRY pImageTGAWrite(POutputStream& outputStream, puint32 width, puint32 height, 
    PImagePixelFormatEnum format, const puint8* data)
{
    puint8 bpp = 0;
    
    switch (format)
    {
        case P_IMAGE_PIXELFORMAT_RGBA8888:
            outputStream.writeBytes(12, TGAHeaderColor);
            bpp = 4;
            break;
        case P_IMAGE_PIXELFORMAT_RGB888:
            outputStream.writeBytes(12, TGAHeaderColor);
            bpp = 3;
            break;
        case P_IMAGE_PIXELFORMAT_LUMINANCE:
            outputStream.writeBytes(12, TGAHeaderBW);
            bpp = 1;
            break;
        default:
            PLOG_ERROR("TGA: Supported component number: 1,3 or 4\n");
            return false;
            break;
    }

    outputStream.writeInt16(width);
    outputStream.writeInt16(height);
    outputStream.writeUint8(bpp * 8);

    if (format == P_IMAGE_PIXELFORMAT_RGBA8888) 
    {
        outputStream.writeUint8(0x24); // bottom left image (0x00) + 8 bit alpha (0x4)
    }
    else 
    {
        outputStream.writeUint8(0x00);
    }

    if (bpp <= 2)
    {
        outputStream.writeBytes(width * height * bpp, data);
    }
    else
    {
        puint8 *tmpData = PNEWARRAY(puint8, width * height * bpp);
        pmemcpy(tmpData, data, width * height * bpp);
        
        // Rearrange the colors from RGB to BGR.
        puint8 tmp;
        for (puint32 j = 0; j < width * height * bpp; j += bpp) 
        {
            tmp = tmpData[j];
            tmpData[j] = tmpData[j + 2];
            tmpData[j + 2] = tmp;
        }

        outputStream.writeBytes(width * height * bpp, tmpData);
        PDELETEARRAY(tmpData);
    }

    outputStream.writeBytes(TGA_SIGNATURE_SIZE, TGAFooter);
    
	return true;
}
