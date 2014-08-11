// pnetworkmanager.h
// The network manager
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#ifndef PNETWORKMANAGER_H
#define PNETWORKMANAGER_H

#include <PFoundation/pmodule.h>

class PNetworkNode;

class P_DLLEXPORT PNetworkManager : public PModule
{
    friend class PNetworkNode;

public:
    PNetworkManager(PContext *context);
    virtual ~PNetworkManager();

    virtual void uninitialize();
    virtual void update();
    virtual pbool resume();
    virtual void pause();

private:
    void addNode(PNetworkNode *node);
    void removeNode(PNetworkNode *node);

private:
    // Can only be server or client, not both.
    PNetworkNode *m_node;  
};

#endif // PNETWORKMANAGER_H
