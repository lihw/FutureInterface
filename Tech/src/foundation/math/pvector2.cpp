// pvector2.cpp
// vector 2 functions.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pvector2.h"

#include <math.h>

pfloat32 P_APIENTRY pVector2Length(const pfloat32 *in)
{
    return sqrtf(pVector2Dot(in, in));
}

