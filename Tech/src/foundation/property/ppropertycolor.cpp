// ppropertycolor.h
// RGBA color property.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "ppropertycolor.h"

#include <PFoundation/pvariant.h>
#include <PFoundation/pconststring.h>


PPropertyNameIndexMap PPropertyColor::m_propertyNameIndexMap;

#if defined P_WIN32
#pragma warning( disable : 4355 )
#endif

PPropertyColor::PPropertyColor()
    : PComboProperty(P_PROPERTY_COLOR)
    , m_r(this)
    , m_g(this)
    , m_b(this)
    , m_a(this)
{
    m_propertyNameIndexMap.initialize(PComboProperty::propertyNameIndexMap());

    addSubproperty(&m_propertyNameIndexMap, "r", &m_r);
    addSubproperty(&m_propertyNameIndexMap, "g", &m_g);
    addSubproperty(&m_propertyNameIndexMap, "b", &m_b);
    addSubproperty(&m_propertyNameIndexMap, "a", &m_a);

    m_r.setRange(0, 1);
    m_g.setRange(0, 1);
    m_b.setRange(0, 1);
    m_a.setRange(0, 1);

    reset();
}

#if defined P_WIN32
#pragma warning( default : 4355 )
#endif

PPropertyColor::~PPropertyColor()
{
    m_propertyNameIndexMap.release();
}
    
void PPropertyColor::operator=(const PPropertyColor &other)
{
    if (this != &other)
    {
        m_r = other.m_r;
        m_g = other.m_g;
        m_b = other.m_b;
        m_a = other.m_a;
    }
}
    
void PPropertyColor::setValue(const PAbstractProperty &property)
{
    if (&property != this)
    {
        PASSERT(property.type() == P_PROPERTY_COLOR);
        const PPropertyColor* p = (const PPropertyColor *)(&property);

        m_r = p->r();
        m_g = p->g();
        m_b = p->b();
        m_a = p->a();
    }
}
    
void PPropertyColor::setValue(const void* value)
{
    const pfloat32* c = (const pfloat32*)value;

    m_r = c[0];
    m_g = c[1]; 
    m_b = c[2];
    m_a = c[3];
}
    
void PPropertyColor::setValue(const PVariant &value)
{
    PASSERT(value.getType() == P_VARIANT_COLOR);
    if (value.getType() == P_VARIANT_COLOR)
    {
        PColorRGBA color = value.toColorRGBA();

        m_r = color.getRFloat();
        m_g = color.getGFloat();
        m_b = color.getBFloat();
        m_a = color.getAFloat();
    }
}
    
void PPropertyColor::interpolate(pfloat32 t, PAbstractProperty *a, PAbstractProperty *b)
{
    PASSERT(t >= 0 && t <= 1);

    PASSERT(a->type() != P_PROPERTY_COLOR);
    PASSERT(b->type() != P_PROPERTY_COLOR);

    PPropertyColor *v1 = (PPropertyColor*)a;
    PPropertyColor *v2 = (PPropertyColor*)b;

    m_r.interpolate(t, &(v1->m_r), &(v2->m_r));
    m_g.interpolate(t, &(v1->m_g), &(v2->m_g));
    m_b.interpolate(t, &(v1->m_b), &(v2->m_b));
    m_a.interpolate(t, &(v1->m_a), &(v2->m_a));
}
    
void PPropertyColor::reset()
{
    m_r = 0.5f;
    m_g = 0.5f;
    m_b = 0.5f;
    m_a = 1.0f;
}
    
void PPropertyColor::operator=(const PColorRGBA &color)
{
    m_r = color.getRFloat();
    m_g = color.getGFloat();
    m_b = color.getBFloat();
    m_a = color.getAFloat();
}

PColorRGBA PPropertyColor::toColor() const
{
    // FIXME: the precision is lost.
    PColorRGBA ret = 
    {
        (puint8)(m_r.toFloat() * 255.0f),
        (puint8)(m_g.toFloat() * 255.0f),
        (puint8)(m_b.toFloat() * 255.0f),
        (puint8)(m_a.toFloat() * 255.0f),
    };

    return ret;
}
    
pbool PPropertyColor::unpack(const pchar *value)
{
    const pchar *p = value;

    pfloat32 r;
    pfloat32 g;
    pfloat32 b;
    pfloat32 a;

    if ((p = pStringUnpackTrimAnnotation(p)) != P_NULL &&
        (p = pStringUnpackFloat(p, &r)) != P_NULL &&
        (p = pStringUnpackFloat(p, &g)) != P_NULL &&
        (p = pStringUnpackFloat(p, &b)) != P_NULL &&
        (p = pStringUnpackFloat(p, &a)) != P_NULL)
    {
        m_r = r;
        m_g = g;
        m_b = b;
        m_a = a;

        return true;
    }
        
    PLOG_ERROR("Failed to unpack a color property called %s", name());
    return false;
}

PPropertyNameIndexMap* PPropertyColor::propertyNameIndexMap() const
{
    return &m_propertyNameIndexMap;
}

