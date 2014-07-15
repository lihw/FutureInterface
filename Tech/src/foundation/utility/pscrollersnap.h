// pscrollersnap.h
// An scrolling controller with snapping. When snapping is enabled, the scroll 
// should always stop at the snapping stops. 
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com 
//
// 

#ifndef PSCROLLERSNAP_H
#define PSCROLLERSNAP_H

#include <PFoundation/pscroller.h>

class P_DLLEXPORT PScrollerSnap : public PScroller
{
public: 
    // Constructor.
    PScrollerSnap();

    // Override KwScroller:startScroll() to round the scrolling distance to snap stops
    // if the snapping is enabled. When the finishing position is equal to 
    // [(current position + distance) / stop + 0.5] when positive or
    // [(current position + distance) / stop - 0.5] when negative.
    // Smooth scrolling to a position.
    // \param startPosition the start position.
    // \param startTime the start time in milliseconds.
    // \param distance how far this scroller will run.
    // \param duration how long the scrolling will last in milliseconds.
    void startScroll(pfloat32 startPosition, puint32 startTime, 
        pfloat32 distance, puint32 duration);

    // Enable the snapping. By default, the snapping is enabled.
    // \param flag true to enable and false to disable.
    void enableSnapping(bool flag);

    // Is the snapping enabled.
    // \return true when enabled.
    pbool isSnappingEnabled() const;

    // Set the snapping stop. The scrolling will always stop
    // at n * stop where n is an integer.
    // \param stop the stop value which should be > 0.
    void setSnappingStop(pfloat32 stop);

protected:
    // If the _snapping is enabled.
    pbool    m_snapping;
    // The snapping stop.
    pfloat32 m_snappingStop;
};


#endif // !P_SCROLLER_SNAP_H
