// pabstractgesture.h
// The gesture base header file.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "pgesturemanager.h"
#include "pgesture_private.h"

#include <PFoundation/PContext.h>
#include <PFoundation/pgesturemanager.h>

#include <PFoundation/passert.h>

PAbstractGesture::PAbstractGesture(PGestureManager *manager, PGestureTypeEnum type) 
{ 
    PASSERT(manager != P_NULL);
    m_manager = manager; 
    m_type = type; 
    m_enabled = true;
    m_state = STATE_READY;
}
    
PAbstractGesture::~PAbstractGesture()
{
}
    
void PAbstractGesture::setEnabled(pbool enabled)
{
    if (m_enabled != enabled)
    {
        if (m_enabled)
        {
            // FIXME: should trigger end events 
            // when gesture is active.
            reset();
        }

        m_enabled = enabled;
    }
}
    
PEvent *PAbstractGesture::createEvent(PEventTypeEnum type)
{
    return m_manager->context()->eventManager()->createEvent(type, P_NULL);
}
