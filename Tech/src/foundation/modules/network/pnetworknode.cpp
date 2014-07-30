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


extern "C" 
{
#define NN_HAVE_WINDOWS
#include <nn.h>
#include <survey.h>
};

PNetworkNode::PNetworkNode(NodeTypeEnum type, const pchar *address,PNetworkManager *manager)
    : m_address(address)
{
    m_type = type;
    
    m_socket = -1;

    // Validate the address
    pbool validAddress = true;
    const pchar *addr = pStringTrim(address);
    if (pstrncmp(addr, "tcp://", 6) != 0)
    {
        PLOG_ERROR("%s is not a valid TCP address.", address);
        validAddress = false;
    }
    const pchar *port = pstrchr(address, ':');
    pint32 p = 0xffff + 1;
    const pchar *ret = pStringUnpackInt(port, &p);
    if (p == P_NULL || (p > 0xffff || p <= 0))
    {
        PLOG_ERROR("%s is not a valid TCP address.", address);
        validAddress = false;
    }
        
    m_state = NETWORK_NOTCONNECTED;
    
    m_bufferSize   = 4096;
    m_successful   = false;
    m_resendTimes  = 0;
    m_buffer       = PNEWARRAY(pchar, m_bufferSize);
    m_bufferLength = 0;

    if (validAddress)
    {
        PASSERT(manager != P_NULL);
        manager->addNode(this);

        resume();
    }
}

PNetworkNode::~PNetworkNode()
{
    if (m_socket >= 0)
    {
        nn_shutdown(m_socket, 0); 
        m_socket = -1;
    }

    PDELETEARRAY(m_buffer);
}
    
pbool PNetworkNode::resume()
{
    PASSERT(m_socket < 0);

    if (m_type == NETWORK_SERVER)
    {
        m_socket = nn_socket(AF_SP, NN_SURVEYOR);
        PASSERT(m_socket >= 0);
        if (m_socket < 0)
        {
            PLOG_ERROR("Failed to create socket for network server.");
            return false;
        }
        if (nn_bind(m_socket, m_address.c_str()) < 0)
        {
            PLOG_ERROR("Failed to bind to %s as a server.", m_address.c_str());
            nn_shutdown(m_socket, 0); 
            m_socket = -1;
            return false;
        }
    }
    else
    {
        m_socket = nn_socket(AF_SP, NN_RESPONDENT);
        PASSERT(m_socket >= 0);
        if (m_socket < 0)
        {
            PLOG_ERROR("Failed to create socket for network client.");
            return false;
        }
        if (nn_connect(m_socket, m_address.c_str()) < 0)
        {
            PLOG_ERROR("Failed to connect to %s.", m_address.c_str());
            nn_shutdown(m_socket, 0); 
            m_socket = -1;
            return false;
        }
    }
        
    m_state = NETWORK_CONNECTED;

    return true;
}

void PNetworkNode::pause()
{
    if (m_socket >= 0)
    {
        nn_shutdown(m_socket, 0);
        m_socket = -1;
        m_state = NETWORK_NOTCONNECTED;
    }
}

void PNetworkNode::setMaxMessageLength(puint32 value)
{
    m_bufferSize = value;

    if (value > m_bufferSize)
    {
        PDELETEARRAY(m_buffer);
        m_buffer = PNEWARRAY(pchar, m_bufferSize);
    }
}

void PNetworkNode::setResendTimes(puint32 value)
{
    m_resendTimes = value;
}

void PNetworkNode::update()
{
    if (m_socket >= 0 && m_state == NETWORK_CONNECTED)
    {
        while (true)
        {
            pint32 nbytes = nn_recv(m_socket, m_buffer, m_bufferSize, NN_DONTWAIT);
            if (nbytes < 0)
            {
                pint32 err = nn_errno();
                if (err != EAGAIN)
                {
                    PLOG_ERROR("Network server is not working normally.");
                    m_state = NETWORK_ERROR;
                    break;
                }
            }
            else
            {
                onReceiveMessage(m_buffer, nbytes);
            }
        }
    }
}

void PNetworkNode::sendMessage(const pchar *message, puint32 length)
{
    if (m_socket >= 0 && m_state == NETWORK_CONNECTED)
    {
        if (length == 0)
        {
            length = pstrlen(message);
        }

        pint32 nbytes = nn_send(m_socket, message, length, NN_DONTWAIT);
        puint32 times = m_resendTimes;
        while (times > 0 && nbytes < 0)
        {
            pint32 err = nn_errno();
            if (err == EAGAIN ||
                err == ETIMEDOUT ||
                err == EINTR)
            {
                nbytes = nn_send(m_socket, message, length, NN_DONTWAIT);
            }
            else 
            {
                PLOG_ERROR("Network client doesn't have an active connection.");
                m_state = NETWORK_ERROR;
                break;
            }

            times--;
        }

        m_successful = (nbytes > 0);
    }
}

void PNetworkNode::onReceiveMessage(pchar *message, puint32 length)
{
}
