// pactivity.h 
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PACTIVITY_H
#define PACTIVITY_H

#include <PFoundation/pglobal.h>

class PContext;

class P_DLLEXPORT PActivity
{
public:
    PActivity(pint32 argc, pchar** argv);
    virtual ~PActivity();

    P_INLINE puint32 getNumberOfContexts() const { return m_contextCount; }

    pbool addContext(PContext* context);
    PContext* findContext(puint32 contextId) const;
    PContext* findContext(const pchar* contextName) const;
    puint32 findContextId(const pchar* contextName) const;
    void removeContext(PContext* context);
    void removeContext(puint32 contextId);
    void removeContext(const pchar* contextName);

    // Initialize the global services.
    pbool initialize();
    // Uninitialize services.
    void uninitialize();

    // All global variables need to be visited through PActivity for 
    // records.
public:
    // The globally unique instance of PActivity.
    P_GLOBAL static PActivity* s_activity;

private:
    // Context array 
    PContext*      m_contexts[P_CONTEXT_MAXIMUM_NUMBER];
    puint32        m_contextCount;
};

// Get the global application instance.
P_EXTERN PActivity * P_APIENTRY pGetActivity();


#endif // !PACTIVITY_H


