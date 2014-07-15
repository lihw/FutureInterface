// ppropertytransform.h
// 4x4 matrix which can be splitted into a scaling, a translation and a rotation.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PPROPERTYTRANSFORM_H
#define PPROPERTYTRANSFORM_H

#include <PFoundation/ppropertyfloat.h>

#include <PFoundation/pvector3.h>
#include <PFoundation/pline.h>
#include <PFoundation/pmatrix4x4.h>


class P_DLLEXPORT PPropertyTransform : public PComboProperty
{
private:
    PPropertyTransform(const PPropertyTransform &other) 
        : PComboProperty(P_PROPERTY_TRANSFORM)
        , m_translationx(this)
        , m_translationy(this)
        , m_translationz(this)
        , m_rotationx(this)
        , m_rotationy(this)
        , m_rotationz(this)
        , m_scalingx(this)
        , m_scalingy(this)
        , m_scalingz(this)
    {}

public:
    PPropertyTransform(); 
    virtual ~PPropertyTransform();
    
    void operator=(const PPropertyTransform &other);
    
    virtual void reset();
    virtual void setValue(const PAbstractProperty &property);
    virtual void setValue(const void *value);
    virtual void setValue(const PVariant &value);
    virtual void interpolate(pfloat32 t, PAbstractProperty *a, PAbstractProperty *b);
    virtual pbool unpack(const pchar *value);

    void setTranslationX(pfloat32 x);
    void setTranslationY(pfloat32 y);
    void setTranslationZ(pfloat32 z);
    pfloat32 translationX() const;
    pfloat32 translationY() const;
    pfloat32 translationZ() const;
    void setTranslation(const PVector3 &v);
    PVector3 translation() const;

    void setRotationX(pfloat32 x);
    void setRotationY(pfloat32 y);
    void setRotationZ(pfloat32 z);
    pfloat32 rotationX() const;
    pfloat32 rotationY() const;
    pfloat32 rotationZ() const;
    void setRotation(const PVector3 &v);
    PVector3 rotation() const;
    void setRotation(pfloat32 angle, pfloat32 x, pfloat32 y, pfloat32 z);
    void setRotation(pfloat32 angle, const PLine &line);

    void setScalingX(pfloat32 x);
    void setScalingY(pfloat32 y);
    void setScalingZ(pfloat32 z);
    pfloat32 scalingX() const;
    pfloat32 scalingY() const;
    pfloat32 scalingZ() const;
    void setScaling(const PVector3 &v);
    PVector3 scaling() const;

    void setLookAt(pfloat32 eyex, pfloat32 eyey, pfloat32 eyez,
                pfloat32 centerx, pfloat32 centery, pfloat32 centerz,
                pfloat32 upx, pfloat32 upy, pfloat32 upz);

