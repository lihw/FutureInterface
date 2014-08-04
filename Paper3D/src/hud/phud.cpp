// phud.cpp
// The layer tha display and manages all 2D controls
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "phud.h"


#include <Paper3D/puicontrol.h>
#include <Paper3D/phudpick.h>


PHUD::PHUD(const puint32 *rect)
{
    PASSERT(rect != P_NULL);
    m_rect[0] = rect[0];
    m_rect[1] = rect[1];
    m_rect[2] = rect[2];
    m_rect[3] = rect[3];

    m_pick = PNEW(PHUDPick(this));
}

PHUD::~PHUD()
{
    PDELETE(m_pick);

    PArray<PUIControl *>::iterator it = m_controls.begin();
    PArray<PUIControl *>::iterator ie = m_controls.end();
    while (it != ie)
    {
        PDELETE(*it);
        ++it;
    }
}

void PHUD::touchDown(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
{
}

void PHUD::touchMove(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
{
}

void PHUD::touchUp(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
{
}

void PHUD::render(PRenderState *renderState)
{
}
    
void PHUD::addControl(PUIControl *control)
{
    PArray<PUIControl *>::iterator it = m_controls.find(control);
    if (it != ie)
    {
        PLOG_WARNING("Same control has been added to HUD already.");
        return ;
    }
    m_controls.append(control);
    for (puint32 i = m_controls.size() - 1; i >= 0; --i)
    {
    }

    m_pick->addControl(control);
}

void PHUD::removeControl(PUIControl *control)
{
    PArray<PUIControl *>::iterator it = m_controls.find(control);
    if (it == ie)
    {
        PLOG_WARNING("Failed to find such control in HUD.");
        return ;
    }
    m_controls.erase(it);
    m_pick->removeControl(control);
}

