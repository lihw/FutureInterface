// psprite2d.cpp
// A sprite in the camera space. It is not affected by either its
// parent's transformation or camera transformation.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "psprite2d.h"

P_OBJECT_DEFINE(PSprite2D)

PSprite2D::PSprite2D(const pchar *name, PScene *scene)
    : PSprite(name, scene)
{
    P_OBJECT_INITIALIZE_PROPERTY(PSprite)
}

PSprite2D::~PSprite2D()
{
    P_OBJECT_UNINITIALIZE_PROPERTY();
}

void PSprite2D::calculateModelCameraMatrix(const PMatrix4x4 &cameraMatrix, 
        pfloat32 *out_matrix)
{

    pMatrix4x4Identity(out_matrix);
    
    out_matrix[0] = m_localTransform.scaling()[0];
    out_matrix[5] = m_localTransform.scaling()[1];
    out_matrix[10] = m_localTransform.scaling()[2];

    out_matrix[12] = m_localTransform.translation()[0];
    out_matrix[13] = m_localTransform.translation()[1];
    out_matrix[14] = m_localTransform.translation()[2];
}

pbool PSprite2D::unpack(const PXmlElement* xmlElement)
{
    return PSprite::unpack(xmlElement);
}
