// p_global_functions.h
// Some frequently used functions.
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PT_GLOBAL_FUNCTIONS_H
#define PT_GLOBAL_FUNCTIONS_H

#include "pplatform.h"
#include "ptypes.h"

#define pAbs(a)           ((a)<0 ? -(a) : (a))

template<typename T>
const T& pMin(const T& a, const T& b)
{
    return a < b? a : b;
}

template<typename T>
const T& pMax(const T& a, const T& b)
{
    return a > b? a : b;
}

template<typename T>
void pSwap(T& a, T& b)
{
    T c(a);
    a = b;
    b = c;
}

P_INLINE pfloat32 pSign(pfloat32 v)
{
    return v < 0? -1.0f : 1.0f;
}

P_INLINE pint32 pSign(pint32 v)
{
    return v < 0? -1 : 1;
}

template<typename T>
T pClamp(const T& a, const T& lower, const T& upper)
{
    return pMin(pMax(a, lower), upper);
}

#endif // !PT_GLOBAL_FUNCTIONS_H
