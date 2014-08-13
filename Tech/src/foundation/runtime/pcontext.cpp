// pcontext.h
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pcontext.h"

#include <PFoundation/pdevice.h>
#include <PFoundation/pmodules_module.h>

#include <PFoundation/passert.h>
#include <PFoundation/plog.h>
#include <PFoundation/pnew.h>
#include <PFoundation/pmemorydebugger.h>


PContext::PContext(const PContextProperties& properties)
    : m_properties(properties)
{
    m_state                 = P_CONTEXT_STATE_UNINITIALIZED;
    m_device                = P_NULL;

    // Initialize the window size to the user defined one.
    m_rect[0] = 0;
    m_rect[1] = 0;
    m_rect[2] = properties.m_windowWidth;
    m_rect[3] = properties.m_windowHeight;
}

PContext::~PContext()
{
    PASSERT(m_device == P_NULL);
    PASSERT(m_state == P_CONTEXT_STATE_UNINITIALIZED ||
            m_state == P_CONTEXT_STATE_ERROR ||
            m_state == P_CONTEXT_STATE_QUITING);
}

void PContext::quit()
{
    if (m_state != P_CONTEXT_STATE_UNINITIALIZED &&
        m_state != P_CONTEXT_STATE_QUITING)
    {
        PASSERT(m_device != P_NULL);
        m_device->setState(P_DEVICE_STATE_CLOSED);

        PLOG_INFO("Context %s is quitting.", name().c_str());
        m_state = P_CONTEXT_STATE_QUITING;
    }
}

pbool PContext::initialize(puint32 screenWidth, puint32 screenHeight)
{
    // It can't be called more than once.
    PASSERT(m_state == P_CONTEXT_STATE_UNINITIALIZED);
    if (m_state != P_CONTEXT_STATE_UNINITIALIZED)
    {
        return true;
    }
     
    updateRect(screenWidth, screenHeight);
    
    m_clock.setMaxFps(m_properties.m_maxFps);
    
    m_device = PNEW(PDevice(this));

    // Native modules
    m_gestureManager = PNEW(PGestureManager(this));
    m_timerManager   = PNEW(PTimerManager(this));
    m_eventManager   = PNEW(PEventManager(this));

    m_gestureManager->initialize();
    m_timerManager->initialize();
    m_eventManager->initialize();

    // Other modules
    for (puint32 i = 0; i < m_modules.size(); ++i)
    {
        m_modules[i]->initialize();
    }
    
    return true;
}

void PContext::destroy()
{
    if (m_state != P_CONTEXT_STATE_UNINITIALIZED)
    {
        for (pint32 i = (pint32)m_modules.size() - 1; i >= 0; --i)
        {   
            m_modules[i]->uninitialize();
            PDELETE(m_modules[i]);
        }

        m_gestureManager->uninitialize();
        m_timerManager->uninitialize();
        m_eventManager->uninitialize();

        PDELETE(m_gestureManager);
        PDELETE(m_timerManager);
        PDELETE(m_eventManager);

        PDELETE(m_device);

        m_state = P_CONTEXT_STATE_UNINITIALIZED;
    }
}

pbool PContext::update()
{
    // Update the clock of this frame.
    m_clock.tick();
    
    // One iteration of window main loop.
    m_device->update();
    // When the window is closed, we quit the context.
    if (m_device->getState() == P_DEVICE_STATE_CLOSED)
    {
        m_state = P_CONTEXT_STATE_QUITING;
    }

    m_timerManager->update();
    m_gestureManager->update();
    m_eventManager->update();

    // Rendering.
    if (m_state == P_CONTEXT_STATE_RUNNING)
    {
        // Callback before updating this frame.
        onUpdate();

        for (puint32 i = 0; i < m_modules.size(); ++i)
        {
            m_modules[i]->update();
        }
    }

    // Memory debugger.
    PMemoryDebugger::update(m_clock.systemDeltaTime());

    return true;
}

void PContext::pause()
{
    onPause();

    PASSERT(m_state == P_CONTEXT_STATE_RUNNING);
    setState(P_CONTEXT_STATE_PAUSED);
    
    m_eventManager->pause();
    m_timerManager->pause();
    m_gestureManager->pause();

    for (pint32 i = (pint32)m_modules.size() - 1; i >= 0; --i)
    {
        m_modules[i]->pause();
    }
    
    m_clock.pause();
}

pbool PContext::resume()
{
    PASSERT(m_state == P_CONTEXT_STATE_PAUSED);
    setState(P_CONTEXT_STATE_RUNNING);

    // Resume the clock first.
    m_clock.resume();

    for (puint32 i = 0; i < m_modules.size(); ++i)
    {
        m_modules[i]->resume();
    }
    
    m_gestureManager->resume();
    m_timerManager->resume();
    m_eventManager->resume();

    onResume();
    
    return true;
}

void PContext::setState(PContextState state)
{
    if (m_state != state)
    {
        m_state = state;

        if (m_state == P_CONTEXT_STATE_RUNNING)
        {
            // Resume the clock when begin to running.
            m_clock.resume();
        }
    }
}
    
