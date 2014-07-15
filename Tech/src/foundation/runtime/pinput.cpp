// pinput.cpp
// The window events.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#include "pinput.h"

#include <PFoundation/pdevice.h>
#include <PFoundation/pcontext.h>
#include <PFoundation/pnew.h>
#include <PFoundation/passert.h>
#include <PFoundation/plog.h>


PInputEventQueue::PInputEventQueue(PDevice* device)
{
    m_device = device;

    m_numberOfEventsCached = P_INPUT_EVENT_QUEUE_INITIAL_SIZE;
    m_events = PNEWARRAY(PInputEvent, m_numberOfEventsCached);
    PASSERT(m_events != P_NULL);
#if P_GESTURE_PINCH_SIMULATION_ENABLED == 1
    m_pinchSimulator = PNEW(PGesturePinchSimulator);
#endif
    m_numberOfEvents = 0;
}

PInputEventQueue::~PInputEventQueue()
{
    PDELETEARRAY(m_events);

#if P_GESTURE_PINCH_SIMULATION_ENABLED == 1
    PDELETE(m_pinchSimulator);
#endif

}

puint32 PInputEventQueue::getNumberOfEvents() const
{
    return m_numberOfEvents;
}

void PInputEventQueue::clear()
{
    m_numberOfEvents = 0;
}

PInputEvent* PInputEventQueue::getEventAtIndex(puint32 index)
{
    PASSERT(index < m_numberOfEvents);
    return &m_events[index];
}

void PInputEventQueue::addKeyEvent(puint32 nativeScanCode, PInputKey key, puint32 state, puint32 timestamp)
{
    PInputEvent* event = reserveEvent();
    PASSERT(event != P_NULL);

    event->setType(P_INPUT_EVENT_KEY);

    PInputEventKey* keyEvent = event->getKeyEvent();

    keyEvent->setRawScanCode(nativeScanCode);
    keyEvent->setTranslatedScanCode(key);
    keyEvent->setState(state);
    keyEvent->setTimestamp(timestamp);

    PInputKeyDeviceStateEnum keyState;
    keyState = (PInputKeyDeviceStateEnum)(state & (P_KEY_DEVICE_STATE_DOWN | P_KEY_DEVICE_STATE_UP));
    m_device->setKeyState(keyEvent->getKey(), keyState);

#if P_GESTURE_PINCH_SIMULATION_ENABLED == 1
    if (state & P_KEY_DEVICE_STATE_CTRL)
    {
        if (m_pinchSimulator->getState() == P_POINTING_DEVICE_STATE_DRAG && keyState == P_KEY_DEVICE_STATE_UP )
        {
            PInputEventTouch* event = addTouchEvent(2, P_TOUCH_STATE_END);
            PASSERT(event != P_NULL);

            m_pinchSimulator->createEvent(event);
        }
    }
#endif

}

void PInputEventQueue::addPointerEvent(pint32 x, pint32 y, PInputPointingDeviceStateEnum state, pbool isCtrlHeld, puint32 timestamp)
{
    puint32 screenWidth = m_device->getScreenWidth();
    puint32 screenHeight = m_device->getScreenHeight();

    // Change orientation before creating event. 
    if (m_device->getOrientationAngle() == P_DEVICE_ORIENTATION_ANGLE_180)
    {
        x = (pint16)(screenWidth - 1 - x);
        y = (pint16)(screenHeight - 1 - y);
    }

    // Translate pointer events to touch events
    PInputCursorStateEnum cursorState = P_CURSOR_STATE_STATIONARY;
    PInputTouchStateEnum touchState = P_TOUCH_STATE_END;

    switch(state)
    {
        case P_POINTING_DEVICE_STATE_DOWN:
            cursorState = P_CURSOR_STATE_DOWN;
            touchState = P_TOUCH_STATE_BEGIN;
            break;
        case P_POINTING_DEVICE_STATE_DRAG:
            cursorState = P_CURSOR_STATE_MOVE;
            touchState = P_TOUCH_STATE_CONTINUE;
            break;
        case P_POINTING_DEVICE_STATE_UP:
            cursorState = P_CURSOR_STATE_UP;
            break;
        case P_POINTING_DEVICE_STATE_MOVE:
        default:
            break;
    }

    if (cursorState != P_CURSOR_STATE_STATIONARY)
    {
#if P_GESTURE_PINCH_SIMULATION_ENABLED == 1

        if (isCtrlHeld)
        {
            PInputEventTouch* event = addTouchEvent(2, touchState);
            PASSERT(event != P_NULL);
            m_pinchSimulator->createEvent(x, y, state, event);
        }
        else
        {
#endif
        PInputEventTouch* event = addTouchEvent(1, touchState);
        PASSERT(event != P_NULL);

        if (event != P_NULL)
        {
            PInputEventTouch::TouchCursor* c = event->getCursor(0);

            c->m_id = 0;
            c->m_x = x;
            c->m_y = y;
            c->m_state = cursorState;
            c->m_pressure = 1.0f;
            c->m_radius = 1.0f;
            c->m_timestamp = timestamp;
        }

#if P_GESTURE_PINCH_SIMULATION_ENABLED == 1
        }
#endif
    }
}

PInputEventTouch* PInputEventQueue::addTouchEvent(puint32 numCursors, PInputTouchStateEnum state)
{
    PInputEvent* event = (PInputEvent*)reserveEvent();
    PASSERT(event != P_NULL);

    event->setType(P_INPUT_EVENT_TOUCH);
    
    PInputEventTouch* touchEvent = event->getTouchEvent();

    touchEvent->setCursorCount(numCursors);
    touchEvent->setState(state);

    return touchEvent;
}
    
void PInputEventQueue::setTouchCursor(PInputEventTouch* touchEvent, pint32 index, pint32 id,
            pfloat32 x, pfloat32 y, pfloat32 pressure, pfloat32 area, pint32 state, puint32 timestamp)
{
    PASSERT(touchEvent != P_NULL);

    PInputEventTouch::TouchCursor* cursor = touchEvent->getCursor(index);
    if (cursor == P_NULL)
    {
        PLOG_ERROR("too many cursor data for this touch event");
        return ;
    }

    cursor->m_id = id;
    if (m_device->getOrientationAngle() == P_DEVICE_ORIENTATION_ANGLE_180)
    {
        cursor->m_x = pint16(180.0f - x);
        cursor->m_y = pint16(180.0f - y);
    }
    else
    {
        cursor->m_x = pint16(x);
        cursor->m_y = pint16(y);
    }
    cursor->m_pressure = pressure;
    cursor->m_radius = area;
    cursor->m_state = (PInputCursorStateEnum)(state);
    cursor->m_timestamp = timestamp;
}

PInputEvent* PInputEventQueue::reserveEvent()
{
    PInputEvent* ret;

    if (m_numberOfEvents >= m_numberOfEventsCached)
    {
        m_numberOfEventsCached += P_INPUT_EVENT_QUEUE_INITIAL_SIZE / 2;

        PLOG_DEBUG("event queue resized to %d", m_numberOfEventsCached);
      
        PInputEvent* newEvents = PNEWARRAY(PInputEvent, m_numberOfEventsCached);
        for (puint32 i = 0; i < m_numberOfEvents; ++i)
        {
            newEvents[i] = m_events[i];
        }

        PDELETEARRAY(m_events);
        m_events = newEvents;
    }

    ret = &m_events[m_numberOfEvents];
    m_numberOfEvents++;

    return ret;
}
