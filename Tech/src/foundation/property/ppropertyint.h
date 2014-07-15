// ppropertyint.h
// The 32-bit signed integer property.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#ifndef PPROPERTYINT_H
#define PPROPERTYINT_H

#include <PFoundation/pabstractproperty.h>

class P_DLLEXPORT PPropertyInt : public PBasicProperty
{
private:
    PPropertyInt(const PPropertyInt& other) : PBasicProperty(P_PROPERTY_INT, P_NULL) {}

public:
    PPropertyInt(PAbstractProperty* parent = P_NULL);
    virtual ~PPropertyInt();
    
    void operator=(const PPropertyInt& other);
    
    virtual void interpolate(pfloat32 t, PAbstractProperty *v1, PAbstractProperty *v2);

    virtual void setValue(const PAbstractProperty &property);
    virtual void setValue(const void *value);
    virtual void setValue(const PVariant &value);
    virtual void reset() { m_value = m_range[0] / 2 + m_range[1] / 2; }
    virtual pbool unpack(const pchar *value);
    
    P_INLINE pint32 toInt() const        { return m_value;    }
    P_INLINE pint32 getLower() const      { return m_range[0]; }
    P_INLINE pint32 getUpper() const      { return m_range[1]; }

    void operator=(pint32 value);
    void setRange(const pint32* range);
    void setRange(pint32 lower, pint32 upper);

private:
    pint32 m_value;
    pint32 m_range[2];
};

#endif // !PPROPERTYINT_H

