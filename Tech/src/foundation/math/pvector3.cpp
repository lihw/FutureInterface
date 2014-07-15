// pvector3.cpp
// vector 3 functions.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pvector3.h"

#include <math.h>

pfloat32 P_APIENTRY pVector3Length(const pfloat32 *in)
{
    return sqrtf(pVector3Dot(in, in));
}

#if defined P_DEBUG
void P_APIENTRY pVector3Print(const pfloat32 *vector3)
{
    PLOG_INFO("%6.3f %6.3f %6.3f", vector3[0], vector3[1], vector3[2]);
}
#endif

