// pgesturefling.cpp
// The gesture fling file.
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

PGestureFling::PGestureFling(PGestureManager* manager, pfloat32 distanceThreshold, pfloat32 velocityThreshold)
    : PAbstractGesture(manager, P_GESTURE_TYPE_FLING)
{
    reset();

    m_distanceThreshold = distanceThreshold;
    m_velocityThreshold = velocityThreshold;
}

PGestureFling::~PGestureFling()
{
}

void PGestureFling::setDistanceThreshold(pfloat32 value)
{
    m_distanceThreshold = value;
}

void PGestureFling::setVelocityThreshold(pfloat32 value)
{
    m_velocityThreshold = value;
}
void PGestureFling::touchDown(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
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

    m_state = STATE_POSSIBLE;

    m_initialPoint[0] = x;
    m_initialPoint[1] = y;
    m_initialTimestamp = timeStamp;
}

void PGestureFling::touchMove(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
{
    if (!m_enabled)
    {
        return ;
    }
    
    if (m_state == STATE_POSSIBLE && pointer > 0)
    {
        reset();

        return;
    }
}

void PGestureFling::touchUp (pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
{
    if (!m_enabled)
    {
        return ;
    }
    // if user touch down two fingers , m_state is changed to STATE_READY when second pointer is down.
    // then touch up first fingers, here must directly and not send fling event.
    if (m_state == STATE_READY || (m_state == STATE_POSSIBLE && pointer > 0))
    {
        reset();

        return;
    }

    pfloat32 dx = (pfloat32)x - (pfloat32)m_initialPoint[0];
    pfloat32 dy = (pfloat32)y - (pfloat32)m_initialPoint[1];
    pfloat32 deltaTime = (pfloat32)timeStamp - (pfloat32)m_initialTimestamp;
    // test on win32 platform ,find sometimes this delta is zero.
    // if  is zero ,return directly.
    if (deltaTime <= 0)
    {
        m_state = STATE_READY;
        return;
    }
    
   // transform pixels/ms to pixels/s
    pfloat32 vx = 1000 * dx / deltaTime;
    pfloat32 vy = 1000 * dy / deltaTime;
    pfloat32 vSq = vx * vx + vy * vy;
    if (dx * dx + dy * dy > m_distanceThreshold * m_distanceThreshold &&
        vSq > m_velocityThreshold * m_velocityThreshold)
    {
        PEvent* event = createEvent(P_EVENT__FLING);
        event->setParameter(P_EVENTPARAMETER__FLING_VELOCITY_X, vx);
        event->setParameter(P_EVENTPARAMETER__FLING_VELOCITY_Y, vy);
        event->queue(reinterpret_cast<PObject *>(P_NULL));
    }

    m_state = STATE_READY;
}

void PGestureFling::reset()
{
    m_state = STATE_READY; 
}

