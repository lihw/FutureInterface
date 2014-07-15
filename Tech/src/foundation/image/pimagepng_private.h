// pimagepng_private.h
// Read and write PNG file using libpng.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PIMAGEPNG_PRIVATE_H
#define PIMAGEPNG_PRIVATE_H

#include <PFoundation/pimage.h>

class PInputStream;
class POutputStream;

P_EXTERN pbool P_APIENTRY pImagePNGRead(PInputStream& inputStream, puint32& width, puint32& height, 
    PImagePixelFormatEnum& format, puint8*& data);

P_EXTERN pbool P_APIENTRY pImagePNGWrite(POutputStream& outputStream, puint32 width, puint32 height, 
    PImagePixelFormatEnum format, const puint8* data);


#endif // !PIMAGEPNG_PRIVATE_H
