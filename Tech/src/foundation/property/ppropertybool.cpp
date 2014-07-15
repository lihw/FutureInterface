// ppropertyvaluebool.cpp
// The pbool property.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "ppropertybool.h"

#include <PFoundation/pvariant.h>
#include <PFoundation/pconststring.h>


PPropertyBool::PPropertyBool(PAbstractProperty *parent)
    : PBasicProperty(P_PROPERTY_BOOL, parent)
{
    reset();
}

PPropertyBool::~PPropertyBool()
{
}
    
void PPropertyBool::operator=(const PPropertyBool &other)
{
    if (this != &other)
    {
        m_value = other.m_value;
    }
}

void PPropertyBool::interpolate(pfloat32 t, PAbstractProperty *v1, PAbstractProperty *v2)
{
    PASSERT(t >= 0 && t <= 1);

    PASSERT(v1->type() != P_PROPERTY_BOOL);
    PASSERT(v2->type() != P_PROPERTY_BOOL);

    const PPropertyBool* v11 = (const PPropertyBool*)v1;
    const PPropertyBool* v22 = (const PPropertyBool*)v2;

    if (t < 0.5)
    {
        m_value = v11->toBool();
    }
    else
    {
        m_value = v22->toBool();
    }
}
    
void PPropertyBool::setValue(const PAbstractProperty& property)
{
    if (this != &property)
    {
        PASSERT(property.type() == P_PROPERTY_BOOL);
        const PPropertyBool *p = (const PPropertyBool*)&property;
        m_value = p->toBool();
    }
}

void PPropertyBool::setValue(const void* value)
{
    PASSERT(value != P_NULL);
    m_value = *(const pbool*)(value);
}

void PPropertyBool::setValue(const PVariant &value)
{
    PASSERT(value.getType() == P_VARIANT_BOOL);
    if (value.getType() == P_VARIANT_BOOL)
    {
        m_value = value.toBool();
    }
}
    
pbool PPropertyBool::unpack(const pchar *value)
{
    const pchar *v = pStringTrim(value);
    if (pstrcmp(v, "true") == 0)
    {
        m_value = true;
    }
    else
    {
        m_value = false;
    }

    return true;
}
