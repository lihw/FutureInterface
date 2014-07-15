// prandom.cpp
// 
// Random number.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "prandom.h"

puint32 PRandom::s_value = 10001;

PRandom::PRandom(puint32 seed)
{
    m_value = seed;
}

PRandom::PRandom()
{
    m_value = 0xdeadcafe;
}

void PRandom::setSeed(puint32 seed)
{
    m_value = seed;
}

puint32 PRandom::getUint32()
{
    m_value = rand31(m_value);

    return m_value;
}
    
puint32 PRandom::getUint32(puint32 lower, puint32 upper)
{
    puint32 ret = getUint32();

    return ret % (upper - lower) + lower;
}

pfloat32 PRandom::getFloat32()
{
    return (pfloat32)getUint32() / 0x7fffffff;
}

puint32 PRandom::rand31(puint32 seed)
{
    puint32 hi, lo;

    lo = 16807 * (seed & 0xFFFF);
    hi = 16807 * (seed >> 16);

    lo += (hi & 0x7FFF) << 16;
    lo += hi >> 15;

    if (lo > 0x7FFFFFFF)
    {
        lo -= 0x7FFFFFFF;
    }

    return lo;
}


puint32 PRandom::randomUint32()
{
    s_value = rand31(s_value);

    return s_value;
}

puint32 PRandom::randomUint32(puint32 lower, puint32 upper)
{
    puint32 ret = randomUint32();

    return ret % (upper - lower) + lower;
}

pfloat32 PRandom::randomFloat32()
{
    return (pfloat32)randomUint32() / 0x7fffffff;
}
