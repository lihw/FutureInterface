// pcolorrgba.h
// The RGBA color structure
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PCOLORRGBA_H
#define PCOLORRGBA_H

#include <PFoundation/pvector4.h>


// TODO: 
// - HSV support
// - HSV <-> RGB conversion
// - store float inside PColorRGBA instead of uint8
// - RGB <-> PVector4 conversion


struct P_DLLEXPORT PColorRGBA 
{
    // Get color component.
    P_INLINE puint8 r() const { return m_rgba[0]; }
    P_INLINE puint8 g() const { return m_rgba[1]; }
    P_INLINE puint8 b() const { return m_rgba[2]; }
    P_INLINE puint8 a() const { return m_rgba[3]; }
    
    P_INLINE puint8 &r() { return m_rgba[0]; }
    P_INLINE puint8 &g() { return m_rgba[1]; }
    P_INLINE puint8 &b() { return m_rgba[2]; }
    P_INLINE puint8 &a() { return m_rgba[3]; }

    pfloat32 getRFloat() const;
    pfloat32 getGFloat() const;
    pfloat32 getBFloat() const;
    pfloat32 getAFloat() const;

    PVector4 toVector4() const;

    // Set the color component.
    void setR(puint8 r);
    void setG(puint8 g);
    void setB(puint8 b);
    void setA(puint8 a);
    
    void setRFloat(pfloat32 r);
    void setGFloat(pfloat32 g);
    void setBFloat(pfloat32 b);
    void setAFloat(pfloat32 a);

    // Get the whole color value
    void getRGBA(puint8* c) const;
    void getRGBAFloat(pfloat32* c) const;
    const puint8* getRGBA() const;
    puint8* getRGBA();

    // Set the whole color value.
    void setRGBA(const puint8* c);
    void setRGBAFloat(const pfloat32* c);
    void setRGBA(puint8 red, puint8 green, puint8 blue, puint8 alpha);
    void setRGBAFloat(pfloat32 red, pfloat32 green, pfloat32 blue, pfloat32 alpha);

    puint8 m_rgba[4];  
};

P_INLINE PColorRGBA P_APIENTRY pColorRGBA(pfloat32 r, pfloat32 g, pfloat32 b, pfloat32 a)
{
    PColorRGBA ret;
    ret.setRGBAFloat(r, g, b, a);
    return ret;
}

P_INLINE PColorRGBA P_APIENTRY pColorRGBA(puint8 r, puint8 g, puint8 b, puint8 a)
{
    PColorRGBA ret = {r, g, b, a};
    return ret;
}

extern const PColorRGBA P_COLOR_BLACK_TRANSPARENT;
extern const PColorRGBA P_COLOR_BLACK;
extern const PColorRGBA P_COLOR_DARK_GRAY;
extern const PColorRGBA P_COLOR_GRAY;
extern const PColorRGBA P_COLOR_WHITE;
extern const PColorRGBA P_COLOR_RED;
extern const PColorRGBA P_COLOR_GREEN;
extern const PColorRGBA P_COLOR_BLUE;
extern const PColorRGBA P_COLOR_YELLOW;
extern const PColorRGBA P_COLOR_ORANGE;
extern const PColorRGBA P_COLOR_LIME;
extern const PColorRGBA P_COLOR_TEAL;
extern const PColorRGBA P_COLOR_PURPLE;
extern const PColorRGBA P_COLOR_PINK;
extern const PColorRGBA P_COLOR_CYAN;



#endif // !PCOLORRGBA_H

