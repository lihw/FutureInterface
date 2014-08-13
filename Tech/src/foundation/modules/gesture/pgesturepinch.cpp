// pgesturepinch.cpp
// The gesture pinch file.
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

#include <math.h>


PGesturePinch::PGesturePinch(PGestureManager* manager)
    : PAbstractGesture(manager, P_GESTURE_TYPE_PINCH)
{
    reset();
}

PGesturePinch::~PGesturePinch()
{

}

void PGesturePinch::touchDown(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
{
    if (!m_enabled)
    {
        return;
    }
    
    if (pointer >= 2)
    {
        // During pinch ,receive third pointer, send pinch end event and reset.
        if (m_state == STATE_MOVING)
        {
            PEvent* event = createEvent(P_EVENT__PINCH_END);
            event->queue(reinterpret_cast<PObject *>(P_NULL));
        }

        reset();

        return;
    }

    m_pressDown[pointer] = true;

    m_points[pointer][0] = x;
    m_points[pointer][1] = y;

    if (m_pressDown[0] && m_pressDown[1])
    {
        m_state = STATE_MOVING;

        m_initialPoints[0][0] = m_points[0][0];
        m_initialPoints[0][1] = m_points[0][1];
        m_initialPoints[1][0] = m_points[1][0];
        m_initialPoints[1][1] = m_points[1][1];
        m_initialDistance = getDistance(m_initialPoints[0], m_initialPoints[1]);
        m_initialRadians = getRadians(m_initialPoints[0], m_initialPoints[1]);

        PEvent* event = createEvent(P_EVENT__PINCH_BEGIN);

        // TODO: Use Vector2I
        event->setParameter(P_EVENTPARAMETER__PINCH_1ST_POINTER, (void*)m_initialPoints[0]);
        event->setParameter(P_EVENTPARAMETER__PINCH_2ND_POINTER, (void*)m_initialPoints[1]);
        event->queue(reinterpret_cast<PObject *>(P_NULL));
    }
}

void PGesturePinch::touchMove(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
{
    if (!m_enabled)
    {
        return;
    }
    
    if (pointer >= 2)
    {
        if (m_state == STATE_MOVING)
        {
            PLOG_WARNING("Gesture pinch only supports two touches");
            return;
        }
    }

    if (m_state == STATE_MOVING)
    {
        PASSERT(m_pressDown[0] && m_pressDown[1]);
        
        m_points[pointer][0] = x;
        m_points[pointer][1] = y;

        pfloat32 currentDistance = getDistance(m_points[0], m_points[1]);
        pfloat32 currentRadians = getRadians(m_points[0], m_points[1]);
        pfloat32 rotateRadians = currentRadians - m_initialRadians;

        pfloat32 scaling = currentDistance / m_initialDistance;

        // TODO: Use Vector2I
        PEvent* event = createEvent(P_EVENT__PINCH);
        event->setParameter(P_EVENTPARAMETER__PINCH_1ST_POINTER, (void*)m_points[0]);
        event->setParameter(P_EVENTPARAMETER__PINCH_2ND_POINTER, (void*)m_points[1]);

        event->setParameter(P_EVENTPARAMETER__PINCH_SCALING, scaling);
        event->setParameter(P_EVENTPARAMETER__PINCH_ROTATION_RADIANS, rotateRadians);
        event->queue(reinterpret_cast<PObject *>(P_NULL));
    }
}

void PGesturePinch::touchUp(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
{
    if (!m_enabled)
    {
        return;
    }
    
    if (pointer >= 2)
    {
        if (m_state == STATE_MOVING)
        {
            PLOG_WARNING("Gesture pinch only supports two touches");
        }
        return;
    }
    
    m_pressDown[pointer] = false;
    if (m_state == STATE_MOVING)
    {
        m_state = STATE_POSSIBLE;

        PEvent* event = createEvent(P_EVENT__PINCH_END);
        event->queue(reinterpret_cast<PObject *>(P_NULL));
    }
    else if (m_state == STATE_POSSIBLE)
    {
        m_state = STATE_READY;
    }
}

void PGesturePinch::reset()
{
    m_state = STATE_READY;

    m_pressDown[0] = false;
    m_pressDown[1] = false;
}

pfloat32 PGesturePinch::getDistance(const pint32 *firstPoint,const pint32 *secondPoint)
{
    PASSERT(firstPoint != P_NULL && secondPoint != P_NULL);

    pfloat32 distX = (pfloat32)(secondPoint[0] - firstPoint[0]);
    pfloat32 distY = (pfloat32)(secondPoint[1] - firstPoint[1]);
    
    // TODO: use quick sqrt.
    return sqrtf(distX * distX + distY * distY);
}

pfloat32  PGesturePinch:: getRadians(const pint32 *firstPoint, const pint32 *secondPoint)
{
    PASSERT(firstPoint != P_NULL && secondPoint != P_NULL);

    pfloat32 distX = (pfloat32)(secondPoint[0] - firstPoint[0]);
    pfloat32 distY = (pfloat32)(secondPoint[1] - firstPoint[1]);
    
    return atan2f(distY, distX);
}
