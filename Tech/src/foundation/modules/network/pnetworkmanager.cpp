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
    m_nodes[0] = P_NULL;
    m_nodes[1] = P_NULL;
}

PNetworkManager::~PNetworkManager()
{
    PASSERT(m_nodes[0] == P_NULL);
    PASSERT(m_nodes[1] == P_NULL);
}

void PNetworkManager::uninitialize()
{
    PDELETE(m_nodes[0]);
    PDELETE(m_nodes[1]);
}

void PNetworkManager::update()
{
    if (m_nodes[0] != P_NULL)
    {
        m_nodes[0]->update();
    }
    if (m_nodes[1] != P_NULL)
    {
        m_nodes[1]->update();
    }
}

pbool PNetworkManager::resume()
{
    pbool ret = true;
    if (m_nodes[0] != P_NULL)
    {
        ret = m_nodes[0]->resume();
    }
    if (ret && m_nodes[1] != P_NULL)
    {
        ret = m_nodes[1]->resume();
    }

    return ret;
}

void PNetworkManager::pause()
{
    if (m_nodes[0] != P_NULL)
    {
        return m_nodes[0]->pause();
    }
    if (m_nodes[1] != P_NULL)
    {
        return m_nodes[1]->pause();
    }
}

void PNetworkManager::addNode(PNetworkNode *node)
{
    if (m_nodes[0] != P_NULL)
    {
        m_nodes[0] = node;
    }
    else
    {
        if (m_nodes[1] != P_NULL)
        {
            PLOG_ERROR("Network manager can't host more than two network nodes.");
            PASSERT_NOTREACHABLE("Network manager can't host more than two network nodes.");
        }
        else 
        {
            if (m_nodes[0]->type() == node->type())
            {
                PLOG_ERROR("Network manager can't host two network %s.",
                    m_nodes[0]->type() == PNetworkNode::NETWORK_SERVER? "server" : "client");
                PASSERT_NOTREACHABLE("Network manager can't host two same network nodes.");
            }
            else
            {
                m_nodes[1] = node;
            }
        }
    }
}

