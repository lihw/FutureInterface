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
public:
    enum NodeTypeEnum
    {
        NETWORK_CLIENT,
        NETWORK_SERVER,
    };
    enum NodeStateEnum
    {
        NETWORK_NOTCONNECTED,
        NETWORK_CONNECTED,     // Connected
        NETWORK_ERROR,         // Connected but has an error in sending or receiving 
    };

    // address is in format like tcp://localhost:6666 or tcp:192.168.0.1:8888
    PNetworkNode(NodeTypeEnum type, const pchar *address, 
        PNetworkManager *manager);
    virtual ~PNetworkNode();
    
    virtual pbool resume();
    virtual void pause();
    virtual void update();

    void sendMessage(const pchar *message, puint32 length = 0);
    // Upon message arrives.
    virtual void onReceiveMessage(pchar *message, puint32 length);

    P_INLINE NodeTypeEnum type() const
    { return m_type; }
    P_INLINE NodeStateEnum state() const
    { return m_state; }
    
    // The length of the message buffer. The default is 4096.
    void setMaxMessageLength(puint32 value);
    // If the sending fails, how many times we redo the sending. The default
    // is zero which means we don't do any resending.
    void setResendTimes(puint32 value);
    // If the last sending succeeds
    P_INLINE pbool isSent() const 
    { return m_successful; }
    P_INLINE puint32 maxMessageLength() const 
    { return m_bufferSize; }

protected:
    NodeTypeEnum  m_type;
    NodeStateEnum m_state;
    PString       m_address;
    pint32        m_socket;
    pchar        *m_buffer;
    puint32       m_bufferSize;   // The received buffer size
    puint32       m_bufferLength; // The length of the received message. 
    pbool         m_successful;   // Sending is successful?
    puint32       m_resendTimes;  // How many times we redo the resending.
};



#endif // !PNETWORKNODE_PRIVATE_H
