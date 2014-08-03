// pnetworkserver.h
// The network server node 
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#ifndef PNETWORKSERVER_H
#define PNETWORKSERVER_H

#include "pnetworknode_private.h"

#include <PFoundation/pmap.h>

// TODO: heart beat mechanism to test if the client is still alive.

struct PNetworkPeerPrivateStruct;
struct PNetworkServerPrivateStruct;

class P_DLLEXPORT PNetworkPeer
{
public:
    PNetworkPeer(puint32 id);
    virtual ~PNetworkPeer();

    P_INLINE puint32 id() const
    { return m_id; }
    P_INLINE PNetworkPeerPrivateStruct *data() const
    { return m_data; }

private:
    puint32 m_id;
    PNetworkPeerPrivateStruct *m_data;
};

class P_DLLEXPORT PNetworkServer : public PNetworkNode
{
public:
    PNetworkServer(puint16 port, PNetworkManager *manager);
    virtual ~PNetworkServer();

    // When a message arrives at host from peer.
    virtual void onMessageReceived(PNetworkPeer *peer, const puint8 *message, puint32 length);
    // When a peer disconnects from the host.
    virtual void onPeerDisconnected(PNetworkPeer *peer);
    // When a peer connects to the host
    virtual PNetworkPeer *onPeerConnected(puint32 id);
    
    // Send the message to specified destination.
    pbool send(PNetworkPeer *peer, const puint8 *message, pint32 length = -1);
    // Broadcast the message.
    void broadcast(const puint8 *message, pint32 length = -1);
    // Disconnect a peer
    void disconnect(PNetworkPeer *peer);
    
protected:
    virtual pbool resume();
    virtual void pause();
    virtual void update();

    // Start the server
    pbool listen(puint16 port);
    // Shutdown the server
    void shutdown();

private:
    PNetworkServerPrivateStruct  *m_data;
    PMap<puint32, PNetworkPeer*>  m_peers;
    puint32                       m_nextId;
    puint16                       m_port;
};

#endif // !PNETWORKSERVER_H
