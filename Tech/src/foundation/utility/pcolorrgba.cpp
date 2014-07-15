// pcolorrgba.cpp
// The RGBA color structure
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pcolorrgba.h"


const PColorRGBA P_COLOR_BLACK_TRANSPARENT = {0, 0, 0, 0};
const PColorRGBA P_COLOR_BLACK             = {0, 0, 0, 255};
const PColorRGBA P_COLOR_DARK_GRAY         = {50, 50, 50, 255};
const PColorRGBA P_COLOR_GRAY              = {100, 100, 100, 255};
const PColorRGBA P_COLOR_WHITE             = {255, 255, 255, 255};
const PColorRGBA P_COLOR_RED               = {255, 0, 0, 255};
const PColorRGBA P_COLOR_GREEN             = {0, 255, 0, 255}; 
const PColorRGBA P_COLOR_BLUE              = {0, 0, 255, 255}; 
const PColorRGBA P_COLOR_YELLOW            = {255, 255, 0, 255}; 
const PColorRGBA P_COLOR_ORANGE            = {255, 127, 0, 255}; 
const PColorRGBA P_COLOR_LIME              = {127, 255, 0, 255};
const PColorRGBA P_COLOR_TEAL              = {0, 255, 255, 255}; 
const PColorRGBA P_COLOR_PURPLE            = {255, 0, 255, 255}; 
const PColorRGBA P_COLOR_PINK              = {255, 0, 127, 255}; 
const PColorRGBA P_COLOR_CYAN              = {0, 255, 255, 255}; 

#define FLOAT32_TO_UINT8(a) ((puint8)((a) * 255.0f))

pfloat32 PColorRGBA::getRFloat() const
{
    return (pfloat32)m_rgba[0] * (1.0f / 255.0f);
}

pfloat32 PColorRGBA::getGFloat() const
{
    return (pfloat32)m_rgba[1] * (1.0f / 255.0f);
}

pfloat32 PColorRGBA::getBFloat() const
{
    return (pfloat32)m_rgba[2] * (1.0f / 255.0f);
}

pfloat32 PColorRGBA::getAFloat() const
{
    return (pfloat32)m_rgba[3] * (1.0f / 255.0f);
}

void PColorRGBA::setR(puint8 red)
{
    m_rgba[0] = red;
}

void PColorRGBA::setG(puint8 green)
{
    m_rgba[1] = green;
}

void PColorRGBA::setB(puint8 blue)
{
    m_rgba[2] = blue;
}

void PColorRGBA::setA(puint8 alpha)
{
    m_rgba[3] = alpha;
}

void PColorRGBA::setRFloat(pfloat32 red)
{
    m_rgba[0] = FLOAT32_TO_UINT8(red);
}

void PColorRGBA::setGFloat(pfloat32 green)
{
    m_rgba[1] = FLOAT32_TO_UINT8(green);
}

void PColorRGBA::setBFloat(pfloat32 blue)
{
    m_rgba[2] = FLOAT32_TO_UINT8(blue);
}

void PColorRGBA::setAFloat(pfloat32 alpha)
{
    m_rgba[3] = FLOAT32_TO_UINT8(alpha);
}

void PColorRGBA::getRGBA(puint8* c) const
{
    c[0] = m_rgba[0];
    c[1] = m_rgba[1];
    c[2] = m_rgba[2];
    c[3] = m_rgba[3];
}

void PColorRGBA::getRGBAFloat(pfloat32* c) const
{
    c[0] = (pfloat32)m_rgba[0] * (1.0f / 255.0f);
    c[1] = (pfloat32)m_rgba[1] * (1.0f / 255.0f);
    c[2] = (pfloat32)m_rgba[2] * (1.0f / 255.0f);
    c[3] = (pfloat32)m_rgba[3] * (1.0f / 255.0f);
}

void PColorRGBA::setRGBA(const puint8* c)
{
    m_rgba[0] = c[0];
    m_rgba[1] = c[1];
    m_rgba[2] = c[2];
    m_rgba[3] = c[3];
}

void PColorRGBA::setRGBAFloat(const pfloat32* c)
{
    m_rgba[0] = FLOAT32_TO_UINT8(c[0]);
    m_rgba[1] = FLOAT32_TO_UINT8(c[1]);
    m_rgba[2] = FLOAT32_TO_UINT8(c[2]);
    m_rgba[3] = FLOAT32_TO_UINT8(c[3]);
}
    
void PColorRGBA::setRGBA(puint8 red, puint8 green, puint8 blue, puint8 alpha)
{
    m_rgba[0] = red;
    m_rgba[1] = green;
    m_rgba[2] = blue;
    m_rgba[3] = alpha;
}

void PColorRGBA::setRGBAFloat(pfloat32 red, pfloat32 green, pfloat32 blue, pfloat32 alpha)
{
    m_rgba[0] = FLOAT32_TO_UINT8(red);
    m_rgba[1] = FLOAT32_TO_UINT8(green);
    m_rgba[2] = FLOAT32_TO_UINT8(blue);
    m_rgba[3] = FLOAT32_TO_UINT8(alpha);
}

const puint8* PColorRGBA::getRGBA() const
{
    return m_rgba;
}

puint8* PColorRGBA::getRGBA()
{
    return m_rgba;
}

PVector4 PColorRGBA::toVector4() const
{
    return pVector4(getRFloat(), getGFloat(), getBFloat(), getAFloat());
}

#undef FLOAT32_TO_UINT8
