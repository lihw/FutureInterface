// pnetworknode_private.h
// The network node base class
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#ifndef PNETWORKNODE_PRIVATE_H
#define PNETWORKNODE_PRIVATE_H

#include <PFoundation/pstring.h>

class PNetworkManager;

class P_DLLEXPORT PNetworkNode
{
    friend class PNetworkManager;

public:
    enum NodeTypeEnum
    {
        NETWORK_CLIENT,
        NETWORK_SERVER,
    };
    enum NodeStateEnum
    {
        NETWORK_DISCONNECTED,
        NETWORK_CONNECTING,
        NETWORK_CONNECTED,     // Connected
        NETWORK_ERROR,         // Connected but has an error in sending or receiving 
    };

    // address is in format like a URL or a IPv4 address.
    PNetworkNode(NodeTypeEnum type, PNetworkManager *manager);
    virtual ~PNetworkNode();
    
    P_INLINE NodeTypeEnum type() const
    { return m_type; }
    P_INLINE NodeStateEnum state() const
    { return m_state; }

protected:
    virtual pbool resume() = 0;
    virtual void pause() = 0;
    virtual void update() = 0;

protected:
    NodeTypeEnum     m_type;
    NodeStateEnum    m_state;
    PNetworkManager *m_manager;
};



#endif // !PNETWORKNODE_PRIVATE_H
