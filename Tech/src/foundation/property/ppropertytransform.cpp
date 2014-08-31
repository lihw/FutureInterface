// ppropertytransform.cpp
// 4x4 matrix which can be splitted into a scaling, a translation and a rotation.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "ppropertytransform.h"

#include <PFoundation/pvariant.h>
#include <PFoundation/pconststring.h>

#include <math.h>

//
// class PPropertyTransform
//

PPropertyNameIndexMap PPropertyTransform::m_propertyNameIndexMap; 

#if defined P_WIN32
#pragma warning( disable : 4355 )
#endif

PPropertyTransform::PPropertyTransform()
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
{
    // Manager properties
    m_propertyNameIndexMap.initialize(PComboProperty::propertyNameIndexMap());

    addSubproperty(&m_propertyNameIndexMap, "translationx", &m_translationx);
    addSubproperty(&m_propertyNameIndexMap, "translationy", &m_translationy);
    addSubproperty(&m_propertyNameIndexMap, "translationz", &m_translationz);
    addSubproperty(&m_propertyNameIndexMap, "rotationx",    &m_rotationx);
    addSubproperty(&m_propertyNameIndexMap, "rotationy",    &m_rotationy);
    addSubproperty(&m_propertyNameIndexMap, "rotationz",    &m_rotationz);
    addSubproperty(&m_propertyNameIndexMap, "scalingx",     &m_scalingx);
    addSubproperty(&m_propertyNameIndexMap, "scalingy",     &m_scalingy);
    addSubproperty(&m_propertyNameIndexMap, "scalingz",     &m_scalingz);

    reset();
}
    
#if defined P_WIN32
#pragma warning( default : 4355 )
#endif

PPropertyTransform::~PPropertyTransform()
{
    m_propertyNameIndexMap.release();
}

void PPropertyTransform::reset()
{
    m_translationx = 0.0f;
    m_translationy = 0.0f;
    m_translationz = 0.0f;

    m_rotationx = 0.0f;
    m_rotationy = 0.0f;
    m_rotationz = 0.0f;
    
    m_scalingx = 1.0f;
    m_scalingy = 1.0f;
    m_scalingz = 1.0f;

    m_matrix.identity();

    m_dirty = false;
    m_changed = true;
}

void PPropertyTransform::setValue(const PAbstractProperty &property)
{
    if (this != &property)
    {
        PASSERT(property.type() == P_PROPERTY_TRANSFORM);
        const PPropertyTransform *p = (const PPropertyTransform*)&property;
        
        operator=(*p);
    }
}

void PPropertyTransform::operator=(const PPropertyTransform& other)
{
    if (this != &other) 
    {
        m_translationx = other.m_translationx;
        m_translationy = other.m_translationy;
        m_translationz = other.m_translationz;

        m_rotationx = other.m_rotationx;
        m_rotationy = other.m_rotationy;
        m_rotationz = other.m_rotationz;

        m_scalingx = other.m_scalingx;
        m_scalingy = other.m_scalingy;
        m_scalingz = other.m_scalingz;

        m_matrix = other.m_matrix;

        m_dirty = other.m_dirty;
        
        m_changed = true;
    }
}
    
void PPropertyTransform::setValue(const PVariant &value)
{
    PASSERT(value.getType() == P_VARIANT_MATRIX4X4);
    if (value.getType() == P_VARIANT_MATRIX4X4)
    {
        setMatrix4x4(value.toMatrix4x4().m_m);
    }
}
    
void PPropertyTransform::setValue(const void* value)
{
    // TODO:
    PASSERT_NOTIMPLEMENTED();
}

void PPropertyTransform::setTranslationX(pfloat32 x)
{
    m_translationx = x;
    m_dirty = true;
}

void PPropertyTransform::setTranslationY(pfloat32 y)
{
    m_translationy = y;
    m_dirty = true;
}

void PPropertyTransform::setTranslationZ(pfloat32 z)
{
    m_translationz = z;
    m_dirty = true;
}

pfloat32 PPropertyTransform::translationX() const
{
    return m_translationx.toFloat();
}

pfloat32 PPropertyTransform::translationY() const
{
    return m_translationy.toFloat();
}

pfloat32 PPropertyTransform::translationZ() const
{
    return m_translationz.toFloat();
}

PVector3 PPropertyTransform::translation() const
{
    return pVector3(m_translationx.toFloat(),
                    m_translationy.toFloat(),
                    m_translationz.toFloat());
}

