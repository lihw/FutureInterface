// pline.h 
// The 3D line
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "pline.h"


PLine::PLine(const pfloat32 *point, const pfloat32 *direction)
{
    m_point = pVector3(point[0], point[1], point[2]);

    m_direction = pVector3(direction[0], direction[1], direction[2]);
    m_direction.normalize();
}


PLine::PLine(const pfloat32 *points[2])
{
    m_point = pVector3(points[0][0], points[0][1], points[0][2]);
    pVector3Subtract(points[1], points[0], m_direction.m_v);
    m_direction.normalize();
}

