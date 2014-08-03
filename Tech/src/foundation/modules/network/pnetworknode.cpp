// pnetworknode.cpp
// The network node base class
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#include "pnetworknode_private.h"

#include <PFoundation/pconststring.h>
#include <PFoundation/pnetworkmanager.h>

PNetworkNode::PNetworkNode(NodeTypeEnum type, PNetworkManager *manager)
{
    m_type = type;
    m_manager = manager;    
    m_state = NETWORK_DISCONNECTED;
    
    PASSERT(manager != P_NULL);
    manager->addNode(this);
}

PNetworkNode::~PNetworkNode()
{
    m_manager->removeNode(this);
}
    