void PPropertyTransform::setTranslation(const PVector3 &v)
{
    m_translationx = v[0];
    m_translationy = v[1]; 
    m_translationz = v[2]; 

    m_dirty = true;
}

void PPropertyTransform::setRotationX(pfloat32 x)
{
    m_rotationx = x;
    m_dirty = true;
}

void PPropertyTransform::setRotationY(pfloat32 y)
{
    m_rotationy = y;
    m_dirty = true;
}

void PPropertyTransform::setRotationZ(pfloat32 z)
{
    m_rotationz = z;
    m_dirty = true;
}

pfloat32 PPropertyTransform::rotationX() const
{
    return m_rotationx.toFloat();
}

pfloat32 PPropertyTransform::rotationY() const
{
    return m_rotationy.toFloat();
}

pfloat32 PPropertyTransform::rotationZ() const
{
    return m_rotationz.toFloat();
}

PVector3 PPropertyTransform::rotation() const
{
    return pVector3(m_rotationx.toFloat(),
                    m_rotationy.toFloat(),
                    m_rotationz.toFloat());
}

void PPropertyTransform::setRotation(const PVector3 &v)
{
    m_rotationx = v[0];
    m_rotationy = v[1];
    m_rotationz = v[2];

    m_dirty = true;
}

void PPropertyTransform::setScalingX(pfloat32 x)
{
    m_scalingx = x;
    m_dirty = true;
}

void PPropertyTransform::setScalingY(pfloat32 y)
{
    m_scalingy = y;
    m_dirty = true;
}

void PPropertyTransform::setScalingZ(pfloat32 z)
{
    m_scalingz = z;
    m_dirty = true;
}

pfloat32 PPropertyTransform::scalingX() const
{
    return m_scalingx.toFloat();
}

pfloat32 PPropertyTransform::scalingY() const
{
    return m_scalingy.toFloat();
}

pfloat32 PPropertyTransform::scalingZ() const
{
    return m_scalingz.toFloat();
}

PVector3 PPropertyTransform::scaling() const
{
    return pVector3(m_scalingx.toFloat(),
                    m_scalingy.toFloat(),
                    m_scalingz.toFloat());
}

void PPropertyTransform::setScaling(const PVector3 &v)
{
    m_scalingx = v[0];
    m_scalingy = v[1];
    m_scalingz = v[2];
    
    m_dirty = true;
}
    
void PPropertyTransform::setRotation(pfloat32 angle, pfloat32 x, pfloat32 y, pfloat32 z)
{
    // TODO:
    PASSERT_NOTIMPLEMENTED();
}
    
void PPropertyTransform::setRotation(pfloat32 angle, const PLine &line)
{
    // TODO:
    PASSERT_NOTIMPLEMENTED();
}
    
void PPropertyTransform::setLookAt(pfloat32 eyex, pfloat32 eyey, pfloat32 eyez,
                                   pfloat32 centerx, pfloat32 centery, pfloat32 centerz,
                                   pfloat32 upx, pfloat32 upy, pfloat32 upz)
{
    pfloat32 eye[]    = {eyex, eyey, eyez};
    pfloat32 center[] = {centerx, centery, centerz};
    pfloat32 up[]     = {upx, upy, upz};

    pMatrix4x4LookAt(eye, center, up, m_matrix.m_m);

    pfloat32 rx;
    pfloat32 ry;
    pfloat32 rz;
    pMatrix4x4GetRotationRxRyRz(m_matrix.m_m, &rx, &ry, &rz);

    // Need to convert the local transform to camera transform
    m_rotationx = -rx;
    m_rotationy = -ry;
    m_rotationz = -rz;

    m_scalingx = 1.0f;
    m_scalingy = 1.0f;
    m_scalingz = 1.0f;

    m_translationx = eyex;
    m_translationy = eyey;
    m_translationz = eyez;
    
    m_dirty = false;
    m_changed = true;
}

const PMatrix4x4 & PPropertyTransform::toMatrix4x4(pbool *changed) const
{
    *changed = m_changed;

    if (m_dirty)
    {
        pMatrix4x4CreateScaling(m_scalingx.toFloat(), m_scalingy.toFloat(), m_scalingz.toFloat(), m_matrix.m_m);

        pMatrix4x4RotateRxRyRz(m_matrix.m_m,
                               m_rotationx.toFloat(),
                               m_rotationy.toFloat(),
                               m_rotationz.toFloat());
        

        m_matrix[12] = m_translationx.toFloat();
        m_matrix[13] = m_translationy.toFloat();
        m_matrix[14] = m_translationz.toFloat();

        *changed = true;
    }
    
    m_changed = false;

    return m_matrix;
}

