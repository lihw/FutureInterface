// ppropertyprojectionmatrix.cpp
// Projection matrix.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "ppropertyprojection.h"

#include <PFoundation/pmatrix4x4.h>
#include <PFoundation/pconststring.h>


PPropertyNameIndexMap PPropertyProjection::m_propertyNameIndexMap; 

#if defined P_WIN32
#pragma warning( disable : 4355 )
#endif

PPropertyProjection::PPropertyProjection()
    : PComboProperty(P_PROPERTY_PROJECTION)
    , m_fov(this)
    , m_aspect(this)
    , m_znear(this)
    , m_zfar(this)
    , m_width(this)
    , m_height(this)
    , m_left(this)
    , m_right(this)
    , m_bottom(this)
    , m_top(this)
{
    m_propertyNameIndexMap.initialize(PComboProperty::propertyNameIndexMap());

    addSubproperty(&m_propertyNameIndexMap, "fov",    &m_fov);
    addSubproperty(&m_propertyNameIndexMap, "aspect", &m_aspect);
    addSubproperty(&m_propertyNameIndexMap, "znear",  &m_znear);
    addSubproperty(&m_propertyNameIndexMap, "zfar",   &m_zfar);
    addSubproperty(&m_propertyNameIndexMap, "width",  &m_width);
    addSubproperty(&m_propertyNameIndexMap, "height", &m_height);
    addSubproperty(&m_propertyNameIndexMap, "left",   &m_left);
    addSubproperty(&m_propertyNameIndexMap, "right",  &m_right);
    addSubproperty(&m_propertyNameIndexMap, "bottom", &m_bottom);
    addSubproperty(&m_propertyNameIndexMap, "top",    &m_top);

    reset();
}

#if defined P_WIN32
#pragma warning( default : 4355 )
#endif

PPropertyProjection::~PPropertyProjection()
{
    m_propertyNameIndexMap.release();
}

void PPropertyProjection::operator=(const PPropertyProjection &other)
{
    if (this != &other)
    {
        m_projection = other.m_projection;
        m_fov        = other.m_fov;
        m_aspect     = other.m_aspect;
        m_znear      = other.m_znear;
        m_zfar       = other.m_zfar;
        m_width      = other.m_width;
        m_height     = other.m_height;
        m_left       = other.m_left;
        m_right      = other.m_right;
        m_bottom     = other.m_bottom;
        m_top        = other.m_top;
        m_matrix     = other.m_matrix;
        m_dirty      = other.m_dirty;
    }
}

void PPropertyProjection::reset()
{
    perspective(45.0f, 1.0f, 0.1f, 1000.0f);
}

void PPropertyProjection::setValue(const PAbstractProperty &property)
{
    if (this != &property)
    {
        PASSERT(property.type() == P_PROPERTY_PROJECTION);
        const PPropertyProjection *p = (const PPropertyProjection*)&property;

        m_projection = p->projection();
        m_fov    = p->fov();
        m_aspect = p->aspect();
        m_znear  = p->zNear();
        m_zfar   = p->zFar(); 
        m_width  = p->width();
        m_height = p->height();
        m_left   = p->left();
        m_right  = p->right();
        m_bottom = p->bottom();
        m_top    = p->top();
        
        m_matrix = p->m_matrix;

        m_dirty = p->m_dirty;
    }
}

void PPropertyProjection::setValue(const void *value)
{
    PASSERT_NOTIMPLEMENTED();
}

void PPropertyProjection::setValue(const PVariant &value)
{
    PASSERT_NOTIMPLEMENTED();
}
    
void PPropertyProjection::interpolate(pfloat32 t, PAbstractProperty *a, PAbstractProperty *b)
{
    PASSERT_NOTIMPLEMENTED();
}

void PPropertyProjection::perspective(pfloat32 fov, pfloat32 aspect, pfloat32 znear, pfloat32 zfar)
{
    m_projection = P_PROJECTION_PERSPECTIVE;

    m_fov    = fov;
    m_aspect = aspect;
    m_znear  = znear; 
    m_zfar   = zfar; 

    perspectivePrivate(fov, aspect, znear, zfar);
    
    m_dirty = false;
}

void PPropertyProjection::orthogonal(pfloat32 aspect, pfloat32 znear, pfloat32 zfar)
{
    m_projection = P_PROJECTION_ORTHOGONAL;

    m_aspect = aspect;
    m_znear  = znear; 
    m_zfar   = zfar; 

    orthogonalPrivate(aspect, znear, zfar);
    
    m_dirty = false;
}

void PPropertyProjection::window(pfloat32 width, pfloat32 height)
{
    m_projection = P_PROJECTION_WINDOW;

    m_width  = width;
    m_height = height; 

    windowPrivate(width, height);

    m_dirty = false;
}

