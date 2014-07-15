// pbackground.h
// Background is the plane at the very back of the scene. It renders
// a full screen quad with a texture.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PBACKGROUND_H
#define PBACKGROUND_H

#include <Paper3D/pdrawable.h>

class PTexture;

class P_DLLEXPORT PBackground : public PDrawable
{
    P_OBJECT

private:
    PBackground(const PBackground &other) : PDrawable(P_NULL, (PScene *)P_NULL) {}
    void operator=(const PBackground &other) {}

public:
    PBackground(const pchar *name, PScene *scene);
    virtual ~PBackground();

    enum PBackgroundLayoutEnum
    {
        LAYOUT_TOP     = 0x1,
        LAYOUT_MIDDLE  = 0x2,
        LAYOUT_BOTTOM  = 0x3,
        
        LAYOUT_LEFT    = 0x10,
        LAYOUT_CENTER  = 0x20,
        LAYOUT_RIGHT   = 0x30,

        LAYOUT_DEFAULT = (LAYOUT_MIDDLE | LAYOUT_CENTER),
    };

    // Set size w.r.t to the screen
    void setSize(pfloat32 width, pfloat32 height);
    // Set the layout in the screen
    void setLayout(puint32 layout);
    // The texture must be a 2D texture.
    void setTexture(PTexture *texture);
    // Translate the texture
    void setTextureOffset(pfloat32 x, pfloat32 y);
	// Set the scaling of the texture visible region.
	void setTextureScaling(pfloat32 x, pfloat32 y);
    
    virtual pbool unpack(const PXmlElement* xmlElement);

protected:
    virtual void update();
    
private:
    PTexture    *m_texture;
	PVector4     m_textureInfo;
    PVector4     m_sizeInfo;
    puint32      m_layout;
    pbool        m_dirty; // Need to sync GPU and CPU
};



#endif // !PBACKGROUND_H
