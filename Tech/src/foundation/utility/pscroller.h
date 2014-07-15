// pscroller.h
// A scrolling controller.
//
// If the user slowly scrolls from one place to another, the view follows
// the finger movement tightly.
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com 
//
// 

#ifndef PSCROLLER_H
#define PSCROLLER_H

#include <PFoundation/pglobal.h>

enum PScrollerInterpolatorEnum
{
    P_SCROLLER_INTERPOLATOR_FLUID,
    P_SCROLLER_INTERPOLATOR_SPLINE,
    P_SCROLLER_INTERPOLATOR_POLYNOMIAL,
};

class P_DLLEXPORT PScroller
{
public: 
    // Constructor.
    PScroller();

    // Smooth scrolling to a position.
    // \param startPosition the start position.
    // \param startTime the start time in milliseconds.
    // \param distance how far this scroller will run.
    // \param duration how long the scrolling will last in milliseconds.
    void startScroll(pfloat32 startPosition, puint32 startTime, 
            pfloat32 distance, puint32 duration);

    // Stop scrolling.
    void finish();

    // Is scrolling not finished?
    P_INLINE pbool isScrolling() const
    { return m_isScrolling; }

    // Get the current scroller's position.
    // \return the current scroller's position.
    P_INLINE pfloat32 currentPosition() const
    { return m_currentPosition; }

    // Get the scroller's start position.
    P_INLINE pfloat32 startPosition() const
    { return m_scrollingStartPosition; }

    // Get the current scrolling speed, in second.
    // \return the velocity in second
    P_INLINE pfloat32 currentVelocity() const
    { return m_currentVelocity; }

    // Update the current velocity and position of the scroller.
    // \param currentTime the current time in milliseconds.
    pbool update(puint32 currentTime);

    // Set the interpolator. The default one is spline.
    // \param interpolator set the interpolator of scrolling.
    void setInterpolator(PScrollerInterpolatorEnum interpolator);
    
    // Get the currently used interpolator. 
    // \return the interpolator used now.
    P_INLINE PScrollerInterpolatorEnum interpolator() const
    { return m_interpolator; }

protected:
    // Create a cubic curve by calculating a number of points on
    // it.
    // \param count the number of points.
    // \param curve the array of points.
    void initializeSpline(puint32 count, pfloat32* spline,
        pfloat32 p1, pfloat32 p2, pfloat32 q1, pfloat32 q2);

    // Initialize the viscous fluid interpolator.
    // \param scale how faster fluid it is. The larger the value, the faster the convergence to 1.
    void initializeFluid(pfloat32 scale);

    // Get an interpolation value from viscous fluid simulation.
    // \param x a value between 0 and 1.
    // \return the interpolated value.
    pfloat32 interpolateByViscousFluid(pfloat32 x);

    // Get an interpolation value from spline
    // \param x a value between 0 and 1.
    // \param spline the spline represented by key points.
    // \param sampleCount spline has sampleCount + 1 floating numbers within [0, 1].
    // \return the interpolated value.
    pfloat32 interpolateBySpline(pfloat32 x);
    
    // Get an interpolation value from polynomial.
    // 3 * x^2 - 2 * x^3 
    // \param x a value between 0 and 1.
    // \return the interpolated value.
    pfloat32 interpolateByPolynomial(pfloat32 x);

    // If the scroll is rolling now.
    pbool m_isScrolling;
    // The start position of the scrolling at this time.
    pfloat32 m_scrollingStartPosition;
    // The start time of the scrolling at this time in millisecond.
    puint32 m_scrollingStartTime;
    // The end position of the scrolling at this time
    pfloat32 m_scrollingFinishPosition;
    // The duration of the scrolling at this time, in milliseconds.
    puint32 m_scrollingDuration;
    // The current position of the scroller.
    pfloat32 m_currentPosition;
    // The scrolling current velocity (in milliseconds)
    pfloat32 m_currentVelocity;
    // The type of interpolator.
    PScrollerInterpolatorEnum m_interpolator;
    // The interpolator function pointer.
    pfloat32 (PScroller::*m_interpolatorFunction)(pfloat32);

     // This controls the viscous fluid effect (how much of it)
    pfloat32 m_viscousFluidScale;
     // This controls the viscous fluid effect.
    pfloat32 m_viscousFluidNormalize;
    enum {SAMPLE_COUNT = 100};
    // The cubic curve.
    pfloat32 m_spline[SAMPLE_COUNT + 1];
};

#endif // !P_SCROLLER_H
