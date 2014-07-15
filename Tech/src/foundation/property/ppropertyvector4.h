// ppropertyvector4.h
// 1x4 floating vector property
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PPROPERTYVECTOR4_H
#define PPROPERTYVECTOR4_H

#include <PFoundation/ppropertyfloat.h>

#include <PFoundation/pvector4.h>

class P_DLLEXPORT PPropertyVector4 : public PComboProperty
{
private:
    PPropertyVector4(const PPropertyVector4& other)
        : PComboProperty(P_PROPERTY_FLOAT)
        , m_x(this)
        , m_y(this)
        , m_z(this)
    {}

public:
    PPropertyVector4();
    virtual ~PPropertyVector4();
    
    void operator=(const PPropertyVector4& other);

    virtual void interpolate(pfloat32 t, PAbstractProperty *a, PAbstractProperty *b);

    virtual void setValue(const PAbstractProperty &property);
    virtual void setValue(const void *value);
    virtual void setValue(const PVariant &value);
    virtual void reset();
    virtual pbool unpack(const pchar *value);

    P_INLINE pfloat32 getX() const { return m_x.toFloat(); }
    P_INLINE pfloat32 getY() const { return m_y.toFloat(); }
    P_INLINE pfloat32 getZ() const { return m_z.toFloat(); }
    P_INLINE pfloat32 getW() const { return m_w.toFloat(); }
    
    P_INLINE void setX(pfloat32 x) { m_x = x; }
    P_INLINE void setY(pfloat32 y) { m_y = y; }
    P_INLINE void setZ(pfloat32 z) { m_z = z; }
    P_INLINE void setW(pfloat32 w) { m_w = w; }

    void setVector4(pfloat32 x, pfloat32 y, pfloat32 z, pfloat32 w);
    void operator=(const PVector4 &v);

    PVector4 toVector4() const;
    void getVector4(pfloat32 *v) const;
    
    P_INLINE pfloat32 operator[](pint32 i) const 
    { 
        PASSERTINFO(i >= 0 && i < 4, "out of range"); 

        if (i == 0) return m_x.toFloat();
        else if (i == 1) return m_y.toFloat();
        else if (i == 2) return m_z.toFloat();
        else if (i == 3) return m_w.toFloat();
        
        return 0;
    }

protected:
    virtual PPropertyNameIndexMap *propertyNameIndexMap() const;

private:
    PPropertyFloat m_x;
    PPropertyFloat m_y;
    PPropertyFloat m_z;
    PPropertyFloat m_w;
    
    static PPropertyNameIndexMap m_propertyNameIndexMap;
};


#endif // !PPROPERTYVECTOR4_H


