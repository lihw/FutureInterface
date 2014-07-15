// ptimer.h 
// The timer header file
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PTIMER_H
#define PTIMER_H

#include <PFoundation/ptimermanager.h>
#include <PFoundation/peventtype.h>

#include <PFoundation/pstring.h>

class PEvent;

enum 
{
    P_TIMER_REPEAT_FOREVER = -1,
};


// Note that timer might not alarm precisely at the specified time for we
// update timers in frame basis.  The alarm time is always aligned with frame
// rate. For example, we ask a timer to jump up after 10 millseconds, but
// current frame rate is 30 FPS. The timer will trigger the callback after 33
// ms rather than 10 milliseconds. To remedy this defect, we pass a true
// elapsed milliseconds to the timer¡¯s callback.

// In addition to send event message P_EVENT__TIMER_EXPIRED, we allow user to
// register a time off callback. The event is sent anyway.
class P_DLLEXPORT PAbstractTimerCallback 
{
public:
    PAbstractTimerCallback() {}
    virtual ~PAbstractTimerCallback(){}
    virtual void onTimerOut(puint32 id, puint32 repeatedTimes, pfloat32 elapsedMilliseconds) {}
};

class P_DLLEXPORT PTimer 
{
    friend class PTimerManager;

    // Consult: Why change list two functions scope from private to public ?
    // I think it constructor is private, copy and assign scope is private too.
    
    PTimer(const PTimer &other) {}
    void operator=(const PTimer &other) {}
    
public:
    void run();
    void kill();

    void setCallback(PAbstractTimerCallback *callback);
    P_INLINE const puint32 getId() const { return m_id; }

private:
    PTimer(PTimerManager *manager, puint32 id, pfloat32 fireTime, 
        puint32 repeatCount, pfloat32 repeatInterval);
    ~PTimer();
    void update(pfloat32 deltaTime);
    PEvent* createEvent(PEventTypeEnum eventType);
    
    P_INLINE const pbool isRunning() const { return m_isRunning; }
    
private:
    pfloat32                   m_elapsedMillis; 
    pfloat32                   m_fireMillis;
    pfloat32                   m_repeatMillis;
    puint32                    m_repeatedTimes; // Only effective in repeat timer. How many times the timer has alarmed.
    puint32                    m_repeatCount; // Only effective in repeat timer. How many times the timer will alarm.
    puint32                    m_id;
    PTimerManager*             m_manager;
    PAbstractTimerCallback    *m_callback;
    pbool                      m_isRunning;
};


#endif // !PTIMER_H
