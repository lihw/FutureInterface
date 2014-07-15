// peventmanager.h
// The event manager
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//
// 

#ifndef PEVENTMANAGER_H
#define PEVENTMANAGER_H

#include <PFoundation/peventtype.h>
#include <PFoundation/pmodule.h>
#include <PFoundation/plist.h>


class PObject;
class PContext;
class PEvent;

class P_DLLEXPORT PEventManager : public PModule
{
    friend class PEvent;

public:
    PEventManager(PContext *context);
    virtual ~PEventManager();
    
    // Update the event queue in every frame. Fire every event whose fire time is due.
    // We can use a heap structure to implment the event queue.
    virtual void update();

    // Create a event object from the memory pool. 
    // in this frame.
    PEvent *createEvent(PEventTypeEnum type, PObject *sender);
    void recycleEvent(PEvent *event);

private:
    // Add a new event to the queue.
    void queueEvent(PEvent *event);
    // Fire this event in this frame.
    void fire(PEvent *event);

    void recycelEvent(PEvent *event);

private:
    PList<PEvent *>  m_queue;
};


#endif  // !PEVENTMANAGER_H
