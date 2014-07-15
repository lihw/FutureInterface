// pmathutility.h 
// Some useful math functions
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PMATHUTILITY_H
#define PMATHUTILITY_H

#include <PFoundation/pglobal.h>

#ifndef P_PI
# define P_PI 3.1415926535897f
#endif

#ifndef P_PIOVER180
# define P_PIOVER180 0.0174532925199f //  PI / 180
#endif

#if !defined P_MAX_FLOAT32 || !defined P_MIN_FLOAT32
# define P_MAX_FLOAT32 3.4e38f
# define P_MIN_FLOAT32 -3.4e38f
#endif

#if !defined P_MAX_INT32 || !defined P_MIN_INT32
# define P_MIN_INT32 (-2147483647 - 1)
# define P_MAX_INT32 2147483647
#endif

#ifndef P_FLOAT_EPSILON
# define P_FLOAT_EPSILON 1.4e-45f
#endif

pfloat32 P_APIENTRY pDegreesToRadians(pfloat32 degree);

#endif // !PMATHUTILITY_H
