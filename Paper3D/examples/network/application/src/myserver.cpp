// myserver.cpp
// "Network" example
//
// Copyright 2012 - 2014 Future Interface. All rights reserved.
//
// Future Interface  lihw81@gmail.com
//

#include "myserver.h"

#include <PFoundation/pcrt.h>
#include <PFoundation/plog.h>


MyServer::MyServer(const pchar *address, PNetworkManager *networkManager)
    : PNetworkServer(address, networkManager)
{
}

MyServer::~MyServer()
{
}

void MyServer::onReceiveMessage(const pchar *message, puint32 length)
{
    pchar msg[4096];
    pstrncmp(msg, message, length);
    msg[length] = 0;
    PLOG_INFO(msg);
}
