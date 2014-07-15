// pentity.cpp
// The base class of all objects which needs
// runtime type information and reference count.
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
// 

#include "PEntity.h"

#include <PFoundation/passert.h>
#include <PFoundation/pnew.h>

PEntity::PEntity()
{
    m_ref = 0;   
}

PEntity::~PEntity()
{
    PASSERT(m_ref == 0);
}

void PEntity::retain()
{
    // FIXME: make the following line thread-safe
    ++m_ref;
}

void PEntity::release()
{
    // FIXME: make the following line thread-safe
    PASSERT(m_ref > 0);
    
    --m_ref; 
    
    if (m_ref == 0)
    {
        PDELETETHIS(this);
    }
}
