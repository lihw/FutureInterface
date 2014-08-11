// pmodule.cpp
// The base class of all module managers
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//
// 

#include "pmodule.h"

#include <PFoundation/passert.h>


PModule::PModule(const pchar *name, PContext *context)
    : m_name(name)
{
    PASSERT(context != P_NULL);
    m_context = context;;
}

PModule::~PModule()
{
}

void PModule::update()
{
}

pbool PModule::initialize()
{
    return true;
}

void PModule::uninitialize()
{
}

void PModule::pause()
{
}

pbool PModule::resume()
{
    return true;
}
