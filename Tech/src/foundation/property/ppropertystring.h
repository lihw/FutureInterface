// ppropertystring.h
// The string property.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#ifndef PPROPERTYSTRING_H
#define PPROPERTYSTRING_H

#include <PFoundation/pabstractproperty.h>

#include <PFoundation/pstring.h>

class P_DLLEXPORT PPropertyString : public PBasicProperty
{
private:
    PPropertyString(const PPropertyString& other) : PBasicProperty(P_PROPERTY_STRING, P_NULL) {}

public:
    PPropertyString(PAbstractProperty *parent = P_NULL);
    virtual ~PPropertyString();
    
    void operator=(const PPropertyString& other);
    
    virtual void interpolate(pfloat32 t, PAbstractProperty *v1, PAbstractProperty *v2);
    
    virtual void setValue(const PAbstractProperty& property);
    virtual void setValue(const void* value);
    virtual void setValue(const PVariant &value);
    virtual pbool unpack(const pchar *value);
    virtual void reset() { m_value = PString(); }

    P_INLINE void operator=(const PString &value) { m_value = value; }
    P_INLINE const PString &toString() const      { return m_value;  }

private:
    PString m_value;
};

#endif // !PPROPERTYSTRING_H

