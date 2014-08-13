// pgesturelongpress.cpp
// The gesture pan file.
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

PGestureLongPress::PGestureLongPress(PGestureManager *manager, pfloat32 distanceThreshold, 
    puint32 intervalThreshold)
    : PAbstractGesture(manager, P_GESTURE_TYPE_LONGPRESS)
{
    reset();
    
    m_distanceThreshold = distanceThreshold;
    m_intervalThreshold = intervalThreshold;
}

PGestureLongPress::~PGestureLongPress()
{
}

void PGestureLongPress::touchDown(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
{
    if (!m_enabled)
    {
        return ;
    }
    
    if (pointer > 0 && m_state == STATE_POSSIBLE)
    {
        reset();

        PLOG_WARNING("Gesture long press support only one touch");

        return;
    }

    if (m_state == STATE_READY)
    {
        m_x = x;
        m_y = y;

        m_state = STATE_POSSIBLE;
        m_elapsedTime = 0;
    }
}

void PGestureLongPress::touchMove(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
{
    if (!m_enabled)
    {
        return ;
    }
    
    if (pointer > 0 && m_state == STATE_POSSIBLE)
    {
        reset();

        PLOG_WARNING("Gesture long press support only one touch");

        return;
    }

    if (m_state == STATE_POSSIBLE)
    {
        pfloat32 dx = (pfloat32)m_x - (pfloat32)x;
        pfloat32 dy = (pfloat32)m_y - (pfloat32)y;

        if (dx * dx + dy * dy > m_distanceThreshold * m_distanceThreshold)
        {
            reset();
        }
    }
}

void PGestureLongPress::touchUp(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
{
    if (!m_enabled)
    {
        return ;
    }
    reset();
}

void PGestureLongPress::reset()
{
    m_state = STATE_READY;
}

void PGestureLongPress::update(pfloat32 deltaTime)
{
    if (m_state == STATE_POSSIBLE)
    {
        m_elapsedTime += deltaTime;

        if (m_elapsedTime > m_intervalThreshold)
        {
            PEvent* event = createEvent(P_EVENT__LONG_PRESS);
            event->setParameter(P_EVENTPARAMETER__TOUCH_X, m_x);
            event->setParameter(P_EVENTPARAMETER__TOUCH_Y, m_y);
            event->queue(reinterpret_cast<PObject *>(P_NULL));

            reset();
        }
    }
}

void PGestureLongPress::setDistanceThreshold(pfloat32 value)
{
    m_distanceThreshold = value;
}

void PGestureLongPress::setIntervalThreshold(puint32 value)
{
    m_intervalThreshold = value;
}

