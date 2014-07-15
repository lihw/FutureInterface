// pbox.h 
// The axis-aligned 3d box
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PBOX_H
#define PBOX_H

#include <PFoundation/pglobal.h>

#include <PFoundation/pvector3.h>

class P_DLLEXPORT PBox
{
public:
    PBox();
    PBox(const pfloat32 *min, const pfloat32 *max);
    ~PBox();
    P_INLINE pfloat32 getWidth() const { return m_max[0] - m_min[0]; }
    P_INLINE pfloat32 getHeight() const { return m_max[1] - m_min[1]; }
    P_INLINE pfloat32 getDepth() const { return m_max[2] - m_min[2]; }
    P_INLINE const PVector3 getMax() const { return pVector3(m_max[0], m_max[1], m_max[2]); }
    P_INLINE const PVector3 getMin() const { return pVector3(m_min[0], m_min[1], m_min[2]); }
    

    // THe union of two boxes.
    PBox operator+(const PBox &other) const;
    const PBox &operator+=(const PBox &other);
    // The intersection of two boxes
    PBox operator^(const PBox &other) const;
    const PBox &operator^=(const PBox &other) ;

public:
    pfloat32 m_min[3];
    pfloat32 m_max[3];
};

#endif // !PBOX_H
