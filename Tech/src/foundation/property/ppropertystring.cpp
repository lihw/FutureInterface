// ppropertystring.cpp
// The string property.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#include "ppropertystring.h"

#include <PFoundation/pvariant.h>
#include <PFoundation/pconststring.h>


PPropertyString::PPropertyString(PAbstractProperty *parent)
    : PBasicProperty(P_PROPERTY_STRING, parent)
{
    reset();
}

PPropertyString::~PPropertyString()
{
}
    
void PPropertyString::operator=(const PPropertyString &other)
{
    if (this != &other)
    {
        m_value = other.m_value;
    }
}

void PPropertyString::interpolate(pfloat32 t, PAbstractProperty *v1, PAbstractProperty *v2)
{
    PASSERT(t >= 0 && t <= 1);

    PASSERT(v1->type() != P_PROPERTY_STRING);
    PASSERT(v2->type() != P_PROPERTY_STRING);

    const PPropertyString* v11 = (const PPropertyString*)v1;
    const PPropertyString* v22 = (const PPropertyString*)v2;

    if (t < 0.5)
    {
        operator=(v11->toString());
    }
    else
    {
        operator=(v22->toString());
    }
}
    
void PPropertyString::setValue(const PAbstractProperty &property)
{
    if (this != &property)
    {
        PASSERT(property.type() == P_PROPERTY_STRING);
        const PPropertyString *p = (const PPropertyString*)&property;
        m_value = p->toString();
    }
}

void PPropertyString::setValue(const void *value)
{
    PASSERT(value != P_NULL);
    m_value = (const pchar *)(value);
}

void PPropertyString::setValue(const PVariant &value)
{
    PASSERT(value.getType() == P_VARIANT_STRING);
    if (value.getType() == P_VARIANT_STRING)
    {
        m_value = value.toString();
    }
}

pbool PPropertyString::unpack(const pchar *value)
{
    m_value = pStringTrim(value);
    return true;
}
