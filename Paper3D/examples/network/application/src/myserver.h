// myserver.h
// "Network" example
//
// Copyright 2012 - 2014 Future Interface. All rights reserved.
//
// Future Interface  lihw81@gmail.com
//

#ifndef MYSERVER_H
#define MYSERVER_H

#include <PFoundation/pnetworkserver.h>

class MyServer : public PNetworkServer
{
public:
    MyServer(const pchar *address, PNetworkManager *networkManager);
    virtual ~MyServer();

    virtual void onReceiveMessage(const pchar *message, puint32 length);
};

#endif //!MYSERVER_H
