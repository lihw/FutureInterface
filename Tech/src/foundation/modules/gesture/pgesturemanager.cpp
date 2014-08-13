// pgesturemanager.cpp
// The gesture detector files.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#include "pgesturemanager.h"

#include "pgesture_private.h"

#include <PFoundation/PContext.h>
#include <PFoundation/pevent.h>

#include <PFoundation/pnew.h>
#include <PFoundation/plog.h>
#include <PFoundation/passert.h>
#include <PFoundation/ptime.h>

#include "pgesture_private.h"


PGestureManager::PGestureManager(PContext *context)
    : PModule("gesture-manager", context)
{  
    m_gestures[P_GESTURE_TYPE_TAP]       = PNEW(PGestureTap(this));
    m_gestures[P_GESTURE_TYPE_LONGPRESS] = PNEW(PGestureLongPress(this));
    m_gestures[P_GESTURE_TYPE_FLING]     = PNEW(PGestureFling(this));
    m_gestures[P_GESTURE_TYPE_PAN]       = PNEW(PGesturePan(this));
    m_gestures[P_GESTURE_TYPE_PINCH]     = PNEW(PGesturePinch(this));
}

PGestureManager::~PGestureManager()
{
    PASSERT(m_gestures[P_GESTURE_TYPE_TAP] == P_NULL);
    PASSERT(m_gestures[P_GESTURE_TYPE_LONGPRESS] == P_NULL);
    PASSERT(m_gestures[P_GESTURE_TYPE_FLING] == P_NULL);
    PASSERT(m_gestures[P_GESTURE_TYPE_PAN] == P_NULL);
    PASSERT(m_gestures[P_GESTURE_TYPE_PINCH] == P_NULL);
}

void PGestureManager::uninitialize()
{
    PDELETE(m_gestures[P_GESTURE_TYPE_TAP]);
    PDELETE(m_gestures[P_GESTURE_TYPE_LONGPRESS]);
    PDELETE(m_gestures[P_GESTURE_TYPE_FLING]);
    PDELETE(m_gestures[P_GESTURE_TYPE_PAN]);
    PDELETE(m_gestures[P_GESTURE_TYPE_PINCH]);
}

void PGestureManager::update()
{
    pfloat32 deltaTime = m_context->clock().systemDeltaTime();

    m_gestures[P_GESTURE_TYPE_TAP]->update(deltaTime);
    m_gestures[P_GESTURE_TYPE_LONGPRESS]->update(deltaTime);
    m_gestures[P_GESTURE_TYPE_FLING]->update(deltaTime);
    m_gestures[P_GESTURE_TYPE_PAN]->update(deltaTime);
    m_gestures[P_GESTURE_TYPE_PINCH]->update(deltaTime);
}

pbool PGestureManager::resume()
{
    m_gestures[P_GESTURE_TYPE_TAP]->reset();
    m_gestures[P_GESTURE_TYPE_LONGPRESS]->reset();
    m_gestures[P_GESTURE_TYPE_FLING]->reset();
    m_gestures[P_GESTURE_TYPE_PAN]->reset();
    m_gestures[P_GESTURE_TYPE_PINCH]->reset();

    return true;
}

void PGestureManager::recognize(PEvent *event)
{
    PEventTypeEnum eventType = event->getType();

    pint32  x         = event->parameter(P_EVENTPARAMETER__TOUCH_X).toInt();  
    pint32  y         = event->parameter(P_EVENTPARAMETER__TOUCH_Y).toInt();
    pint32  pointer   = event->parameter(P_EVENTPARAMETER__TOUCH_CURSOR_ID).toInt();
    puint32 timestamp = event->parameter(P_EVENTPARAMETER__TOUCH_TIMESTAMP).toUint32();

    if (pointer > 1)
    {
        return ;
    }

    if (eventType == P_EVENT__TOUCH_DOWN)
    {
        m_gestures[P_GESTURE_TYPE_TAP]->touchDown(x, y, timestamp, pointer);
        m_gestures[P_GESTURE_TYPE_LONGPRESS]->touchDown(x, y, timestamp, pointer);
        m_gestures[P_GESTURE_TYPE_FLING]->touchDown(x, y, timestamp, pointer);
        m_gestures[P_GESTURE_TYPE_PAN]->touchDown(x, y, timestamp, pointer);
        m_gestures[P_GESTURE_TYPE_PINCH]->touchDown(x, y, timestamp, pointer);
    }
    else if (eventType == P_EVENT__TOUCH_UP)
    {
        clamp(x, y);

        m_gestures[P_GESTURE_TYPE_TAP]->touchUp(x, y, timestamp, pointer);
        m_gestures[P_GESTURE_TYPE_LONGPRESS]->touchUp(x, y, timestamp, pointer);
        m_gestures[P_GESTURE_TYPE_FLING]->touchUp(x, y, timestamp, pointer);
        m_gestures[P_GESTURE_TYPE_PAN]->touchUp(x, y, timestamp, pointer);
        m_gestures[P_GESTURE_TYPE_PINCH]->touchUp(x, y, timestamp, pointer);
    }
    else if (eventType == P_EVENT__TOUCH_MOVE)
    {
        clamp(x, y);

        m_gestures[P_GESTURE_TYPE_TAP]->touchMove(x, y, timestamp, pointer);
        m_gestures[P_GESTURE_TYPE_LONGPRESS]->touchMove(x, y, timestamp, pointer);
        m_gestures[P_GESTURE_TYPE_FLING]->touchMove(x, y, timestamp, pointer);
        m_gestures[P_GESTURE_TYPE_PAN]->touchMove(x, y, timestamp, pointer);
        m_gestures[P_GESTURE_TYPE_PINCH]->touchMove(x, y, timestamp, pointer);
    }
}

void PGestureManager::clamp(pint32 &refX, pint32 &refY)
{
    refX = pMax(refX, (pint32)m_context->rect()[0]);
    refX = pMin(refX, (pint32)(m_context->rect()[0] + m_context->rect()[2] - 1));
    refY = pMax(refY, (pint32)m_context->rect()[1]);
    refY = pMin(refY, (pint32)(m_context->rect()[1] + m_context->rect()[3] - 1));
}

