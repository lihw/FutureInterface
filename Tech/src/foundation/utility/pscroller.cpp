// pscroller.cpp
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

#include "pscroller.h"

#include <PFoundation/pdebugutility.h>

#include <math.h>
    
const static pfloat32 INFLEXION = 0.35f;

PScroller::PScroller()
{
    const pfloat32 START_TENSION = 0.4f;
    const pfloat32 END_TENSION = 1.0f - START_TENSION;
    const pfloat32 P1 = START_TENSION * INFLEXION;
    const pfloat32 P2 = (1.0f - END_TENSION) * INFLEXION;

    // Create the spline.
    initializeSpline(SAMPLE_COUNT, m_spline, P1, P2, START_TENSION, END_TENSION);

    // Initialize the fluid simulation.
    initializeFluid(8.0f);

    // The default interpolator is spline.
    setInterpolator(P_SCROLLER_INTERPOLATOR_SPLINE);

    m_currentPosition = 0;
    m_currentVelocity = 0;
    m_isScrolling     = false;
}

void PScroller::startScroll(pfloat32 startPosition, puint32 startTime, 
        pfloat32 distance, puint32 duration)
{
    m_isScrolling = true;

    m_scrollingStartPosition  = startPosition;
    m_scrollingStartTime      = startTime;
    m_scrollingFinishPosition = m_scrollingStartPosition + distance;
    m_scrollingDuration       = duration;
    m_currentVelocity         = 0.0f;
}

void PScroller::finish()
{
    m_currentPosition = m_scrollingFinishPosition;
    m_currentVelocity = 0.0f;
    
    m_isScrolling = false;
}
    
pbool PScroller::update(puint32 currentTime)
{
    if (!m_isScrolling) 
    {
        return false;
    }

    // FIXME: as currentTime is the timestamp of the current frame, it might
    // be earlier than m_scrollingStartTime when startScroll() is called in the
    // same frame but later. That's why we use minimum of the two.
    puint32 elapsedTime = currentTime - pMin(m_scrollingStartTime, currentTime);

    if (elapsedTime < m_scrollingDuration) 
    {
        pfloat32 q = (pfloat32)elapsedTime / (pfloat32)m_scrollingDuration;
        // Interpolate q
        q = (this->*m_interpolatorFunction)(q);

        pfloat32 previousPosition = m_currentPosition;
        m_currentPosition = m_scrollingStartPosition + q * (m_scrollingFinishPosition - m_scrollingStartPosition);
        m_currentVelocity = 1000.0f * (m_currentPosition - previousPosition) / (pfloat32)elapsedTime;
    }
    else
    {
        finish();
        return false;
    }

    return true;
}
 
                          
pfloat32 PScroller::interpolateByViscousFluid(pfloat32 x)
{
    x *= m_viscousFluidScale;
    if (x < 1.0f) 
    {
        x -= (1.0f - exp(-x));
    } 
    else 
    {
        const pfloat32 start = 0.36787944117f;   // 1/e == exp(-1)
        x = 1.0f - exp(1.0f - x);
        x = start + x * (1.0f - start);
    }

    x *= m_viscousFluidNormalize;

    return x;
}

pfloat32 PScroller::interpolateBySpline(pfloat32 x)
{
    pfloat32 samples = (pfloat32)SAMPLE_COUNT;
    puint32 index = (puint32)(samples * x);

    pfloat32 xInf = (pfloat32)index / samples;
    //pfloat32 xSup = (pfloat32)(index + 1) / samples;
    pfloat32 sInf = m_spline[index];
    pfloat32 sSup = m_spline[index + 1];

    return sInf + (x - xInf) * (sSup - sInf) * samples;
}
    
pfloat32 PScroller::interpolateByPolynomial(pfloat32 x)
{
    pfloat32 x2 = x * x;
    return 3.0f * x2 - 2.0f * x * x2; 
}

void PScroller::initializeFluid(pfloat32 scale)
{
    m_viscousFluidScale = scale;
    m_viscousFluidNormalize = 1.0f;
    m_viscousFluidNormalize = 1.0f / interpolateByViscousFluid(1.0f);
}

void PScroller::initializeSpline(puint32 count, pfloat32* spline, 
        pfloat32 p1, pfloat32 p2, pfloat32 q1, pfloat32 q2)
{
    pfloat32 xMin = 0.0f;

    for (puint32 i = 0; i < count; i++) 
    {
        pfloat32 t = (pfloat32) i / (pfloat32)count;

        float xMax = 1.0f;
        float x, coef;

        while (true) 
        {
            x = xMin + (xMax - xMin) / 2.0f;

            coef = 3.0f * x * (1.0f - x);

            pfloat32 tx = coef * ((1.0f - x) * p1 + x * p2) + x * x * x;

            if (pAbs(tx - t) < 1e-5f) 
            {
                break;
            }

            if (tx > t) 
            {
                xMax = x;
            }
            else 
            {
                xMin = x;
            }
        }

        spline[i] = coef * ((1.0f - x) * q1 + x * q2) + x * x * x;
    }

    spline[count] = 1.0f;
}
    
    
void PScroller::setInterpolator(PScrollerInterpolatorEnum interpolator)
{
    switch (interpolator)
    {
        case P_SCROLLER_INTERPOLATOR_SPLINE:
            m_interpolatorFunction = &PScroller::interpolateBySpline;
            break;
        case P_SCROLLER_INTERPOLATOR_FLUID:
            m_interpolatorFunction = &PScroller::interpolateByViscousFluid;
            break;
        case P_SCROLLER_INTERPOLATOR_POLYNOMIAL:
            m_interpolatorFunction = &PScroller::interpolateByPolynomial;
            break;
    }

    m_interpolator = interpolator;
}
    
