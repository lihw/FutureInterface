// pactivity.cpp
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pactivity.h"

#include <PFoundation/PContext.h>
#include <PFoundation/pnew.h>
#include <PFoundation/passert.h>
#include <PFoundation/pnetwork.h>

PActivity *PActivity::s_activity = P_NULL;


PActivity::PActivity(pint32 argc, pchar** argv)
{
    // In spite of value assignments, don't put any module initialization code
    // here.
    PASSERT(s_activity == P_NULL);
    if (s_activity == P_NULL)
    {
        m_contextCount = 0;

        s_activity = this;
    }
    
    // Initialize contexts.
    for (puint32 i = 0; i < P_CONTEXT_MAXIMUM_NUMBER; ++i)
    {
        m_contexts[i] = P_NULL;
    }
}

PActivity::~PActivity()
{
}

pbool PActivity::initialize()
{
    // TODO: Put the global initialize code here.
    pInitializeLogSystem();
    
    // TODO: enable network module under iOS
#if !defined P_IOS
    //pInitializeNetwork();
#endif

    return true;
}

void PActivity::uninitialize()
{
    // TODO: Put the global uninitialize code here.
    
    // TODO: enable network module under iOS
#if !defined P_IOS
    //pUninitializeNetwork();
#endif

    pUninitializeLogSystem();
    
    // Delete contexts
    for (puint32 i = 0; i < m_contextCount; ++i)
    {
        PDELETE(m_contexts[i]);
    }
    m_contextCount = 0;
}

pbool PActivity::addContext(PContext* context)
{
    // Check if the context with the same name existing.
    for (puint32 i = 0; i < P_CONTEXT_MAXIMUM_NUMBER; ++i)
    {
        if (m_contexts[i] != P_NULL)
        {
            if (m_contexts[i]->name() == context->name())
            {
                return false;
            }
        }
    }

    // Check if there is still some space to have a new context.
    if (m_contextCount < P_CONTEXT_MAXIMUM_NUMBER)
    {
        // Find a valid position.
        for (puint32 i = 0; i < P_CONTEXT_MAXIMUM_NUMBER; i++)
        {
            if (m_contexts[i] == P_NULL)
            {
                m_contexts[i] = context;
                m_contextCount++;
                break;
            }
        }
        return true;
    }

    return false;
}

PContext* PActivity::findContext(const pchar *contextName) const
{
    for (puint32 i = 0; i < P_CONTEXT_MAXIMUM_NUMBER; ++i)
    {
        // Find the context by name 
        if (m_contexts[i] != P_NULL)
        {
            if (m_contexts[i]->name() == contextName)
            {
                return m_contexts[i];
            }
        }
    }
    return P_NULL;
}

puint32 PActivity::findContextId(const pchar *contextName) const
{
    for (puint32 i = 0; i < P_CONTEXT_MAXIMUM_NUMBER; ++i)
    {
        // Find the context by name 
        if (m_contexts[i] != P_NULL)
        {
            if (m_contexts[i]->name() == contextName)
            {
                return i;
            }
        }
    }
    return P_CONTEXT_INVALID_ID;
}

PContext* PActivity::findContext(puint32 contextId) const
{
    if (contextId >= P_CONTEXT_MAXIMUM_NUMBER)
    {
       return P_NULL;
    }

    return m_contexts[contextId];
}
    
void PActivity::removeContext(PContext* context)
{
    PASSERT(context != P_NULL);
    
    if (context != P_NULL)
    {
        for (puint32 i = 0; i < P_CONTEXT_MAXIMUM_NUMBER; ++i)
        {
            if (m_contexts[i] == context)
            {
                m_contexts[i] = P_NULL;
                m_contextCount -= 1;
                break;
            }
        }        
    }
}

void PActivity::removeContext(puint32 contextId)
{
    PASSERT(m_contexts[contextId] != P_NULL);

    if (m_contexts[contextId] != P_NULL)
    {
        m_contexts[contextId] = P_NULL;
        m_contextCount -= 1;
    }
}

void PActivity::removeContext(const pchar* contextName)
{
    puint32 i = 0;
    for (i = 0; i < P_CONTEXT_MAXIMUM_NUMBER; ++i)
    {
        if (m_contexts[i] != P_NULL)
        {
            if (m_contexts[i]->name() == contextName)
            {
                m_contexts[i] = P_NULL;            
                m_contextCount -= 1;
                break;
            }
        }
    }
}

PActivity * P_APIENTRY pGetActivity()
{
    return PActivity::s_activity;
}