void PPropertyProjection::frustum(pfloat32 left, pfloat32 right, pfloat32 bottom, 
    pfloat32 top, pfloat32 znear, pfloat32 zfar)
{
    m_projection = P_PROJECTION_FRUSTUM;

    m_znear  = znear; 
    m_zfar   = zfar; 
    m_left   = left;
    m_right  = right;
    m_bottom = bottom;
    m_top    = top;

    frustumPrivate(left, right, bottom, top, znear, zfar);

    m_dirty = false;
}

void PPropertyProjection::orthogonal(pfloat32 left, pfloat32 right, pfloat32 bottom, 
    pfloat32 top, pfloat32 znear, pfloat32 zfar)
{
    m_projection = P_PROJECTION_ORTHOGONAL2;

    m_znear  = znear; 
    m_zfar   = zfar; 
    m_left   = left;
    m_right  = right;
    m_bottom = bottom;
    m_top    = top;

    orthogonalPrivate(left, right, bottom, top, znear, zfar);

    m_dirty = false;
}

pfloat32 PPropertyProjection::aspect() const
{
    return m_aspect.toFloat();
}

pfloat32 PPropertyProjection::fov() const
{
    return m_fov.toFloat();
}

pfloat32 PPropertyProjection::zNear() const
{
    return m_znear.toFloat();
}

pfloat32 PPropertyProjection::zFar() const
{
    return m_zfar.toFloat();
}

pfloat32 PPropertyProjection::width() const
{
    return m_width.toFloat();
}

pfloat32 PPropertyProjection::height() const
{
    return m_height.toFloat();
}

pfloat32 PPropertyProjection::left() const
{
    return m_left.toFloat();
}
pfloat32 PPropertyProjection::right() const
{
    return m_right.toFloat();
}
pfloat32 PPropertyProjection::bottom() const
{
    return m_bottom.toFloat();
}
pfloat32 PPropertyProjection::top() const
{
    return m_top.toFloat();
}

const PMatrix4x4 & PPropertyProjection::toMatrix4x4() const
{
    if (m_dirty)
    {
        switch (m_projection)
        {
            case P_PROJECTION_ORTHOGONAL:
                orthogonalPrivate(m_aspect.toFloat(), m_znear.toFloat(), m_zfar.toFloat());
                break;
            case P_PROJECTION_ORTHOGONAL2:
                orthogonalPrivate(m_left.toFloat(), m_right.toFloat(), m_bottom.toFloat(), m_top.toFloat(), m_znear.toFloat(), m_zfar.toFloat());
                break;
            case P_PROJECTION_PERSPECTIVE:
                perspectivePrivate(m_fov.toFloat(), m_aspect.toFloat(), m_znear.toFloat(), m_zfar.toFloat());
                break;
            case P_PROJECTION_WINDOW:
                windowPrivate(m_width.toFloat(), m_height.toFloat());
                break;
            case P_PROJECTION_FRUSTUM:
                frustumPrivate(m_left.toFloat(), m_right.toFloat(), m_bottom.toFloat(), m_top.toFloat(), m_znear.toFloat(), m_zfar.toFloat());
                break;
            default:
                break;
        }

        m_dirty = false;
    }

    return m_matrix;
}

pbool PPropertyProjection::unpack(const pchar *value)
{
    const pchar *p = value;
    pchar projection[16];
    pfloat32 fov;
    pfloat32 aspect;
    pfloat32 znear;
    pfloat32 zfar;
    pfloat32 width;
    pfloat32 height;

    if ((p = pStringUnpackString(p, projection)) != P_NULL) 
    {
        if (pstrcmp(projection, "orthogonal") == 0)
        {
            if ((p = pStringUnpackTrimAnnotation(p)) != P_NULL &&
                (p = pStringUnpackFloat(p, &aspect)) != P_NULL &&
                (p = pStringUnpackTrimAnnotation(p)) != P_NULL &&
                (p = pStringUnpackFloat(p, &znear)) != P_NULL &&
                (p = pStringUnpackTrimAnnotation(p)) != P_NULL &&
                (p = pStringUnpackFloat(p, &zfar)) != P_NULL)
            {
                m_projection = P_PROJECTION_ORTHOGONAL;

                m_aspect     = aspect;
                m_znear      = znear;
                m_zfar       = zfar;
                m_dirty      = false;

                orthogonalPrivate(aspect, znear, zfar);

                return true;
            }
        }
        else if (pstrcmp(projection, "perspective") == 0)
        {
            if ((p = pStringUnpackTrimAnnotation(p)) != P_NULL &&
                (p = pStringUnpackFloat(p, &fov)) != P_NULL &&
                (p = pStringUnpackTrimAnnotation(p)) != P_NULL &&
                (p = pStringUnpackFloat(p, &aspect)) != P_NULL &&
                (p = pStringUnpackTrimAnnotation(p)) != P_NULL &&
                (p = pStringUnpackFloat(p, &znear)) != P_NULL &&
                (p = pStringUnpackTrimAnnotation(p)) != P_NULL &&
                (p = pStringUnpackFloat(p, &zfar)) != P_NULL)
            {
                m_projection = P_PROJECTION_PERSPECTIVE;

                m_fov        = fov;
                m_aspect     = aspect;
                m_znear      = znear;
                m_zfar       = zfar;
                m_dirty      = false;

                perspectivePrivate(fov, aspect, znear, zfar);

                return true;
            }
        }
        else if (pstrcmp(projection, "window") == 0)
        {
            if ((p = pStringUnpackTrimAnnotation(p)) != P_NULL &&
                (p = pStringUnpackFloat(p, &width)) != P_NULL &&
                (p = pStringUnpackTrimAnnotation(p)) != P_NULL &&
                (p = pStringUnpackFloat(p, &height)) != P_NULL)
            {
                m_projection = P_PROJECTION_WINDOW;

                m_width      = width;
                m_height     = height;
                m_dirty      = false;

                windowPrivate(width, height);

                return true;
            }
        }
        else if (pstrcmp(projection, "frustum") == 0)
        {
            PASSERT_NOTIMPLEMENTED();
        }
        else if (pstrcmp(projection, "orthogonal2") == 0)
        {
            PASSERT_NOTIMPLEMENTED();
        }
        else
        {
            PASSERTINFO(P_NULL, "Unknown projection type in this projection property");
            return false;
        }

    }

    PLOG_ERROR("Failed to unpack a projection property called %s", name());

    return false;
}
    
