// parcball.cpp
// To rotate the scene based on the mouse input in the screen.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//



#include "parcball.h"


PArcball::PArcball()
{
    m_moving = false;

    m_rotationMatrix.identity();
}
    
PArcball::~PArcball()
{
}

void PArcball::restart()
{
    m_moving = false;

    m_lastRotation = m_rotation;
}

void PArcball::updateMouse(const pfloat32 x, const pfloat32 y)
{
    if (!m_moving)
    {
        mapToSphere(x, y, &m_lastArcballCoordinate.m_v[0]);
        m_moving = true;
    }
    else
    {
        mapToSphere(x, y, &m_arcballCoordinate[0]);

        PVector3 perp;
        pVector3Cross(&m_lastArcballCoordinate.m_v[0], 
                      &m_arcballCoordinate.m_v[0], 
                      &perp.m_v[0]);

        // Compute the rotating quaternion
        pfloat32 l = perp.length();

        pfloat32 quat[4];

        if (l > 1e-4f) 
        {
            quat[0] = perp[0];
            quat[1] = perp[1];
            quat[2] = perp[2];
            quat[3] = m_lastArcballCoordinate.dot(m_arcballCoordinate); 
        } 
        else 
        {
            quat[0] = quat[1] = quat[2] = quat[3] = 0.0f;
        }

        // Accumulate the rotation.
        pQuaternionMultiply(quat, &m_lastRotation.m_q[0], &m_rotation.m_q[0]);
        
        pQuaternionGetMatrix3x3(&m_rotation.m_q[0], &m_rotationMatrix.m_m[0]);
    }
}

const pfloat32* PArcball::getRotationMatrixF() const
{
    return &m_rotationMatrix.m_m[0];
}

const PMatrix3x3 &PArcball::getRotationMatrix() const
{
    return m_rotationMatrix;
}

void PArcball::mapToSphere(const pfloat32 x, const pfloat32 y, pfloat32* coordinate)
{
    coordinate[0] = (pfloat32)x;
    coordinate[1] = (pfloat32)y;

    pfloat32 l = coordinate[0] * coordinate[0] + coordinate[1] * coordinate[1];

    if (l > 1.0f) 
    {
        pfloat32 inv = 1.0f / sqrt(l);
        coordinate[0] *= inv;
        coordinate[1] *= inv;
        coordinate[2] = 0.0f;
    } 
    else 
    {
        coordinate[2] = sqrt(1.0f - l);
    }
}

