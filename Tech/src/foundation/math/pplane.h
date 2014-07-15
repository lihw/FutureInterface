// pplane.h 
// a 3D plane
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PPLANE_H
#define PPLANE_H

#include <PFoundation/pvector3.h>
#include <PFoundation/pvector4.h>

class PLine;
struct PMatrix4x4;


// Ax + By + Cz + D = 0
class P_DLLEXPORT PPlane
{

public:
    PPlane(const pfloat32 *p0, const pfloat32 *p1, const pfloat32 *p2);
    PPlane(pfloat32 A, pfloat32 B, pfloat32 C, pfloat32 D);
    
    pbool intersect(const PLine &line, PVector3 *out_point) const;
    pbool intersect(const PPlane &other, PLine *out_line) const;
    pfloat32 distance(const pfloat32 *point) const;
    PVector3 anyPoint() const; // Get any point on the plane.
    void transform(const PMatrix4x4 &transform);

    P_INLINE PVector3 normal() const { return m_N; }
    P_INLINE pfloat32 A() const { return m_N[0]; }
    P_INLINE pfloat32 B() const { return m_N[1]; }
    P_INLINE pfloat32 C() const { return m_N[2]; }
    P_INLINE pfloat32 D() const { return m_D; }
    P_INLINE PVector4 toVector4() const { return pVector4(m_N[0], m_N[1], m_N[2], m_D); }

    PVector3 m_N;
    pfloat32 m_D;
};

#endif // !PPLANE_H

