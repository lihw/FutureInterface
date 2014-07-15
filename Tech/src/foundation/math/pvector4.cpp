// pvector4.cpp
// vector 4 functions.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pvector4.h"

#include <math.h>

pfloat32 P_APIENTRY pVector4Length(const pfloat32 *in)
{
    return sqrtf(pVector4Dot(in, in));
}

