// pentity.h
// The base class of all objects which needs
// runtime type information and reference count.
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//
// 

#ifndef PENTITY_H
#define PENTITY_H

#include <PFoundation/pglobal.h>

class P_DLLEXPORT PEntity
{
    PEntity(const PEntity &other) {}
    void operator=(const PEntity &other);

public:
    PEntity(); 
    virtual ~PEntity();
    
    virtual void retain();
    virtual void release();

protected:
    puint32     m_ref;

    // TODO: RTTI
};

#endif // !PENTITY_H
