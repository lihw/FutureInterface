// ppropertyvector3.h
// 1x3 floating vector property
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PPROPERTYVECTOR3_H
#define PPROPERTYVECTOR3_H

#include <PFoundation/ppropertyfloat.h>

#include <PFoundation/pvector3.h>

class P_DLLEXPORT PPropertyVector3 : public PComboProperty
{
private:
    PPropertyVector3(const PPropertyVector3& other)
        : PComboProperty(P_PROPERTY_FLOAT)
        , m_x(this)
        , m_y(this)
        , m_z(this)
    {}

public:
    PPropertyVector3();
    virtual ~PPropertyVector3();
    
    void operator=(const PPropertyVector3& other);

    virtual void interpolate(pfloat32 t, PAbstractProperty *a, PAbstractProperty *b);

    virtual void setValue(const PAbstractProperty &property);
    virtual void setValue(const void *value);
    virtual void setValue(const PVariant &value);
    virtual void reset();
    virtual pbool unpack(const pchar *value);

    P_INLINE pfloat32 getX() const { return m_x.toFloat(); }
    P_INLINE pfloat32 getY() const { return m_y.toFloat(); }
    P_INLINE pfloat32 getZ() const { return m_z.toFloat(); }
    
    P_INLINE void setX(pfloat32 x) { m_x = x; }
    P_INLINE void setY(pfloat32 y) { m_y = y; }
    P_INLINE void setZ(pfloat32 z) { m_z = z; }

    void setVector3(pfloat32 x, pfloat32 y, pfloat32 z);
    void operator=(const PVector3 &v);

    PVector3 toVector3() const;
    
    P_INLINE pfloat32 operator[](pint32 i) const 
    { 
        PASSERTINFO(i >= 0 && i < 3, "out of range"); 

        if (i == 0) return m_x.toFloat();
        else if (i == 1) return m_y.toFloat();
        else if (i == 2) return m_z.toFloat();
        
        return 0;
    }

protected:
    virtual PPropertyNameIndexMap *propertyNameIndexMap() const;

private:
    PPropertyFloat m_x;
    PPropertyFloat m_y;
    PPropertyFloat m_z;
    
    static PPropertyNameIndexMap m_propertyNameIndexMap;
};


#endif // !PPROPERTYVECTOR3_H

