// pplane.cpp
// a 3D plane
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pplane.h"

#include <PFoundation/pvector3.h>
#include <PFoundation/pline.h>
#include <PFoundation/pmatrix4x4.h>
#include <PFoundation/pmatrix3x3.h>

#include <PFoundation/passert.h>


PPlane::PPlane(const pfloat32 *p0, const pfloat32 *p1, const pfloat32 *p2)
{
    pfloat32 e10[3];
    pfloat32 e20[3];

    pVector3Subtract(p1, p0, e10);
    pVector3Subtract(p2, p0, e20);

    pVector3Cross(e10, e20, m_N.m_v);
    m_N.normalize();

    m_D = -pVector3Dot(m_N.m_v, p0);
}

PPlane::PPlane(pfloat32 A, pfloat32 B, pfloat32 C, pfloat32 D)
{
    m_N[0] = A;
    m_N[1] = B;
    m_N[2] = C;
    m_D    = D;

    pfloat32 n = m_N.length();

    m_N[0] /= n;
    m_N[1] /= n;
    m_N[2] /= n;
    m_D    /= n;
}
    
pbool PPlane::intersect(const PLine &line, PVector3 *out_point) const
{
    // The distance from a point on a line to the intersection of the line with the plane.
    pfloat32 dotProduct = line.getDirection().dot(normal()); 
    if (pAbs(dotProduct) > 1e-5f)
    {
        PVector3 point = line.getAnyPoint();
        pfloat32 d = (anyPoint() - point).dot(normal()) / dotProduct;
        *out_point = point + line.getDirection() * d;
        return true;
    }

    // Line is parallel to the plane.
    return false;
    
}

pbool PPlane::intersect(const PPlane &other, PLine *out_line) const
{
    PASSERT_NOTIMPLEMENTED();
    return false;
}

pfloat32 PPlane::distance(const pfloat32 *point) const
{
    return pVector3Dot(m_N.m_v, point) + m_D;
}
    
PVector3 PPlane::anyPoint() const
{
    return m_N * -m_D;
}
    
void PPlane::transform(const PMatrix4x4 &transform)
{
    pfloat32 normalMatrix[9];
    pMatrix3x3InverseTransposeMatrix4x4(transform.m_m, normalMatrix);

    pfloat32 n[3];
    pMatrix3x3MultiplyVector3(normalMatrix, m_N.m_v, n);

    pfloat32 d[] = 
    {
        m_N[0] * -m_D,
        m_N[1] * -m_D,
        m_N[2] * -m_D,
        1.0f,
    };

    pfloat32 td[4];
    pMatrix4x4MultiplyVector4(transform.m_m, d, td);

    m_N[0] = n[0];
    m_N[1] = n[1];
    m_N[2] = n[2];
    m_D = -pVector3Dot(m_N.m_v, td);

    // Normalize. 
    pfloat32 l = pVector3Length(m_N.m_v);
    m_N[0] /= l;
    m_N[1] /= l;
    m_N[2] /= l;

    m_D /= l;
}
