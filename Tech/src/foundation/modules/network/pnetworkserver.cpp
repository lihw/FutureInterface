// pnetworkserver.cpp
// The network server node 
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "pnetworkserver.h"

PNetworkServer::PNetworkServer(const pchar *address, PNetworkManager *manager)
    : PNetworkNode(NETWORK_SERVER, address, manager)
{
}

PNetworkServer::~PNetworkServer()
{
}


