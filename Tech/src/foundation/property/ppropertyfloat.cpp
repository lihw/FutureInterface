// ppropertyfloat.cpp
// 32-bit floating number property.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "ppropertyfloat.h"

#include <PFoundation/pvariant.h>
#include <PFoundation/pconststring.h>
#include <PFoundation/pmathutility.h>

PPropertyFloat::PPropertyFloat(PAbstractProperty *parent)
    : PBasicProperty(P_PROPERTY_FLOAT, parent) 
{
    m_range[0] = P_MIN_FLOAT32;
    m_range[1] = P_MAX_FLOAT32;
    m_value = 0;
}
    
PPropertyFloat::~PPropertyFloat()
{
}

void PPropertyFloat::operator=(const PPropertyFloat &other)
{
    if (this != &other)
    {
        m_value = other.m_value;
        m_range[0] = other.m_range[0];
        m_range[1] = other.m_range[1];
    }
}

void PPropertyFloat::interpolate(pfloat32 t, PAbstractProperty *v1, PAbstractProperty *v2)
{
    PASSERT(t >= 0 && t <= 1);

    PASSERT(v1->type() == P_PROPERTY_FLOAT);
    PASSERT(v2->type() == P_PROPERTY_FLOAT);

    PPropertyFloat *v11 = (PPropertyFloat*)v1;
    PPropertyFloat *v22 = (PPropertyFloat*)v2;

    operator=(v11->toFloat() * (1.0f - t) + v22->toFloat() * t);
}

void PPropertyFloat::setValue(const PAbstractProperty &property)
{
    if (this != &property)
    {
        PASSERT(property.type() == P_PROPERTY_FLOAT);
        const PPropertyFloat *p = (const PPropertyFloat*)(&property);

        operator=(p->toFloat());
    }
}
    
void PPropertyFloat::setValue(const void *value)
{
    PASSERT(value != P_NULL);
    operator=(*(const pfloat32*)value);
}
    
void PPropertyFloat::operator=(pfloat32 value)
{
    m_value = pMin(m_range[1], pMax(m_range[0], value));
}

void PPropertyFloat::setRange(const pfloat32 *range)
{
    PASSERT(range != P_NULL);
    PASSERT(range[0] < range[1]);

    m_range[0] = range[0];
    m_range[1] = range[1];

    operator=(m_value);
}

void PPropertyFloat::setRange(pfloat32 lower, pfloat32 upper)
{
    PASSERT(lower < upper);
    m_range[0] = lower;
    m_range[1] = upper;

    operator=(m_value);
}

void PPropertyFloat::setValue(const PVariant &value)
{
    PASSERT(value.getType() == P_VARIANT_FLOAT);
    if (value.getType() == P_VARIANT_FLOAT)
    {
        operator=(value.toFloat());
    }
}
    
pbool PPropertyFloat::unpack(const pchar *value)
{
    pfloat32 v;
    if (pStringUnpackFloat(value, &v) != P_NULL)
    {
        operator=(v);
        return true;
    }
    
    PLOG_ERROR("Failed to unpack a float property called %s", name());
    return false;
}
