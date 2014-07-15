// ppropertyint.cpp
// The 32-bit signed integer property.
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "ppropertyint.h"

#include <PFoundation/pvariant.h>
#include <PFoundation/pconststring.h>
#include <PFoundation/pmathutility.h>

PPropertyInt::PPropertyInt(PAbstractProperty *parent)
    : PBasicProperty(P_PROPERTY_INT, parent) 
{
    m_range[0] = P_MIN_INT32;
    m_range[1] = P_MAX_INT32;
    m_value = 0;
}
  
PPropertyInt::~PPropertyInt()
{
}

void PPropertyInt::operator=(const PPropertyInt &other)
{
    if (this != &other)
    {
        m_value = other.m_value;
        m_range[0] = other.m_range[0];
        m_range[1] = other.m_range[1];
    }
}
    
void PPropertyInt::interpolate(pfloat32 t, PAbstractProperty *v1, PAbstractProperty *v2)
{
    PASSERT(t >= 0 && t <= 1);

    PASSERT(v1->type() == P_PROPERTY_INT);
    PASSERT(v2->type() == P_PROPERTY_INT);
    const PPropertyInt *v11 = (const PPropertyInt*)v1;
    const PPropertyInt *v22 = (const PPropertyInt*)v2;

    operator=((pint32)((pint32)v11->toInt() * (1.0f - t) + (pint32)v22->toInt() * t));
}

void PPropertyInt::setValue(const PAbstractProperty &property)
{
    if (this != &property)
    {
        PASSERT(property.type() == P_PROPERTY_INT);
        const PPropertyInt *p = (const PPropertyInt*)&property;
        operator=(p->toInt());
    }
}

void PPropertyInt::setValue(const void *value)
{
    PASSERT(value != P_NULL);
    operator=(*(const pint32*)value);
}

void PPropertyInt::setRange(const pint32 *range)
{
    PASSERT(range != P_NULL);
    PASSERT(range[0] < range[1]);

    m_range[0] = range[0];
    m_range[1] = range[1];

    operator=(m_value);
}

void PPropertyInt::setRange(pint32 lower, pint32 upper)
{
    PASSERT(lower < upper);
    m_range[0] = lower;
    m_range[1] = upper;

    operator=(m_value);
}

void PPropertyInt::operator=(pint32 value)
{
    m_value = pMin(m_range[1], pMax(value, m_range[0]));
}
    
void PPropertyInt::setValue(const PVariant &value)
{
    PASSERT(value.getType() == P_VARIANT_INT);
    if (value.getType() == P_VARIANT_INT)
    {
        operator=(value.toInt());
    }
}
    
pbool PPropertyInt::unpack(const pchar *value)
{
    pint32 v;

    if ((pStringUnpackInt(value, &v)) != P_NULL) 
    {
        operator=(v);

        return true;
    }
        
    PLOG_ERROR("Failed to unpack an int property called %s", name());
    return false;
}
