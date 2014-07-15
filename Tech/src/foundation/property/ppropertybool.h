// ppropertybool.h
// The pbool property.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#ifndef PPROPERTYBOOL_H
#define PPROPERTYBOOL_H

#include <PFoundation/pabstractproperty.h>

class P_DLLEXPORT PPropertyBool : public PBasicProperty
{
private:
    PPropertyBool(const PPropertyBool& other) : PBasicProperty(P_PROPERTY_BOOL, P_NULL) {}

public:
    PPropertyBool(PAbstractProperty *parent = P_NULL);
    virtual ~PPropertyBool();
    
    void operator=(const PPropertyBool& other);
    
    virtual void interpolate(pfloat32 t, PAbstractProperty *v1, PAbstractProperty *v2);
    
    virtual void setValue(const PAbstractProperty& property);
    virtual void setValue(const void* value);
    virtual void setValue(const PVariant &value);
    virtual void reset() { m_value = false; }
    virtual pbool unpack(const pchar *value);

    P_INLINE void operator=(pbool value) { m_value = value; }
    P_INLINE pbool toBool() const        { return m_value;  } 

private:
    pbool m_value;
};

#endif // !PPROPERTYBOOL_H

