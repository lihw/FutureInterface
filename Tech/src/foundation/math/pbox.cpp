// pbox.cpp
// The axis parallel box
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pbox.h"

#include <PFoundation/passert.h>
#include <PFoundation/pmatrix4x4.h>


PBox::PBox()
{
    m_min[0] = m_min[1] = m_min[2] = 0.0f;
    m_max[0] = m_max[1] = m_max[2] = 0.0f;
}

PBox::PBox(const pfloat32 *min, const pfloat32 *max)
{
    PASSERT(min[0] <= max[0] && 
            min[1] <= max[1] && 
            min[2] <= max[2]);
    m_min[0] = min[0];
    m_min[1] = min[1];
    m_min[2] = min[2];

    m_max[0] = max[0];
    m_max[1] = max[1];
    m_max[2] = max[2];
}

PBox::~PBox()
{
}

PBox::PBox(const PBox &other)
{
    m_min[0] = other.m_min[0];
    m_min[1] = other.m_min[1];
    m_min[2] = other.m_min[2];

    m_max[0] = other.m_max[0];
    m_max[1] = other.m_max[1];
    m_max[2] = other.m_max[2];
}

PBox PBox::operator+(const PBox &other) const
{
    pfloat32 newMin[3];
    pfloat32 newMax[3];

    newMin[0] = pMin(m_min[0], other.m_min[0]);
    newMin[1] = pMin(m_min[1], other.m_min[1]);
    newMin[2] = pMin(m_min[2], other.m_min[2]);
    
    newMax[0] = pMax(m_max[0], other.m_max[0]);
    newMax[1] = pMax(m_max[1], other.m_max[1]);
    newMax[2] = pMax(m_max[2], other.m_max[2]);

    return PBox(newMin, newMax);
}

const PBox &PBox::operator+=(const PBox &other) 
{
    if (m_min[0] > other.m_min[0]) m_min[0] = other.m_min[0];
    if (m_min[1] > other.m_min[1]) m_min[1] = other.m_min[1];
    if (m_min[2] > other.m_min[2]) m_min[2] = other.m_min[2];
    
    if (m_max[0] < other.m_max[0]) m_max[0] = other.m_max[0];
    if (m_max[1] < other.m_max[1]) m_max[1] = other.m_max[1];
    if (m_max[2] < other.m_max[2]) m_max[2] = other.m_max[2];

    return *this;
}

PBox PBox::operator^(const PBox &other) const
{
    pfloat32 newMin[3];
    pfloat32 newMax[3];

    newMin[0] = pMin(pMax(m_min[0], other.m_min[0]), m_max[0]);
    newMin[1] = pMin(pMax(m_min[1], other.m_min[1]), m_max[1]);
    newMin[2] = pMin(pMax(m_min[2], other.m_min[2]), m_max[2]);
    
    newMax[0] = pMax(pMax(m_max[0], other.m_max[0]), m_max[0]);
    newMax[1] = pMax(pMax(m_max[1], other.m_max[1]), m_max[1]);
    newMax[2] = pMax(pMax(m_max[2], other.m_max[2]), m_max[2]);
    
    return PBox(newMin, newMax);
}

const PBox &PBox::operator^=(const PBox &other) 
{
    m_min[0] = pMin(pMax(m_min[0], other.m_min[0]), m_max[0]);
    m_min[1] = pMin(pMax(m_min[1], other.m_min[1]), m_max[1]);
    m_min[2] = pMin(pMax(m_min[2], other.m_min[2]), m_max[2]);
    
    m_max[0] = pMax(pMax(m_max[0], other.m_max[0]), m_max[0]);
    m_max[1] = pMax(pMax(m_max[1], other.m_max[1]), m_max[1]);
    m_max[2] = pMax(pMax(m_max[2], other.m_max[2]), m_max[2]);

    return *this;
}

void PBox::transform(const PMatrix4x4 &transform)
{
    pfloat32 tmp[3];

    pMatrix4x4MultiplyVector3(transform.m_m, m_min, tmp);
    m_min[0] = tmp[0];
    m_min[1] = tmp[1];
    m_min[2] = tmp[2];

    pMatrix4x4MultiplyVector3(transform.m_m, m_max, tmp);
    m_max[0] = tmp[0];
    m_max[1] = tmp[1];
    m_max[2] = tmp[2];
}
