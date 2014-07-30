// myclient.h
// "Network" example
//
// Copyright 2012 - 2014 Future Interface. All rights reserved.
//
// Future Interface  lihw81@gmail.com
//

#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <PFoundation/pnetworkclient.h>

class MyClient : public PNetworkClient
{
public:
    MyClient(const pchar *address, PNetworkManager *networkManager);
    virtual ~MyClient();
};

#endif //!MYCLIENT_H
