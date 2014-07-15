// pscrollersnap.cpp
// An scrolling controller with snapping. When snapping is enabled, the scroll 
// should always stop at the snapping stops. 
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com 
//

#include "pscrollersnap.h"

#include <PFoundation/pdebugutility.h>

#include <math.h>


PScrollerSnap::PScrollerSnap()
    : PScroller()
{
    m_snapping = true;

    m_snappingStop = 1.0f;
}

void PScrollerSnap::startScroll(pfloat32 startPosition, puint32 startTime, 
        pfloat32 distance, puint32 duration)
{
    if (m_snapping)
    {
        pfloat32 finishPosition = startPosition + distance;
        if (finishPosition > 0)
        {
            finishPosition = floor(finishPosition / m_snappingStop + 0.5f) * m_snappingStop;
        }
        else
        {
            finishPosition = floor(finishPosition / m_snappingStop - 0.5f) * m_snappingStop;
        }
        
        distance = finishPosition - startPosition;
    }

    return PScroller::startScroll(startPosition, startTime, distance, duration);
}

void PScrollerSnap::enableSnapping(pbool flag)
{
    m_snapping = flag;
}

bool PScrollerSnap::isSnappingEnabled() const
{
    return m_snapping;
}

void PScrollerSnap::setSnappingStop(pfloat32 stop)
{
    if (stop > 0)
    {
        m_snappingStop = stop;
    }
}
