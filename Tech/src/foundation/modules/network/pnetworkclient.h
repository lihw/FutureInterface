// pnetworkclient.h
// The network client node 
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#ifndef PNETWORKCLIENT_H
#define PNETWORKCLIENT_H

#include "pnetworknode_private.h"

struct PNetworkClientPrivateStruct;

class P_DLLEXPORT PNetworkClient : public PNetworkNode
{
public:
    // timeout is the maximum time in milliseconds for establishing the connection.
    PNetworkClient(const pchar *address, puint16 port, puint32 timeout, PNetworkManager *manager);
    virtual ~PNetworkClient();
    
    // When a message arrives at the client from the host.
    virtual void onMessageReceived(const puint8 *message, puint32 length);
    virtual void onConnected();
    virtual void onDisconnected();
    virtual void onConnectFailed();

    void send(const puint8 *message, pint32 length = -1);

protected:
    virtual void update();
    virtual pbool resume();
    virtual void pause();

    pbool connect(const pchar *address, puint16 port);
    void disconnect();

protected:
    PString                      m_address;
    puint16                      m_port;

private:
    PNetworkClientPrivateStruct *m_data;
    pint32                       m_timeout;
    pfloat32                     m_connectingTime;
};



#endif // !PNETWORKCLIENT_H