const PMatrix4x4 PPropertyTransform::toCameraMatrix4x4() const
{
    PMatrix4x4 matrix;

    // M = -R * (-T)
    // M: camera matrix
    // R: the rotation component of the camera
    // T: the translation component of the camera.
    pfloat32 rotation[16];
    pfloat32 translation[16];

    pMatrix4x4CreateRotationRxRyRz(-m_rotationx.toFloat(),
                                   -m_rotationy.toFloat(),
                                   -m_rotationz.toFloat(),
                                   rotation);
    pMatrix4x4CreateTranslation(-m_translationx.toFloat(),
                                -m_translationy.toFloat(),
                                -m_translationz.toFloat(),
                                translation);

    pMatrix4x4Multiply(rotation, translation, matrix.m_m);

    return matrix;
}

void PPropertyTransform::operator=(const PMatrix4x4 &matrix)
{
    setMatrix4x4(matrix.m_m);
}

void PPropertyTransform::setMatrix4x4(const pfloat32 *matrix)
{
    m_matrix.copy(matrix);
    
    pfloat32 sx;
    pfloat32 sy;
    pfloat32 sz;
    pMatrix4x4GetScaling(matrix, &sx, &sy, &sz);

    m_scalingx = sx;
    m_scalingy = sy;
    m_scalingz = sz;
    
    m_translationx = matrix[12];
    m_translationy = matrix[13];
    m_translationz = matrix[14];
    
    pfloat32 matrixRT[16];
    pMatrix4x4Copy(matrix, matrixRT);

    matrixRT[0] /= sx;
    matrixRT[1] /= sx;
    matrixRT[2] /= sx;
    
    matrixRT[4] /= sy;
    matrixRT[5] /= sy;
    matrixRT[6] /= sy;
    
    matrixRT[8]  /= sz;
    matrixRT[9]  /= sz;
    matrixRT[10] /= sz;

    // FIXME: matrix->rotation->matrix is buggy. Please verify.
    pfloat32 rx;
    pfloat32 ry;
    pfloat32 rz;
    pMatrix4x4GetRotationRxRyRz(matrixRT, &rx, &ry, &rz);

    m_rotationx = rx;
    m_rotationy = ry;
    m_rotationz = rz;

    m_dirty = false;
    m_changed = true;
}

void PPropertyTransform::interpolate(pfloat32 t, PAbstractProperty *a, PAbstractProperty *b)
{
    // TODO:
    PASSERT_NOTIMPLEMENTED();
}
    
pbool PPropertyTransform::unpack(const pchar *value)
{
    const pchar *p = value;
    pfloat32 sx, sy, sz;
    pfloat32 rx, ry, rz;
    pfloat32 tx, ty, tz;

    if ((p = pStringUnpackTrimAnnotation(p)) != P_NULL &&
        (p = pStringUnpackFloat(p, &sx)) != P_NULL &&
        (p = pStringUnpackFloat(p, &sy)) != P_NULL &&
        (p = pStringUnpackFloat(p, &sz)) != P_NULL &&
        (p = pStringUnpackTrimAnnotation(p)) != P_NULL &&
        (p = pStringUnpackFloat(p, &rx)) != P_NULL &&
        (p = pStringUnpackFloat(p, &ry)) != P_NULL &&
        (p = pStringUnpackFloat(p, &rz)) != P_NULL &&
        (p = pStringUnpackTrimAnnotation(p)) != P_NULL &&
        (p = pStringUnpackFloat(p, &tx)) != P_NULL &&
        (p = pStringUnpackFloat(p, &ty)) != P_NULL &&
        (p = pStringUnpackFloat(p, &tz)) != P_NULL)
    {
        m_translationx = tx;
        m_translationy = ty;
        m_translationz = tz;
        
        m_scalingx = sx;
        m_scalingy = sy;
        m_scalingz = sz;
        
        m_rotationx = rx;
        m_rotationy = ry;
        m_rotationz = rz;

        m_dirty = true;

        return true;
    }
    
    PLOG_ERROR("Failed to unpack a transform property called %s", name());
    return false;
}
    
PPropertyNameIndexMap* PPropertyTransform::propertyNameIndexMap() const
{
    return &m_propertyNameIndexMap;
}