PMatrix4x4 PPropertyProjection::toInversedMatrix4x4() const
{
    PMatrix4x4 ret;
    switch (m_projection)
    {
        case P_PROJECTION_WINDOW:
            pMatrix4x4InversedOrthographicProjection(0, 
                                                     m_width.toFloat(), 
                                                     0, 
                                                     m_height.toFloat(), 
                                                     -100.0f, 
                                                     100.0f,
                                                     ret.m_m);
        case P_PROJECTION_ORTHOGONAL:
            pMatrix4x4InversedOrthographicProjection(-m_aspect.toFloat(), 
                                                     m_aspect.toFloat(), 
                                                     -1.0f, 
                                                     1.0f, 
                                                     m_znear.toFloat(), 
                                                     m_zfar.toFloat(),
                                                     ret.m_m);
            break;
        case P_PROJECTION_ORTHOGONAL2:
            pMatrix4x4InversedOrthographicProjection(m_left.toFloat(), 
                                                     m_right.toFloat(), 
                                                     m_bottom.toFloat(),
                                                     m_top.toFloat(),
                                                     m_znear.toFloat(), 
                                                     m_zfar.toFloat(),
                                                     ret.m_m);
            break;
        case P_PROJECTION_PERSPECTIVE:
            pMatrix4x4InversedPerspectiveProjection(m_fov.toFloat(), 
                                                    m_aspect.toFloat(), 
                                                    m_znear.toFloat(), 
                                                    m_zfar.toFloat(),
                                                    ret.m_m);
            break;
        case P_PROJECTION_FRUSTUM:
        default:
            PASSERT_NOTIMPLEMENTED();
            pMatrix4x4Identity(ret.m_m);
            break;
    }

    return ret;
}

void PPropertyProjection::setMatrix4x4(const pfloat32 *matrix)
{
    m_dirty = false;
    m_matrix.copy(matrix);
    m_projection = P_PROJECTION_USER;
}

void PPropertyProjection::operator=(const PMatrix4x4 &matrix)
{
    setMatrix4x4(matrix.m_m);
}

PPropertyNameIndexMap* PPropertyProjection::propertyNameIndexMap() const
{
    return &m_propertyNameIndexMap;
}

void PPropertyProjection::perspectivePrivate(pfloat32 fov, pfloat32 aspect, pfloat32 znear, pfloat32 zfar) const
{
    pMatrix4x4PerspectiveProjection(fov, aspect, znear, zfar, m_matrix.m_m);
}

void PPropertyProjection::orthogonalPrivate(pfloat32 aspect, pfloat32 znear, pfloat32 zfar) const
{
    pMatrix4x4OrthographicProjection(-aspect, aspect, -1.0f, 1.0f, znear, zfar, m_matrix.m_m);
}

void PPropertyProjection::windowPrivate(pfloat32 width, pfloat32 height) const
{
    pMatrix4x4OrthographicProjection(0, width, 0, height, -100.0f, 100.0f, m_matrix.m_m);
}

void PPropertyProjection::frustumPrivate(pfloat32 left, pfloat32 right, pfloat32 bottom, pfloat32 top, pfloat32 znear, pfloat32 zfar) const
{
    PASSERT_NOTIMPLEMENTED();    
}

void PPropertyProjection::orthogonalPrivate(pfloat32 left, pfloat32 right, pfloat32 bottom, pfloat32 top, pfloat32 znear, pfloat32 zfar) const
{
    pMatrix4x4OrthographicProjection(left, right, bottom, top, znear, zfar, m_matrix.m_m);
}

