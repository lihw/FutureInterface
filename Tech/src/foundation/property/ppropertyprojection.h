// ppropertyprojectionmatrix.h
// Projection matrix.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#ifndef P_PROPERTYPROJECTION_H
#define P_PROPERTYPROJECTION_H

#include <PFoundation/ppropertyfloat.h>

#include <PFoundation/pmatrix4x4.h>


enum PProjectionTypeEnum 
{
    P_PROJECTION_ORTHOGONAL,
    P_PROJECTION_PERSPECTIVE,
    P_PROJECTION_WINDOW,
    P_PROJECTION_FRUSTUM,
    P_PROJECTION_ORTHOGONAL2,
    P_PROJECTION_USER, // if projection martrix is set by user directly

    P_PROJECTION_TYPE_FIRST = P_PROJECTION_ORTHOGONAL,
    P_PROJECTION_TYPE_LAST = P_PROJECTION_USER,
    P_PROJECTION_TYPE_NUMBER = P_PROJECTION_TYPE_LAST - P_PROJECTION_TYPE_FIRST + 1,

    P_PROJECTION_TYPE_DEFAULT = P_PROJECTION_PERSPECTIVE,
};

class P_DLLEXPORT PPropertyProjection : public PComboProperty
{
    friend class PPropertyProjectionFloat;

private:
    PPropertyProjection(const PPropertyProjection &other)
        : PComboProperty(P_PROPERTY_PROJECTION)
        , m_width(this)
        , m_height(this)
        , m_aspect(this)
        , m_fov(this)
        , m_znear(this)
        , m_zfar(this)
        , m_left(this)
        , m_right(this)
        , m_bottom(this)
        , m_top(this)
    {}

public:
    // By default, it is initialized to a perspective projection.
    PPropertyProjection();
    ~PPropertyProjection();

    void operator=(const PPropertyProjection &other);
    
    virtual void reset();
    virtual void setValue(const PAbstractProperty &property);
    virtual void setValue(const void *value);
    virtual void setValue(const PVariant &value);
    virtual pbool unpack(const pchar *value);
    
    virtual void interpolate(pfloat32 t, PAbstractProperty *a, PAbstractProperty *b);

    void perspective(pfloat32 fov, pfloat32 aspect, pfloat32 znear, pfloat32 zfar);
    void orthogonal(pfloat32 aspect, pfloat32 znear, pfloat32 zfar);
    void orthogonal(pfloat32 left, pfloat32 right, pfloat32 bottom, pfloat32 top, pfloat32 znear, pfloat32 far);
    void window(pfloat32 width, pfloat32 height);
    void frustum(pfloat32 left, pfloat32 right, pfloat32 bottom, pfloat32 top, pfloat32 znear, pfloat32 far);

    P_INLINE PProjectionTypeEnum projection() const { return m_projection; }

    pfloat32 aspect() const;
    pfloat32 fov() const;
    pfloat32 zNear() const;
    pfloat32 zFar() const;
    pfloat32 width() const;
    pfloat32 height() const;
    pfloat32 left() const;
    pfloat32 right() const;
    pfloat32 top() const;
    pfloat32 bottom() const;

    const PMatrix4x4 &toMatrix4x4() const;
    PMatrix4x4 toInversedMatrix4x4() const;

    void setMatrix4x4(const pfloat32 *matrix);
    void operator=(const PMatrix4x4 &matrix);

private:
    class PPropertyProjectionFloat: public PPropertyFloat
    {
    public:
        P_INLINE PPropertyProjectionFloat(PAbstractProperty *property) : PPropertyFloat(property) {};
        virtual void setValue(const void *value) 
        { 
            PPropertyFloat::setValue(value);
            ((PPropertyProjection*)m_parent)->m_dirty = true; 
        }
        virtual void setValue(const PAbstractProperty& property) 
        { 
            PPropertyFloat::setValue(property);
            ((PPropertyProjection*)m_parent)->m_dirty = true; 
        }
        void operator=(pfloat32 value) 
        { 
            PPropertyFloat::operator=(value);
            ((PPropertyProjection*)m_parent)->m_dirty = true; 
        }
    };

protected:
    virtual PPropertyNameIndexMap *propertyNameIndexMap() const;

private:
    void perspectivePrivate(pfloat32 fov, pfloat32 aspect, pfloat32 znear, pfloat32 zfar) const;
    void orthogonalPrivate(pfloat32 apsect, pfloat32 znear, pfloat32 zfar) const;
    void windowPrivate(pfloat32 width, pfloat32 height) const;
    void frustumPrivate(pfloat32 left, pfloat32 right, pfloat32 bottom, pfloat32 top, pfloat32 znear, pfloat32 zfar) const;
    void orthogonalPrivate(pfloat32 left, pfloat32 right, pfloat32 bottom, pfloat32 top, pfloat32 znear, pfloat32 zfar) const;

private:
    PProjectionTypeEnum m_projection;

    mutable PPropertyProjectionFloat m_fov;
    mutable PPropertyProjectionFloat m_aspect;
    mutable PPropertyProjectionFloat m_znear;
    mutable PPropertyProjectionFloat m_zfar;
    mutable PPropertyProjectionFloat m_width; 
    mutable PPropertyProjectionFloat m_height;
    mutable PPropertyProjectionFloat m_left;
    mutable PPropertyProjectionFloat m_right;
    mutable PPropertyProjectionFloat m_top;
    mutable PPropertyProjectionFloat m_bottom;

    mutable PMatrix4x4 m_matrix;
    mutable pbool m_dirty;
    static PPropertyNameIndexMap m_propertyNameIndexMap;
};


#endif // !P_PROPERTYPROJECTION_H
