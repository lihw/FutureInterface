// myclient.cpp
// "Network" example
//
// Copyright 2012 - 2014 Future Interface. All rights reserved.
//
// Future Interface  lihw81@gmail.com
//

#include "myclient.h"
    
MyClient::MyClient(const pchar *address, PNetworkManager *networkManager)
    : PNetworkClient(address, networkManager)
{
}

MyClient::~MyClient()
{
}
