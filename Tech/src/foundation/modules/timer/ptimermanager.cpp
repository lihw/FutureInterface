// ptimermanager.cpp 
// The timer manager  file
// in the context. 
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "ptimermanager.h" 

#include <PFoundation/ptimer.h>
#include <PFoundation/PContext.h>
#include <PFoundation/peventmanager.h>
#include <PFoundation/pevent.h>

#include <PFoundation/pclock.h>
#include <PFoundation/ptime.h>

PTimerManager::PTimerManager(PContext *context)
    : PModule("timer-manager", context)
{
    m_nextId  = 1;
}

PTimerManager::~PTimerManager()
{
    PList<PTimer *>::iterator it = m_activeTimers.begin();
    PList<PTimer *>::iterator ie = m_activeTimers.end();
    while (it != ie)
    {
        PDELETE(*it);
        ++it;
    }

    it = m_readyTimers.begin();
    ie = m_readyTimers.end();
    while (it != ie)
    {
        PDELETE(*it);
        ++it;
    }
}

void PTimerManager::update()
{
    pfloat32 deltaTime = context()->clock().deltaTime();

    PList<PTimer *>::iterator it = m_activeTimers.begin();
    PList<PTimer *>::iterator ie = m_activeTimers.end();
    // Here need a itt to save the old it because when call update,
    // maybe call kill() and then call removeTimer() to erase it
    while (it != ie)
    {
        PTimer *timer = *it;
        PList<PTimer *>::iterator itt = it;
        ++itt;
        timer->update(deltaTime);
        it = itt;
    }
}

void PTimerManager::activateTimer(PTimer* timer)
{
    PASSERT(timer->isRunning());
    PList<PTimer *>::iterator it = m_readyTimers.find(timer);
    if (it != m_readyTimers.end())
    {
        m_readyTimers.erase(it);
        m_activeTimers.prepend(timer);
    }
    else
    {
        PLOG_WARNING("Timer %d is either not existing or already started",
            timer->getId());
    }
}

void PTimerManager::removeTimer(PTimer* timer)
{
    if (timer->isRunning())
    {
        PList<PTimer *>::iterator it = m_activeTimers.find(timer);
        if (it != m_activeTimers.end())
        {
            m_activeTimers.erase(it);
        }
    }
    else
    {
        PList<PTimer *>::iterator it = m_readyTimers.find(timer);
        if (it != m_readyTimers.end())
        {
            m_readyTimers.erase(it);
        }
    }
}

PTimer* PTimerManager::createTimer(pfloat32 fireTime, puint32 repeatCount, pfloat32 repeatInterval)
{
    PTimer *ret = PNEW(PTimer(this, m_nextId, fireTime, 
        repeatCount, repeatInterval));
    m_readyTimers.append(ret);
    m_nextId++;
    return ret;
}
