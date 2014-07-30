// pnetworkclient.cpp
// The network client node 
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#include "pnetworkclient.h"


PNetworkClient::PNetworkClient(const pchar *address, PNetworkManager *manager)
    : PNetworkNode(NETWORK_CLIENT, address, manager)
{
}

PNetworkClient::~PNetworkClient()
{
}
    
