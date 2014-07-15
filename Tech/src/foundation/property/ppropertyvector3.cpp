// ppropertyvector3.cpp
// 1x3 floating vector property
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "ppropertyvector3.h"

#include <PFoundation/pvariant.h>
#include <PFoundation/pconststring.h>


PPropertyNameIndexMap PPropertyVector3::m_propertyNameIndexMap;

#if defined P_WIN32
#pragma warning( disable : 4355 )
#endif

PPropertyVector3::PPropertyVector3()
    : PComboProperty(P_PROPERTY_VECTOR3) 
    , m_x(this)
    , m_y(this)
    , m_z(this)
{
    m_propertyNameIndexMap.initialize(PComboProperty::propertyNameIndexMap());

    addSubproperty(&m_propertyNameIndexMap, "x", &m_x);
    addSubproperty(&m_propertyNameIndexMap, "y", &m_y);
    addSubproperty(&m_propertyNameIndexMap, "z", &m_z);

    reset();
}

void PPropertyVector3::operator=(const PPropertyVector3& other)
{
    if (this != &other)
    {
        m_x = other.m_x;
        m_y = other.m_y;
        m_z = other.m_z;
    }
}

PPropertyVector3::~PPropertyVector3()
{
    m_propertyNameIndexMap.release();
}

#if defined P_WIN32
#pragma warning( default : 4355 )
#endif

void PPropertyVector3::setValue(const PAbstractProperty& property)
{
    if (this != &property)
    {
        PASSERT(property.type() == P_PROPERTY_VECTOR3);
        const PPropertyVector3* p = (const PPropertyVector3*)&property;
        operator=(p->toVector3());
    }
}

void PPropertyVector3::setValue(const void* value)
{
    PASSERT(value != P_NULL);
    const pfloat32 *v = (const pfloat32*)value; 
    setVector3(v[0], v[1], v[2]);
}
    
void PPropertyVector3::setValue(const PVariant &value)
{
    PASSERT(value.getType() == P_VARIANT_VECTOR3);
    if (value.getType() == P_VARIANT_VECTOR3)
    {
        operator=(value.toVector3());
    }
}

void PPropertyVector3::reset()
{
    setX(1);
    setY(0);
    setZ(0);
}

void PPropertyVector3::setVector3(pfloat32 x, pfloat32 y, pfloat32 z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

void PPropertyVector3::operator=(const PVector3 &v)
{
    m_x = v[0];
    m_y = v[1];
    m_z = v[2];
}

PVector3 PPropertyVector3::toVector3() const
{
    return pVector3(m_x.toFloat(), m_y.toFloat(), m_z.toFloat());
}
    
void PPropertyVector3::interpolate(pfloat32 t, PAbstractProperty *a, PAbstractProperty *b)
{
    PASSERT(t >= 0 && t <= 1);

    PASSERT(a->type() != P_PROPERTY_VECTOR3);
    PASSERT(b->type() != P_PROPERTY_VECTOR3);

    PPropertyVector3 *v1 = (PPropertyVector3*)a;
    PPropertyVector3 *v2 = (PPropertyVector3*)b;

    m_x.interpolate(t, &(v1->m_x), &(v2->m_x));
    m_y.interpolate(t, &(v1->m_y), &(v2->m_y));
    m_z.interpolate(t, &(v1->m_z), &(v2->m_z));
}
    
pbool PPropertyVector3::unpack(const pchar *value)
{
    const pchar *p = value;
    pfloat32 x, y, z;

    if ((p = pStringUnpackFloat(p, &x)) != P_NULL &&
        (p = pStringUnpackFloat(p, &y)) != P_NULL &&
        (p = pStringUnpackFloat(p, &z)) != P_NULL)
    {
        m_x = x;
        m_y = y;
        m_z = z;

        return true;
    }
    
    PLOG_ERROR("Failed to unpack a vector3 property called %s", name());

    return false;
}

PPropertyNameIndexMap *PPropertyVector3::propertyNameIndexMap() const
{
    return &m_propertyNameIndexMap;
}
