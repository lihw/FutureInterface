// pnetwork.cpp
// The network functions
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "pnetwork.h"

#include <PFoundation/plog.h>

#include <enet/enet.h>

pbool pInitializeNetwork()
{
    if (enet_initialize() != 0) 
    {
        PLOG_ERROR("Could not initialize network module.");
        return false;
    }

    return true;
}

void pUninitializeNetwork()
{
    enet_deinitialize();
}
