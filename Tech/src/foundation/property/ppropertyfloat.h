// ppropertyfloat.h
// 32-bit floating number property.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#ifndef PPROPERTYFLOAT_H
#define PPROPERTYFLOAT_H

#include <PFoundation/pabstractproperty.h>

class P_DLLEXPORT PPropertyFloat : public PBasicProperty
{
private:
    PPropertyFloat(const PPropertyFloat &other) : PBasicProperty(P_PROPERTY_FLOAT, P_NULL) {}

public:
    PPropertyFloat(PAbstractProperty *parent = P_NULL);
    virtual ~PPropertyFloat();
    
    void operator=(const PPropertyFloat& other);
    
    virtual void interpolate(pfloat32 t, PAbstractProperty *v1, PAbstractProperty *v2);

    virtual void setValue(const PAbstractProperty &property);
    virtual void setValue(const void *value);
    virtual void setValue(const PVariant &value);
    virtual void reset() { m_value = m_range[0] * 0.5f + m_range[1] * 0.5f; }
    virtual pbool unpack(const pchar *value);

    P_INLINE pfloat32 toFloat() const        { return m_value;    }
    P_INLINE pfloat32 getLower() const        { return m_range[0]; }
    P_INLINE pfloat32 getUpper() const        { return m_range[1]; }

    void operator=(pfloat32 value);
    void setRange(const pfloat32 *range);
    void setRange(pfloat32 lower, pfloat32 upper);

private:
    pfloat32 m_value;
    pfloat32 m_range[2];
};


#endif // !PPROPERTYFLOAT_H

