// pmathutility.cpp 
// Some useful math function
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pmathutility.h"

pfloat32 P_APIENTRY pDegreesToRadians(pfloat32 degrees)
{
    return degrees * P_PIOVER180;
}
