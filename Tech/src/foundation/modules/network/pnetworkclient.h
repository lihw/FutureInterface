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

class P_DLLEXPORT PNetworkClient : public PNetworkNode
{
public:
    PNetworkClient(const pchar *address, PNetworkManager *manager);
    virtual ~PNetworkClient();
    
private:
};



#endif // !PNETWORKCLIENT_H
