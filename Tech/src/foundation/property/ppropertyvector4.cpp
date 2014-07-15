// ppropertyvector4.cpp
// 1x4 floating vector property
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "ppropertyvector4.h"

#include <PFoundation/pvariant.h>
#include <PFoundation/pconststring.h>


PPropertyNameIndexMap PPropertyVector4::m_propertyNameIndexMap;

#if defined P_WIN32
#pragma warning( disable : 4355 )
#endif

PPropertyVector4::PPropertyVector4()
    : PComboProperty(P_PROPERTY_VECTOR4) 
    , m_x(this)
    , m_y(this)
    , m_z(this)
    , m_w(this)
{
    m_propertyNameIndexMap.initialize(PComboProperty::propertyNameIndexMap());

    addSubproperty(&m_propertyNameIndexMap, "x", &m_x);
    addSubproperty(&m_propertyNameIndexMap, "y", &m_y);
    addSubproperty(&m_propertyNameIndexMap, "z", &m_z);
    addSubproperty(&m_propertyNameIndexMap, "w", &m_z);

    reset();
}

void PPropertyVector4::operator=(const PPropertyVector4& other)
{
    if (this != &other)
    {
        m_x = other.m_x;
        m_y = other.m_y;
        m_z = other.m_z;
        m_w = other.m_w;
    }
}

PPropertyVector4::~PPropertyVector4()
{
    m_propertyNameIndexMap.release();
}

#if defined P_WIN32
#pragma warning( default : 4355 )
#endif

void PPropertyVector4::setValue(const PAbstractProperty& property)
{
    if (this != &property)
    {
        PASSERT(property.type() == P_PROPERTY_VECTOR4);
        const PPropertyVector4* p = (const PPropertyVector4*)&property;
        operator=(p->toVector4());
    }
}

void PPropertyVector4::setValue(const void* value)
{
    PASSERT(value != P_NULL);
    const pfloat32 *v = (const pfloat32*)value; 
    setVector4(v[0], v[1], v[2], v[3]);
}

void PPropertyVector4::setValue(const PVariant &value)
{
    PASSERT(value.getType() == P_VARIANT_VECTOR4);
    if (value.getType() == P_VARIANT_VECTOR4)
    {
        operator=(value.toVector4());
    }
}

void PPropertyVector4::reset()
{
    setX(1);
    setY(0);
    setZ(0);
    setW(0);
}

void PPropertyVector4::setVector4(pfloat32 x, pfloat32 y, pfloat32 z, pfloat32 w)
{
    m_x = x;
    m_y = y;
    m_z = z;
    m_w = w;
}

void PPropertyVector4::operator=(const PVector4& v)
{
    m_x = v[0];
    m_y = v[1];
    m_z = v[2];
    m_w = v[3];
}

PVector4 PPropertyVector4::toVector4() const
{
    return pVector4(m_x.toFloat(), m_y.toFloat(), m_z.toFloat(), m_w.toFloat());
}

void PPropertyVector4::getVector4(pfloat32 *v) const
{
    v[0] = m_x.toFloat();
    v[1] = m_y.toFloat();
    v[2] = m_z.toFloat();
    v[3] = m_w.toFloat();
}

void PPropertyVector4::interpolate(pfloat32 t, PAbstractProperty *a, PAbstractProperty *b)
{
    PASSERT(t >= 0 && t <= 1);

    PASSERT(a->type() != P_PROPERTY_VECTOR4);
    PASSERT(b->type() != P_PROPERTY_VECTOR4);

    PPropertyVector4 *v1 = (PPropertyVector4*)a;
    PPropertyVector4 *v2 = (PPropertyVector4*)b;

    m_x.interpolate(t, &(v1->m_x), &(v2->m_x));
    m_y.interpolate(t, &(v1->m_y), &(v2->m_y));
    m_z.interpolate(t, &(v1->m_z), &(v2->m_z));
    m_w.interpolate(t, &(v1->m_w), &(v2->m_w));
}

pbool PPropertyVector4::unpack(const pchar *value)
{
    const pchar *p = value;
    pfloat32 x, y, z, w;

    if ((p = pStringUnpackFloat(p, &x)) != P_NULL &&
        (p = pStringUnpackFloat(p, &y)) != P_NULL &&
        (p = pStringUnpackFloat(p, &z)) != P_NULL &&
        (p = pStringUnpackFloat(p, &w)) != P_NULL)
    {
        m_x = x;
        m_y = y;
        m_z = z;
        m_w = w;

        return true;
    }
    
    PLOG_ERROR("Failed to unpack a vector4 property called %s", name());

    return false;
}

PPropertyNameIndexMap *PPropertyVector4::propertyNameIndexMap() const
{
    return &m_propertyNameIndexMap;
}

