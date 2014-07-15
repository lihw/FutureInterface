// pline.h 
// The 3D line
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PLINE_H
#define PLINE_H

#include <PFoundation/pvector3.h>

class P_DLLEXPORT PLine
{
public:
    PLine(const pfloat32 *points[2]);
    PLine(const pfloat32 *point, const pfloat32 *direction);

    P_INLINE PVector3 getAnyPoint() const
    { return m_point + m_direction * 2.0f; }
    P_INLINE PVector3 getDirection() const
    { return m_direction; }

private:
    PVector3 m_point;
    PVector3 m_direction;
};


#endif  // !PLINE_H
