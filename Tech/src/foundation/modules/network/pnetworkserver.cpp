// pnetworkserver.cpp
// The network server node 
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "pnetworkserver.h"

#include <enet/enet.h>

struct PNetworkServerPrivateStruct
{
    ENetHost  *server;
    ENetEvent  event;
    ENetPacket *packet;
};

struct PNetworkPeerPrivateStruct
{
    ENetPeer *peer;
};
    
//
// class PNetworkPeer
//
PNetworkPeer::PNetworkPeer(puint32 id)
{
    m_data = PNEW(PNetworkPeerPrivateStruct);
    m_id = id;
}

PNetworkPeer::~PNetworkPeer()
{
    PDELETE(m_data);
}

//
// class PNetworkServer
//

PNetworkServer::PNetworkServer(puint16 port, PNetworkManager *manager)
    : PNetworkNode(NETWORK_SERVER, manager)
{
    m_data = PNEW(PNetworkServerPrivateStruct);
    m_data->packet = enet_packet_create(NULL, P_NETWORK_MAX_PACKET_SIZE, ENET_PACKET_FLAG_RELIABLE);
    m_data->packet->referenceCount = 0x0fffffff; // This packet should never be destroyed

    m_nextId = 1;
    m_port = port;

    listen(m_port);
}

PNetworkServer::~PNetworkServer()
{
    shutdown();

    enet_packet_destroy(m_data->packet);

    PDELETE(m_data);
}

void PNetworkServer::onMessageReceived(PNetworkPeer *peer, const puint8 *message, puint32 length)
{
}

void PNetworkServer::onPeerDisconnected(PNetworkPeer *peer)
{
}

PNetworkPeer *PNetworkServer::onPeerConnected(puint32 id)
{
    return PNEW(PNetworkPeer(id));
}
    
pbool PNetworkServer::resume()
{
    return listen(m_port);
}

void PNetworkServer::pause()
{
    shutdown();
}

pbool PNetworkServer::listen(puint16 port)
{
    if (m_state == NETWORK_DISCONNECTED)
    {
        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = port;
        // FIXME: unlimited incoming and outgoing bandwidth
        m_data->server = enet_host_create(&address, P_NETWORK_MAX_CONNECTIONS, 1, 0, 0);
        if (m_data->server == P_NULL)
        {
            PLOG_ERROR("Failed to start network server.");
            return false;
        }

        m_state = NETWORK_CONNECTED;
    }

    return true;
}

void PNetworkServer::shutdown()
{
    if (m_state == NETWORK_CONNECTED ||
        m_state == NETWORK_ERROR)
    {
        PMap<puint32, PNetworkPeer*>::iterator it = m_peers.begin();
        PMap<puint32, PNetworkPeer*>::iterator ie = m_peers.end();
        while (it != ie)
        {
            enet_peer_disconnect_now(it.value()->data()->peer, it.key());
            PDELETE(it.value());
            ++it;
        }

        enet_host_destroy(m_data->server);

        m_state = NETWORK_DISCONNECTED;
    }
}

pbool PNetworkServer::send(PNetworkPeer *peer, const puint8 *message, pint32 length)
{
    if (m_state == NETWORK_CONNECTED)
    {
        PMap<puint32, PNetworkPeer*>::iterator it = m_peers.find(peer->id());
        if (it != m_peers.end())
        {
            if (length == -1)
            {
                m_data->packet->dataLength = pstrlen((const pchar *)message) + 1;
            }
            else
            {
                m_data->packet->dataLength = length;
            }
            
            pmemcpy(m_data->packet->data, message, m_data->packet->dataLength);
            if (enet_peer_send(peer->data()->peer, 0, m_data->packet) < 0)
            {
                PLOG_ERROR("Failed to send message to peer (%d).", peer->id());
                return false;
            }

            return true;
        }
    }

    return false;
}
    
void PNetworkServer::disconnect(PNetworkPeer *peer)
{
    PASSERT_NOTIMPLEMENTED();
}

void PNetworkServer::broadcast(const puint8 *message, pint32 length)
{
    if (m_state == NETWORK_CONNECTED)
    {
        if (length == -1)
        {
            m_data->packet->dataLength = pstrlen((const pchar *)message) + 1;
        }
        else
        {
            m_data->packet->dataLength = length;
        }
        pmemcpy(m_data->packet->data, message, m_data->packet->dataLength);
        enet_host_broadcast(m_data->server, 0, m_data->packet);
        enet_host_flush(m_data->server);
    }
}
    
void PNetworkServer::update()
{
    if (m_state == NETWORK_CONNECTED)
    {
        pint32 ret;
        while ((ret = enet_host_service(m_data->server, &m_data->event, P_NETWORK_POLL_TIME)) > 0)
        // FIXME: why enet_host_check_events doesn't work as enet_host_service()?
        //while ((ret = enet_host_check_events(m_data->server, &m_data->event)) > 0)
        {
            switch (m_data->event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                    {
                        puint32 id = m_nextId++;
                        PNetworkPeer *peer = onPeerConnected(id);
                        if (peer != P_NULL)
                        {
                            m_peers.insert(id, peer);
                            peer->data()->peer = m_data->event.peer;
                        }
                        m_data->event.peer->data = PNEW(puint32);
                        *(puint32*)(m_data->event.peer->data) = id;
                    }
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    {
                        puint32 id = *(puint32*)(m_data->event.peer->data);
                        PMap<puint32, PNetworkPeer*>::iterator it = m_peers.find(id);
                        if (it != m_peers.end())
                        {
                            onMessageReceived(it.value(), (const puint8 *)(m_data->event.packet->data), 
                                m_data->event.packet->dataLength);
                        }
                        else
                        {
                            PLOG_ERROR("Failed to find network connection %d.", id);
                        }
                        enet_packet_destroy (m_data->event.packet);
                    }
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    {
                        puint32 id = *(puint32*)(m_data->event.peer->data);
                        PMap<puint32, PNetworkPeer*>::iterator it = m_peers.find(id);
                        if (it != m_peers.end())
                        {
                            onPeerDisconnected(it.value());
                            PDELETE(it.value());
                            m_peers.erase(it);
                            puint32 *tmp = (puint32*)(m_data->event.peer->data);
                            PDELETE(tmp);
                        }
                        else
                        {
                            PLOG_ERROR("Failed to find network connection %d.", id);
                        }
                    }
                    break;
                default:
                    PASSERT_NOTREACHABLE("Unknown server side event.");
                    break;
            }
        }

        if (ret < 0)
        {
            m_state = NETWORK_ERROR;
        }
    }
}
