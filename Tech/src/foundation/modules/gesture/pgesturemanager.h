// pgesturemanager.h
// The gesture Manager header file.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#ifndef PGESTUREMANAGER_H
#define PGESTUREMANAGER_H

#include <PFoundation/pmodule.h>

#include "pgesture_private.h"

class PEvent;

class P_DLLEXPORT PGestureManager : public PModule
{
public:
    PGestureManager(PContext *context);
    virtual ~PGestureManager();
    
    virtual void update();
    virtual void uninitialize();
    virtual pbool resume();

    void recognize(PEvent *event);

private:
    // TODO: move this functionality to platform layer, so that every touch event is inside
    // application's window region.
    void clamp(pint32 &x, pint32 &y);

private:
    PAbstractGesture *m_gestures[P_GESTURE_TYPE_NUMBER];
};

#endif // PGESTUREMANAGER_H
