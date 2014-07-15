// pevent.cpp
// The base class of events
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com 
//
// 

#include "pevent.h"

PVariant PEvent::s_nil;
    
PEvent::PEvent(PEventTypeEnum type, PObject *sender, puint32 timestamp, PEventManager *manager)
{
    m_type = type;
    m_sender = sender;
    m_manager = manager;
    m_timestamp = timestamp;
    m_priority = P_EVENT_PRIORITY_DEFAULT;
    m_needForwarding = false;
    m_handled = false;
}

PEvent::~PEvent()
{

}

void PEvent::release()
{
    m_manager->recycelEvent(this);
}

void PEvent::setPriority(PEventPriorityEnum priority)
{
    m_priority = priority;
}

void PEvent::setNeedForwarding(pbool value)
{
    m_needForwarding = value;
}

void PEvent::sendImmediate(PObject *receiver)
{
    m_receiver = receiver;
    m_manager->fire(this);
}

void PEvent::queue(PObject *receiver, puint32 after)
{
    m_receiver = receiver;
    m_fireTime = after;
    m_manager->queueEvent(this);
}

void PEvent::setParameter(PEventParameterEnum parameterName, pfloat32 value)
{
    PASSERTINFO(parameterName < P_EVENT_PARAMETER_MAX_NUMBER, "too many parameters");
    if (parameterName >= P_EVENT_PARAMETER_MAX_NUMBER)
    {
        PLOG_ERROR("too many parameters");
        return ;
    }
    m_parameters[parameterName] = PVariant(value);
}

void PEvent::setParameter(PEventParameterEnum parameterName, const void* value)
{
    PASSERTINFO(parameterName < P_EVENT_PARAMETER_MAX_NUMBER, "too many parameters");
    if (parameterName >= P_EVENT_PARAMETER_MAX_NUMBER)
    {
        PLOG_ERROR("too many parameters");
        return ;
    }
    m_parameters[parameterName] = PVariant(value);
}
void PEvent::setParameter(PEventParameterEnum parameterName, pint32 value)
{
    PASSERTINFO(parameterName < P_EVENT_PARAMETER_MAX_NUMBER, "too many parameters");
    if (parameterName >= P_EVENT_PARAMETER_MAX_NUMBER)
    {
        PLOG_ERROR("too many parameters");
        return ;
    }
    PVariant variant = PVariant(value);
    m_parameters[parameterName] = variant;
}
void PEvent::setParameter(PEventParameterEnum parameterName, puint32 value)
{
     PASSERTINFO(parameterName < P_EVENT_PARAMETER_MAX_NUMBER, "too many parameters");
    if (parameterName >= P_EVENT_PARAMETER_MAX_NUMBER)
    {
        PLOG_ERROR("too many parameters");
        return ;
    }
    PVariant variant = PVariant(value);
    m_parameters[parameterName] = variant;
}
void PEvent::setParameter(PEventParameterEnum parameterName, pbool value)
{
    PASSERTINFO(parameterName < P_EVENT_PARAMETER_MAX_NUMBER, "too many parameters");
    if (parameterName >= P_EVENT_PARAMETER_MAX_NUMBER)
    {
        PLOG_ERROR("too many parameters");
        return ;
    }
    PVariant dataValue = PVariant(value);
    m_parameters[parameterName] = dataValue;
}

void PEvent::setParameter(PEventParameterEnum parameterName, void *value)
{
    PASSERTINFO(parameterName < P_EVENT_PARAMETER_MAX_NUMBER, "too many parameters");
    if (parameterName >= P_EVENT_PARAMETER_MAX_NUMBER)
    {
        PLOG_ERROR("too many parameters");
        return ;
    }
    PVariant dataValue = PVariant(value);
    m_parameters[parameterName] = dataValue;
}

PVariant& PEvent::parameter(PEventParameterEnum parameterName)
{
    PASSERTINFO(parameterName < P_EVENT_PARAMETER_MAX_NUMBER, "too many parameters");
    if (parameterName >= P_EVENT_PARAMETER_MAX_NUMBER)
    {
        PLOG_ERROR("too many parameters");
        return s_nil;
    }
    return m_parameters[parameterName];
}
