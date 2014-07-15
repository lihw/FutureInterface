// ptimermanager.h 
// The Timer manager which manages all Timers
// in the context. 
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PTIMER_MANAGER_H
#define PTIMER_MANAGER_H

#include <PFoundation/pmodule.h>
#include <PFoundation/plist.h>

class PContext;
class PTimer;

class P_DLLEXPORT PTimerManager : public PModule
{
    friend class PTimer;

public:
    PTimerManager(PContext *context);
    virtual ~PTimerManager();

    virtual void update();

    PTimer *createTimer(pfloat32 fireTime, puint32 repeatCount = 0, 
        pfloat32 repeatInterval = 0);
    
private:
    void activateTimer(PTimer *timer);
    void removeTimer(PTimer *timer);
    
private:
    PList<PTimer *> m_activeTimers; // running timers.
    PList<PTimer *> m_readyTimers;  // created but not started
    puint32         m_nextId;       // The id of next timer.
};

#endif // !PTIMER_MANAGER_H
