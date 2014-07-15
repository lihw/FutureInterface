// pevent.h
// The base class of events
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//
// 

#ifndef PEVENT_H
#define PEVENT_H

#include <PFoundation/peventtype.h>
#include <PFoundation/peventparameters.h>
#include <PFoundation/peventmanager.h>

#include <PFoundation/pvariant.h>


#define P_EVENT_PARAMETER_MAX_NUMBER 32

enum PEventPriorityEnum 
{
    P_EVENT_PRIORITY_HIGH,
    P_EVENT_PRIORITY_NORMAL,
    P_EVENT_PRIORITY_LOW,

    P_EVENT_PRIORITY_DEFAULT = P_EVENT_PRIORITY_NORMAL,
    P_EVENT_PRIORITY_FIRST = P_EVENT_PRIORITY_HIGH,
    P_EVENT_PRIORITY_LAST = P_EVENT_PRIORITY_LOW + 1,

    P_EVENT_PRIORITY_NUMBER = P_EVENT_PRIORITY_LAST - P_EVENT_PRIORITY_FIRST + 1,
};

class PObject;
class P_DLLEXPORT PEvent
{
    friend class PEventManager;

private:
    PEvent(const PEvent &other) {}
    void operator=(const PEvent &other) {}

    PEvent(PEventTypeEnum type, PObject *sender, puint32 timestamp, PEventManager *manager);
    ~PEvent();
    P_INLINE pbool canFire() { return m_fireTime == 0; } 

public:
    void release();
    P_INLINE PEventTypeEnum getType() { return m_type; }
    void setPriority(PEventPriorityEnum priority);
    void setNeedForwarding(pbool value);

    void sendImmediate(PObject *receiver);
    void queue(PObject *receiver, puint32 after = 0);

    void setParameter(PEventParameterEnum parameterName, pfloat32 value);
    void setParameter(PEventParameterEnum parameterName, pint32 value);
    void setParameter(PEventParameterEnum parameterName, pbool value);
    void setParameter(PEventParameterEnum parameterName, void *value);
    void setParameter(PEventParameterEnum parameterName, puint32 value);
    void setParameter(PEventParameterEnum parameterName, const void* value);

    PVariant& parameter(PEventParameterEnum parameterName);

    P_INLINE PObject *sender() const
    { return m_sender; }
    P_INLINE PObject *receiver() const
    { return m_receiver; }
    P_INLINE pbool hasHandled() const
    { return m_handled; }
    P_INLINE PEventPriorityEnum priority() const
    { return m_priority; }
    P_INLINE PEventTypeEnum type() const
    { return m_type; }
    P_INLINE puint32 time() const 
    { return m_timestamp; }

private:
    PEventManager       *m_manager;

    PEventPriorityEnum  m_priority;
    PEventTypeEnum      m_type;
    PObject            *m_sender;
    PObject            *m_receiver;
    pbool                m_handled;
    puint32             m_fireTime;
    pbool                m_needForwarding;
    puint32             m_timestamp;

    PVariant            m_parameters[P_EVENT_PARAMETER_MAX_NUMBER];
    static PVariant     s_nil;
};


#endif // !PEVENT_H
