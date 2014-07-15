// ppropertycolor.h
// RGBA color property.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PPROPERTYCOLOR_H
#define PPROPERTYCOLOR_H

#include <PFoundation/ppropertyfloat.h>

#include <PFoundation/pcolorrgba.h>

class P_DLLEXPORT PPropertyColor : public PComboProperty
{
private:
    PPropertyColor(const PPropertyColor& other)
        : PComboProperty(P_PROPERTY_COLOR)
        , m_r(this)
        , m_g(this)
        , m_b(this)
        , m_a(this)
    {}

public:
    PPropertyColor();
    virtual ~PPropertyColor();
    
    void operator=(const PPropertyColor& other);
    
    virtual void setValue(const PAbstractProperty &property);
    virtual void setValue(const void *value);
    virtual void setValue(const PVariant &value);
    virtual void reset();
    virtual pbool unpack(const pchar *value);

    virtual void interpolate(pfloat32 t, PAbstractProperty *a, PAbstractProperty *b);

    P_INLINE void setR(pfloat32 r) { m_r = r; }
    P_INLINE void setG(pfloat32 g) { m_g = g; }
    P_INLINE void setB(pfloat32 b) { m_b = b; }
    P_INLINE void setA(pfloat32 a) { m_a = a; }
    
    P_INLINE pfloat32 r() const { return m_r.toFloat(); } 
    P_INLINE pfloat32 g() const { return m_g.toFloat(); }
    P_INLINE pfloat32 b() const { return m_b.toFloat(); }
    P_INLINE pfloat32 a() const { return m_a.toFloat(); }

    void operator=(const PColorRGBA &color); 
    
    PColorRGBA toColor() const;

protected:
    virtual PPropertyNameIndexMap* propertyNameIndexMap() const;

private:
    PPropertyFloat m_r;
    PPropertyFloat m_g;
    PPropertyFloat m_b;
    PPropertyFloat m_a;

    static PPropertyNameIndexMap m_propertyNameIndexMap;
};


#endif // !PPROPERTYCOLOR_H
