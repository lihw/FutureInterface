// pgesturepan.cpp
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

PGesturePan::PGesturePan(PGestureManager* manager, pfloat32 distanceThreshold)
    : PAbstractGesture(manager, P_GESTURE_TYPE_PAN)
{
    m_distanceThreshold = distanceThreshold;

    reset();
}

PGesturePan::~PGesturePan()
{
}

void PGesturePan::setDistanceThreshold(pfloat32 value)
{
    m_distanceThreshold = value;
}

void PGesturePan::touchDown(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
{
    if (!m_enabled)
    {
        return ;
    }

    if (pointer > 0)
    {
        // During pan, receiving another pointer down, send pan end event and reset
        if (m_state == STATE_MOVING)
        {
            PEvent* event = createEvent(P_EVENT__PAN_END);
            event->queue(reinterpret_cast<PObject *>(P_NULL));
        }

        reset();
    }
    else
    {
        m_x = x;
        m_y = y;

        m_state = STATE_POSSIBLE;
    }
}

void PGesturePan::touchMove(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
{
    if (!m_enabled)
    {
        return ;
    }

    if (pointer > 0)
    {
        return;
    }

    switch (m_state)
    {
        case STATE_POSSIBLE:
            {
                // If we moved enought distance 
                pfloat32 dx = (pfloat32)m_x - (pfloat32)x;
                pfloat32 dy = (pfloat32)m_x - (pfloat32)y;

                if (dx * dx + dy * dy > m_distanceThreshold * m_distanceThreshold)
                {
                    // Send the pan begin event
                    PEvent* event = createEvent(P_EVENT__PAN_BEGIN);
                    event->setParameter(P_EVENTPARAMETER__TOUCH_X, m_x);
                    event->setParameter(P_EVENTPARAMETER__TOUCH_Y, m_y);
                    event->queue(reinterpret_cast<PObject *>(P_NULL));
    
                    // Send the pan move event
                    event = createEvent(P_EVENT__PAN);
        
                    event->setParameter(P_EVENTPARAMETER__TOUCH_X, x);
                    event->setParameter(P_EVENTPARAMETER__TOUCH_Y, y);
                    event->setParameter(P_EVENTPARAMETER__PAN_DELTA_X, x - m_x);
                    event->setParameter(P_EVENTPARAMETER__PAN_DELTA_Y, y - m_y);
                    event->queue(reinterpret_cast<PObject *>(P_NULL));

                    m_x = x;
                    m_y = y;

                    // Update the state to move.
                    m_state = STATE_MOVING;
                }
            }
            break;
        case STATE_MOVING:
            {
                // Send the pan move event
                PEvent *event = createEvent(P_EVENT__PAN);
    
                event->setParameter(P_EVENTPARAMETER__TOUCH_X, x);
                event->setParameter(P_EVENTPARAMETER__TOUCH_Y, y);
                event->setParameter(P_EVENTPARAMETER__PAN_DELTA_X, x - m_x);
                event->setParameter(P_EVENTPARAMETER__PAN_DELTA_Y, y - m_y);
                event->queue(reinterpret_cast<PObject *>(P_NULL));

                m_x = x;
                m_y = y;

                // Update the state to move.
                m_state = STATE_MOVING;
            }
            break;
        default:
            PLOG_WARNING("Pan is not supposed to move now");
            break;
    } 
}

void PGesturePan::touchUp(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer)
{
    if (!m_enabled)
    {
        return ;
    }

    if (pointer > 0)
    {
        return;
    }
    
    switch (m_state)
    {
        case STATE_READY:
            PLOG_WARNING("Pan is not supposed to end now");
            break;
        case STATE_POSSIBLE:
            return;
        case STATE_MOVING:
            {
                PEvent* event = createEvent(P_EVENT__PAN_END);
                event->setParameter(P_EVENTPARAMETER__TOUCH_X, x);
                event->setParameter(P_EVENTPARAMETER__TOUCH_Y, y);
                event->setParameter(P_EVENTPARAMETER__PAN_DELTA_X, x - m_x);
                event->setParameter(P_EVENTPARAMETER__PAN_DELTA_Y, y - m_y);
                event->queue(reinterpret_cast<PObject *>(P_NULL));
                reset();
            }
            break;
        default:
            PLOG_WARNING("Pan is not supposed to end now.");
    }
}

void PGesturePan::reset()
{
    m_state = STATE_READY;
}

