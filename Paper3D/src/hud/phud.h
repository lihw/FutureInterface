// phud.h
// The layer tha display and manages all 2D controls
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PHUD_H
#define PHUD_H

#include <PFoundation/parray.h>

class PUIControl;
class PHUDPick;

class P_DLLEXPORT PHUD
{
public:
    PHUD(const puint32 *rect);
    ~PHUD();

    void touchDown(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);
    void touchMove(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);
    void touchUp(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);

    void render(PRenderState *renderState);

    void setRelativeSize(pbool enabled);

    P_INLINE PUIImage *background() const
    { return m_background; }

private:
    void addControl(PUIControl *control);
    void removeControl(PUIControl *control);

private:
    // The region of this HUD w.r.t to the screen.
    puint32 m_rect[4];
    // The current bound scene.
    PScene *m_scene;   
    // The list of controls
    PArray<PUIControl *> m_controls;
    // The data struct that acclerates control picking.
    PHUDPick *m_pick;
};


#endif // !PHUD_H

