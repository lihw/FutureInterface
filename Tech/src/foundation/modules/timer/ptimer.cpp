// ptimer.cpp 
// The timer file
// in the context. 
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "ptimer.h" 

#include <PFoundation/ptimermanager.h>
#include <PFoundation/PContext.h>
#include <PFoundation/peventmanager.h>
#include <PFoundation/pevent.h>

PTimer::PTimer(PTimerManager *manager, puint32 id, pfloat32 fireTime, 
    puint32 repeatCount , pfloat32 repeatInterval)
{
    PASSERT(manager != P_NULL);

    m_fireMillis       = fireTime;
    m_repeatMillis     = repeatInterval;
    m_repeatCount      = repeatCount;
    m_repeatedTimes    = 0;
    m_id               = id;
    m_manager          = manager;
    m_callback         = P_NULL;
    m_isRunning        = false;
}

PTimer::~PTimer()
{
    m_manager->removeTimer(this);
    m_manager = P_NULL;
}

void PTimer::run()
{
    m_elapsedMillis     = 0;
    m_repeatedTimes     = 0;
    m_isRunning         = true;
    
    m_manager->activateTimer(this);
}

void PTimer::kill()
{
    PLOG_WARNING( "The timer destroy by call kill");
    m_manager->removeTimer(this);
    PDELETETHIS(this);
}

void PTimer::setCallback(PAbstractTimerCallback *callback)
{
    m_callback = callback;
}

void PTimer::update(pfloat32 deltaTime)
{
    PASSERT(m_manager != P_NULL);

    m_elapsedMillis += deltaTime;

    if (m_fireMillis < m_elapsedMillis)
    {
        m_repeatedTimes++;

        PEvent* event = createEvent(P_EVENT__TIMER_EXPIRED);
        event->setParameter(P_EVENTPARAMETER__TIMER_ID, m_id);
        event->setParameter(P_EVENTPARAMETER__TIMER_REPEAT_TIMES, m_repeatedTimes - 1);
        event->setParameter(P_EVENTPARAMETER__TIMER_ELAPSED_MILLIS, m_elapsedMillis);
        event->queue(reinterpret_cast<PObject *>(P_NULL));
        if (m_callback != P_NULL)
        {
            m_callback->onTimerOut(m_id, m_repeatedTimes, m_elapsedMillis);
        }

        if (m_repeatedTimes - 1 >= m_repeatCount)
        {
            kill();
        }
        else
        {
            m_fireMillis += m_repeatMillis;
        }
    }
}

PEvent* PTimer::createEvent(PEventTypeEnum eventType)
{
    // TODO: manage the memory allocation effectively.
    PContext *context = m_manager->context();
    return context->eventManager()->createEvent(eventType, P_NULL);
}
