// pgesturetap.cpp
// The gesture tap file.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "pgesturemanager.h"
#include "pgesture_private.h"

#include <PFoundation/pgesturemanager.h>
#include <PFoundation/pevent.h>
#include <PFoundation/peventtype.h>
#include <PFoundation/PContext.h>

PGestureTap::PGestureTap(PGestureManager* manager, pfloat32 distanceThreshold, 
    puint32 intervalThreshold)
    : PAbstractGesture(manager, P_GESTURE_TYPE_TAP)
{
    reset();

    m_distanceThreshold = distanceThreshold;
    m_intervalThreshold = intervalThreshold;
    m_tapCount = 0;
}

PGestureTap::~PGestureTap()
{
}

void PGestureTap::touchDown(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
{
    if (!m_enabled)
    {
        return ;
    }

    if (pointer > 0 && m_state == STATE_POSSIBLE)
    {
        reset();

        PLOG_WARNING("Gesture tap support only one touch");
        return;
    }
    
    if (m_state == STATE_READY)
    {
        m_firstPoint[0] = x;
        m_firstPoint[1] = y;
        m_firstTapTime = timeStamp;

        m_state = STATE_POSSIBLE;
    }
}

void PGestureTap::touchMove (pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
{
    if (!m_enabled)
    {
        return ;
    }

    if (pointer > 0 && m_state == STATE_POSSIBLE)
    {
        reset(); 

        return;
    }
}

void PGestureTap::touchUp (pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
{
    if (!m_enabled)
    {
        return ;
    }

    if (pointer > 0 && m_state == STATE_POSSIBLE)
    {
        reset();

        return;
    }

    if (m_state == STATE_POSSIBLE)
    {
        puint32 deltaTime = timeStamp - m_firstTapTime;
        pfloat32 dx = (pfloat32)x - (pfloat32)m_firstPoint[0];
        pfloat32 dy = (pfloat32)y - (pfloat32)m_firstPoint[1];
        
        pfloat32 dThresholdSquare = m_distanceThreshold * m_distanceThreshold;
        
        if (deltaTime < m_intervalThreshold &&
            dx * dx + dy * dy < dThresholdSquare)
        {
            m_tapCount++;
            PEvent* event = createEvent(P_EVENT__TAP);
            event->setParameter(P_EVENTPARAMETER__TOUCH_X, x);
            event->setParameter(P_EVENTPARAMETER__TOUCH_Y, y);
            event->setParameter(P_EVENTPARAMETER__TAPCOUNT, (pint32)m_tapCount);
            
            event->queue(reinterpret_cast<PObject *>(P_NULL));
        }
        else
        {
            m_tapCount = 0;
            reset();
        }
    }
}

void PGestureTap::reset()
{
    m_state = STATE_READY;
}

void PGestureTap::setDistanceThreshold(pfloat32 value)
{
    m_distanceThreshold = value;
}

void PGestureTap::setIntervalThreshold(puint32 value)
{
    m_intervalThreshold = value;
}

