// peventmanager.cpp
// The event manager
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//
// 


#include "peventmanager.h"

#include <PFoundation/pevent.h>
#include <PFoundation/pcontext.h>

#include <PFoundation//pobject.h>
#include <PFoundation/pclock.h>


PEventManager::PEventManager(PContext *context)
    : PModule("event-manager", context)
{
}

PEventManager::~PEventManager()
{
    PList<PEvent *>::iterator it;
    PList<PEvent *>::iterator ib = m_queue.begin();
    PList<PEvent *>::iterator ie = m_queue.end();
    for (it = ib; it != ie; ++it)
    {
        PDELETE(*it);
    }
    m_queue.clear();
}

void PEventManager::update()
{
    puint32 deltaTime = (puint32)m_context->clock().systemDeltaTime();

    // The deltaTime is the elapsed time from last frame, so each event should
    // update its timer in this function so that it approaches the firing stage
    // gradually. The correct logic should be 
    //
    // Step 1. update times of all queued events 

    PList<PEvent *>::iterator it;
    PList<PEvent *>::iterator ib = m_queue.begin();
    PList<PEvent *>::iterator ie = m_queue.end();
    for (it = ib; it != ie; ++it)
    {
        if ((*it)->m_fireTime < deltaTime)
        {
            (*it)->m_fireTime = 0;
        }
        else
        {
            (*it)->m_fireTime -= pMin((*it)->m_fireTime, deltaTime);
        }
    }

    // Step 2. fire those events on due in the order of the fire time.
    it = ib;
    while (it != ie)
    {
        if ((*it)->canFire())
        {
            PEvent *event = *it;
            fire(event);
            m_queue.erase(it);
            // FIXME: it can't be right to reset the iterator
            // to the head of list. Those nodes already visited
            // will be checked again. It seems to me we need to have a dedicated
            // event queue data structure instead of using qlist.
            it = m_queue.begin();
            recycelEvent(event);
        }
        else
        {
            ++it;
        }
    }
}
    
void PEventManager::queueEvent(PEvent *event)
{
    m_queue.append(event);
}

void PEventManager::fire(PEvent *event)
{
    event->m_handled = true;
    if (event->m_receiver == P_NULL)
    {
        m_context->dispatch(event);
    }
    else
    {
        event->m_receiver->onEvent(event);
    }
}

PEvent * PEventManager::createEvent(PEventTypeEnum type, PObject *sender)
{
    // TODO: a memory pool.
    PEvent *ret = PNEW(PEvent(type, sender, m_context->clock().timestamp(), this));
    return ret;
}

void PEventManager::recycelEvent(PEvent *event)
{
    // TODO: recycle the event memory.
    PDELETE(event);
}