    // The parameter changed indicate whether the transform matrix has been update since the last
    // time when getMatrix is called.
    const PMatrix4x4 &toMatrix4x4(pbool *changed) const;
    // To a camera transform matrix
    const PMatrix4x4 toCameraMatrix4x4() const;
    void setMatrix4x4(const pfloat32 *matrix);
    void operator=(const PMatrix4x4 &matrix);

protected:
    virtual PPropertyNameIndexMap *propertyNameIndexMap() const;

private:
    class PPropertyFloatTranslationX : public PPropertyFloat
    {
    public:
        P_INLINE PPropertyFloatTranslationX(PAbstractProperty *property) : PPropertyFloat(property) {};
        virtual void setValue(const void *value) 
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setTranslationX(toFloat()); }
        virtual void setValue(const PAbstractProperty &property) 
        { PPropertyFloat::setValue(property); ((PPropertyTransform*)m_parent)->setTranslationX(toFloat()); }
        virtual void setValue(const PVariant &value)
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setTranslationX(toFloat()); }
        P_INLINE void operator=(pfloat32 v) { PPropertyFloat::operator=(v); }
    };
    class PPropertyFloatTranslationY : public PPropertyFloat
    {
    public:
        P_INLINE PPropertyFloatTranslationY(PAbstractProperty *property) : PPropertyFloat(property) {};
        virtual void setValue(const void *value) 
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setTranslationY(toFloat()); }
        virtual void setValue(const PAbstractProperty &property) 
        { PPropertyFloat::setValue(property); ((PPropertyTransform*)m_parent)->setTranslationY(toFloat()); }
        virtual void setValue(const PVariant &value) 
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setTranslationY(toFloat()); }
        P_INLINE void operator=(pfloat32 v) { PPropertyFloat::operator=(v); }
    };
    class PPropertyFloatTranslationZ : public PPropertyFloat
    {
    public:
        P_INLINE PPropertyFloatTranslationZ(PAbstractProperty *property) : PPropertyFloat(property) {};
        virtual void setValue(const void *value) 
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setTranslationZ(toFloat()); }
        virtual void setValue(const PAbstractProperty &property) 
        { PPropertyFloat::setValue(property); ((PPropertyTransform*)m_parent)->setTranslationZ(toFloat()); }
        virtual void setValue(const PVariant &value) 
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setTranslationZ(toFloat()); }
        P_INLINE void operator=(pfloat32 v) { PPropertyFloat::operator=(v); }
    };
    class PPropertyFloatRotationX : public PPropertyFloat
    {
    public:
        P_INLINE PPropertyFloatRotationX(PAbstractProperty *property) : PPropertyFloat(property) {};
        virtual void setValue(const void *value) 
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setRotationX(toFloat()); }
        virtual void setValue(const PAbstractProperty &property) 
        { PPropertyFloat::setValue(property); ((PPropertyTransform*)m_parent)->setRotationX(toFloat()); }
        virtual void setValue(const PVariant &value) 
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setRotationX(toFloat()); }
        P_INLINE void operator=(pfloat32 v) { PPropertyFloat::operator=(v); }
    };
    class PPropertyFloatRotationY : public PPropertyFloat
    {
    public:
        P_INLINE PPropertyFloatRotationY(PAbstractProperty *property) : PPropertyFloat(property) {};
        virtual void setValue(const void *value) 
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setRotationY(toFloat()); }
        virtual void setValue(const PAbstractProperty &property) 
        { PPropertyFloat::setValue(property); ((PPropertyTransform*)m_parent)->setRotationY(toFloat()); }
        virtual void setValue(const PVariant &value) 
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setRotationY(toFloat()); }
        P_INLINE void operator=(pfloat32 v) { PPropertyFloat::operator=(v); }
    };
    class PPropertyFloatRotationZ : public PPropertyFloat
    {
    public:
        P_INLINE PPropertyFloatRotationZ(PAbstractProperty *property) : PPropertyFloat(property) {};
        virtual void setValue(const void *value) 
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setRotationZ(toFloat()); }
        virtual void setValue(const PAbstractProperty &property) 
        { PPropertyFloat::setValue(property); ((PPropertyTransform*)m_parent)->setRotationZ(toFloat()); }
        virtual void setValue(const PVariant &value) 
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setRotationZ(toFloat()); }
        P_INLINE void operator=(pfloat32 v) { PPropertyFloat::operator=(v); }
    };
    class PPropertyFloatScalingX : public PPropertyFloat
    {
    public:
        P_INLINE PPropertyFloatScalingX(PAbstractProperty *property) : PPropertyFloat(property) {};
        virtual void setValue(const void *value) 
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setScalingX(toFloat()); }
        virtual void setValue(const PAbstractProperty &property) 
        { PPropertyFloat::setValue(property); ((PPropertyTransform*)m_parent)->setScalingX(toFloat()); }
        virtual void setValue(const PVariant &value) 
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setScalingX(toFloat()); }
        P_INLINE void operator=(pfloat32 v) { PPropertyFloat::operator=(v); }
    };
    class PPropertyFloatScalingY : public PPropertyFloat
    {
    public:
        P_INLINE PPropertyFloatScalingY(PAbstractProperty *property) : PPropertyFloat(property) {};
        virtual void setValue(const void *value) 
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setScalingY(toFloat()); }
        virtual void setValue(const PAbstractProperty& property) 
        { PPropertyFloat::setValue(property); ((PPropertyTransform*)m_parent)->setScalingY(toFloat()); }
        virtual void setValue(const PVariant &value) 
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setScalingY(toFloat()); }
        P_INLINE void operator=(pfloat32 v) { PPropertyFloat::operator=(v); }
    };
    class PPropertyFloatScalingZ : public PPropertyFloat
    {
    public:
        P_INLINE PPropertyFloatScalingZ(PAbstractProperty *property) : PPropertyFloat(property) {};
        virtual void setValue(const void *value) 
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setScalingZ(toFloat()); }
        virtual void setValue(const PAbstractProperty& property) 
        { PPropertyFloat::setValue(property); ((PPropertyTransform*)m_parent)->setScalingZ(toFloat()); }
        virtual void setValue(const PVariant &value) 
        { PPropertyFloat::setValue(value); ((PPropertyTransform*)m_parent)->setScalingZ(toFloat()); }
        P_INLINE void operator=(pfloat32 v) { PPropertyFloat::operator=(v); }
    };
private:
    mutable PPropertyFloatTranslationX m_translationx; 
    mutable PPropertyFloatTranslationY m_translationy;
    mutable PPropertyFloatTranslationZ m_translationz;
    
    mutable PPropertyFloatRotationX m_rotationx; ///< rotation Euler angle x in radian.
    mutable PPropertyFloatRotationY m_rotationy; ///< ditto.
    mutable PPropertyFloatRotationZ m_rotationz; ///< ditto.
    
    mutable PPropertyFloatScalingX m_scalingx; 
    mutable PPropertyFloatScalingY m_scalingy; 
    mutable PPropertyFloatScalingZ m_scalingz; 

    mutable PMatrix4x4 m_matrix; ///< 4x4 matrix that combines translation and rotation.

    mutable pbool   m_dirty;   ///< If the matrix needs update.
    mutable pbool   m_changed;   ///< If the matrix is changed since last call of toMatrix4x4()

    static PPropertyNameIndexMap m_propertyNameIndexMap;
};


#endif // !PPROPERTYTRANSFORM_H
