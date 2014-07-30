// pnetworkserver.h
// The network server node 
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#ifndef PNETWORKSERVER_H
#define PNETWORKSERVER_H

#include "pnetworknode_private.h"

class P_DLLEXPORT PNetworkServer: public PNetworkNode
{
public:
    PNetworkServer(const pchar *address, PNetworkManager *manager);
    virtual ~PNetworkServer();
};

#endif // !PNETWORKSERVER_H
