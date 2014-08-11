// pmodule.h
// The base class of all module managers
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//
// 

#ifndef PMODULE_H
#define PMODULE_H

#include <PFoundation/pstring.h>

class PContext;


class P_DLLEXPORT PModule
{
public:
    PModule(const pchar *name, PContext *context);
    virtual ~PModule();

    virtual void update();
    virtual pbool initialize();
    virtual void uninitialize();
    virtual void pause();
    virtual pbool resume();
    
    P_INLINE PContext *context() const {return m_context; }
    P_INLINE const PString &name() const { return m_name; }

protected:
    PContext *m_context;
    PString   m_name;
};


#endif // !PMODULE_H
