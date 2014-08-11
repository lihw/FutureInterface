// pnetworkclient.cpp
// The network client node 
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#include "pnetworkclient.h"

#include <PFoundation/pcontext.h>
#include <PFoundation/pnetworkmanager.h>
#include <PFoundation/pclock.h>

#include <enet/enet.h>

struct PNetworkClientPrivateStruct
{
    ENetHost *client;
    ENetPeer  *peer;
    ENetEvent  event;
    ENetPacket *packet;
};

PNetworkClient::PNetworkClient(const pchar *address, puint16 port, puint32 timeout, PNetworkManager *manager)
    : PNetworkNode(NETWORK_CLIENT, manager)
    , m_address(address)
    , m_port(port)
    , m_timeout(timeout)
{
    m_data = PNEW(PNetworkClientPrivateStruct);
    m_data->packet = enet_packet_create(NULL, P_NETWORK_MAX_PACKET_SIZE, ENET_PACKET_FLAG_RELIABLE);
    m_data->packet->referenceCount = 0x0fffffff; // This packet should never be destroyed

    connect(address, port);
}

PNetworkClient::~PNetworkClient()
{
    disconnect();

    enet_packet_destroy(m_data->packet);
    PDELETE(m_data);
}
    
void PNetworkClient::onMessageReceived(const puint8 *message, puint32 length)
{
}

void PNetworkClient::onConnected()
{
}

void PNetworkClient::onDisconnected()
{
}

void PNetworkClient::onConnectFailed()
{
}

pbool PNetworkClient::connect(const pchar *address, puint16 port)
{
    if (m_state == NETWORK_DISCONNECTED)
    {
        m_data->client = enet_host_create(NULL, 1, 1, 0, 0);

        if (m_data->client== NULL) 
        {
            PLOG_ERROR("Could not create the newwork client.");
            return false;
        }

        ENetAddress addr;
        enet_address_set_host(&addr, address);
        addr.port = port;

        m_data->peer = enet_host_connect(m_data->client, &addr, 1, P_NULL);
        
        m_state = NETWORK_CONNECTING;
        
        m_connectingTime = 0;

        return true;
    }

    return false;
}
    
void PNetworkClient::disconnect()
{
    if (m_state == NETWORK_CONNECTED ||
        m_state == NETWORK_CONNECTING ||
        m_state == NETWORK_ERROR)
    {
        enet_peer_reset(m_data->peer);
        enet_host_destroy(m_data->client);
        m_state = NETWORK_DISCONNECTED;
    }
}

void PNetworkClient::send(const puint8 *message, pint32 length)
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

        if (enet_peer_send(m_data->peer, 0, m_data->packet) < 0)
        {
            PLOG_ERROR("Failed to send message to server.");
        }
    }
}

void PNetworkClient::update()
{
    if (m_state == NETWORK_CONNECTING ||
        m_state == NETWORK_CONNECTED)
    {
        pint32 ret;
        // FIXME: why enet_host_check_events doesn't work as enet_host_service()?
        //while ((ret = enet_host_check_events(m_data->client, &m_data->event)) > 0)
        while ((ret = enet_host_service(m_data->client, &m_data->event, P_NETWORK_POLL_TIME)) > 0)
        {
            switch (m_data->event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                    m_state = NETWORK_CONNECTED;
                    onConnected();
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                    onMessageReceived(m_data->event.packet->data, m_data->event.packet->dataLength);
                    enet_packet_destroy (m_data->event.packet);
                    break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    m_state = NETWORK_DISCONNECTED;
                    onDisconnected();
                    break;
                default:
                    break;
            }
        }

        if (m_state == NETWORK_CONNECTING)
        {
            m_connectingTime += m_manager->context()->clock().systemDeltaTime();
            if (m_connectingTime > (pfloat32)m_timeout)
            {
                onConnectFailed();

                disconnect();
            }
        }
    }
}
    
pbool PNetworkClient::resume()
{
    return connect(m_address.c_str(), m_port);
}

void PNetworkClient::pause()
{
    disconnect();
}