void PContext::addModule(PModule *module)
{
#if defined P_MODULE
    for (puint32 i = 0; i < m_modules.size(); ++i)
    {
        if (m_modules[i]->name() == module->name())
        {
            PASSERT_NOTREACHABLE();
            PLOG_WARNING("Module %s already exists in context.", module->name().c_str());
            return ;
        }
    }
#endif
    m_modules.append(module);
}

pbool PContext::onInitialized()
{
    return true;
}

pbool PContext::onClose()
{
    return true;
}

void PContext::onDestroy()
{
}

void PContext::updateRect(puint32 screenWidth, puint32 screenHeight)
{
	const puint32 userWindowWidth = (m_properties.m_windowWidth == 0xffffffff? screenWidth : m_properties.m_windowWidth);
    const puint32 userWindowHeight = (m_properties.m_windowHeight == 0xffffffff? screenHeight : m_properties.m_windowHeight);

    pfloat32 widthScaling = (pfloat32)screenWidth/ (pfloat32)userWindowWidth;
    pfloat32 heightScaling = (pfloat32)screenHeight / (pfloat32)userWindowHeight;
    if (heightScaling < widthScaling)
    {
        m_rect[2] = (puint32)(userWindowWidth * heightScaling);
        m_rect[3] = screenHeight;
    }
    else
    {
        m_rect[2] = screenWidth;
        m_rect[3] = (puint32)(widthScaling * userWindowHeight);
    }

    switch (m_properties.m_windowLayout)
    {
        case P_WINDOWLAYOUT_LEFT_BOTTOM:
            m_rect[0] = 0;
            m_rect[1] = 0;
            break;
        case P_WINDOWLAYOUT_MIDDLE_CENTER:
            m_rect[0] = (screenWidth - m_rect[2]) / 2;
            m_rect[1] = (screenHeight - m_rect[3]) / 2;
            break;
        case P_WINDOWLAYOUT_RIGHT_TOP:
            m_rect[0] = screenWidth - m_rect[2];
            m_rect[1] = screenHeight - m_rect[3];
            break;
        default:
            break;
    }
}

void PContext::onScreenResized(puint32 width, puint32 height)
{ 
    updateRect(width, height);

    // Update all layers' viewports
    /*
    PList<PScene *>::iterator it = m_scenes.begin();
    PList<PScene *>::iterator ie = m_scenes.end();
    while (it != ie)
    {
        (*it)->onWindowResized(m_rect);
        ++it;
    }
    */
}

pbool PContext::onUpdate()
{
    return true;
}

void PContext::dispatch(PEvent *event)
{
    switch (event->type())
    {
        case P_EVENT__KEYUP:
        case P_EVENT__KEYDOWN:
        case P_EVENT__KEYREPEAT:
            onKeyboard(event);
            break;
        case P_EVENT__TOUCH_DOWN:
        case P_EVENT__TOUCH_MOVE:
        case P_EVENT__TOUCH_UP:
            onTouch(event);
            break;
        case P_EVENT__TAP:
            onTap(event);
            break;
        case P_EVENT__PAN_BEGIN:
            onPanBegin(event);
            break;
        case P_EVENT__PAN:
            onPan(event);
            break;
        case P_EVENT__PAN_END:
            onPanEnd(event);
            break;
        case P_EVENT__PINCH_BEGIN:
            onPinchBegin(event);
            break;
        case P_EVENT__PINCH:
            onPinch(event);
            break;
        case P_EVENT__PINCH_END:
            onPinchEnd(event);
            break;
        case P_EVENT__FLING:
            onFling(event);
            break;
        case P_EVENT__LONG_PRESS:
            onLongPress(event);
            break;
        case P_EVENT__TIMER_EXPIRED:
            onTimer(event);
            break;
        default:
            onUserEvent(event);
            break;
    }
}

pbool PContext::onKeyboard(PEvent *event)
{
    return true;
}

pbool PContext::onTouch(PEvent *event)
{
    return true;
}
pbool PContext::onUserEvent(PEvent *event)
{
    return true;
}
void PContext::onPause()
{
    // Override me.
}

void PContext::onResume()
{
    // Override me.
}

pbool PContext::onTap(PEvent *event)
{
    return false;
}

pbool PContext::onLongPress (PEvent *event)
{
    return false;
}

pbool PContext::onFling(PEvent *event)
{
    return false;
}

pbool PContext::onPanBegin(PEvent *event)
{
    return false;
}

pbool PContext::onPan(PEvent *event)
{
    return false;
}

pbool PContext::onPanEnd(PEvent *event)
{
    return false;
}

pbool PContext::onPinchBegin(PEvent *event)
{
    return false;
}

pbool PContext::onPinch(PEvent *event)
{
    return false;
}

pbool PContext::onPinchEnd(PEvent *event)
{
    return false;
}

pbool PContext::onTimer(PEvent *event)
{
    return false;
}

