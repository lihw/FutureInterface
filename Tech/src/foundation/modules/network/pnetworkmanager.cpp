// pnetworkmanager.h
// The network manager
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#include "pnetworkmanager.h"

#include "pnetworknode_private.h"

#include <PFoundation/PContext.h>

#include <PFoundation/pnew.h>
#include <PFoundation/plog.h>
#include <PFoundation/passert.h>


PNetworkManager::PNetworkManager(PContext *context)
    : PModule("network-manager", context)
{  
    m_node = P_NULL;
}

PNetworkManager::~PNetworkManager()
{
    PASSERT(m_node == P_NULL);
}
    
void PNetworkManager::uninitialize()
{
    PDELETE(m_node);
}

void PNetworkManager::update()
{
    if (m_node != P_NULL)
    {
        m_node->update();
    }
}

pbool PNetworkManager::resume()
{
    if (m_node != P_NULL)
    {
        return m_node->resume();
    }

    return true;
}

void PNetworkManager::pause()
{
    if (m_node != P_NULL)
    {
        m_node->pause();
    }
}

void PNetworkManager::addNode(PNetworkNode *node)
{
    if (m_node == P_NULL)
    {
        m_node = node;
    }
    else
    {
        PLOG_ERROR("The network has been initialized");
    }
}

void PNetworkManager::removeNode(PNetworkNode *node)
{
    m_node = P_NULL;
}

