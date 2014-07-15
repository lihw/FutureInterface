// prandom.h
// 
// Random number.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PRANDOM_H
#define PRANDOM_H

#include <PFoundation/pglobal.h>

class P_DLLEXPORT PRandom
{
public:
    // Constructor.
    // \param seed the random seed.
    PRandom(puint32 seed);

    // Constructor using default random seed.
    PRandom();

    // Set the seed.
    // \param seed the seed number.
    void setSeed(puint32 seed);

    // Return a random puint32 integer.
    // \return the random number.
    puint32 getUint32();

    // Return a random puint32 in the range [lower, upper)
    // \param lower the lower bound.
    // \param upper the upper bound.
    // \return the random number inside the given range.
    puint32 getUint32(puint32 lower, puint32 upper);

    // Return a [0, 1] flating random value.
    pfloat32 getFloat32();

    // Static version of random number generator.
    static puint32 randomUint32(); 
    static puint32 randomUint32(puint32 lower, puint32 upper);
    static pfloat32 randomFloat32();

private:
    // Park-Miller "minimal standard" 31 bit pseudo-random number generator,
    // implemented with David G. Carta's optimization: with 32 bit math and
    // without division.
    // \param seed the random seed.
    // \return a random value between 0 and 0x7fffffff.
    static puint32 rand31(puint32 seed);

private:
    /// The current random value.
    puint32 m_value;

    static puint32 s_value;
};

#endif // !PRANDOM_H
