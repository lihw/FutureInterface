//
// pinterpolatedvalue.h
//
// Intepolate two values by simulating a spring motion.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pinterpolatedvalue.h"

#include <PFoundation/passert.h>
#include <PFoundation/pmathutility.h>

#include <math.h>


static const pfloat32 P_INTERPOLATED_VALUE_MAXIMUM_ANGULAR_FREQUENCY = 500.0f;
static const pfloat32 P_INTERPOLATED_VALUE_MAXIMUM_DAMP_RATIO = 500.0f;
static const pfloat32 P_INTERPOLATED_VALUE_MAXIMUM_TIMESTEP = 0.03f;

PInterpolatedValue::PInterpolatedValue(pfloat32 startingValue,
                    pfloat32 targetValue,
                    pfloat32 angularFrequency,
                    pfloat32 dampRatio)
{
    m_value = startingValue;
    m_speed = 0;
    m_target = targetValue;
    m_isRepeated = false;
    m_range[0] = P_MIN_FLOAT32;
    m_range[1] = P_MAX_FLOAT32;
    m_angluarFrequency = pClamp(angularFrequency, 0.0f, P_INTERPOLATED_VALUE_MAXIMUM_ANGULAR_FREQUENCY);
    m_dampRatio = pClamp(dampRatio, 0.0f, P_INTERPOLATED_VALUE_MAXIMUM_DAMP_RATIO);
    m_maxTimeStep = P_INTERPOLATED_VALUE_MAXIMUM_TIMESTEP;
    m_timeStep = m_maxTimeStep * 0.5f;
}

PInterpolatedValue::~PInterpolatedValue()
{
}

void PInterpolatedValue::setRange(pfloat32 minimumValue, pfloat32 maximumValue)
{
    PASSERT(minimumValue < maximumValue);

    m_range[0] = minimumValue;
    m_range[1] = maximumValue;
}

pfloat32 PInterpolatedValue::getMinimum() const
{
    return m_range[0];
}

pfloat32 PInterpolatedValue::getMaximum() const
{
    return m_range[1];
}

void PInterpolatedValue::setMaximumTimeStep(pfloat32 timeStep)
{
    m_maxTimeStep = timeStep;
}

pfloat32 PInterpolatedValue::getMaximumTimeStep() const
{
    return m_maxTimeStep;
}

void PInterpolatedValue::enableRepeat(bool enabled)
{
    m_isRepeated = enabled;
}

bool PInterpolatedValue::isRepeatEnabled() const
{
    return m_isRepeated;
}

void PInterpolatedValue::setValue(pfloat32 value, bool resetInterpolation)
{
    m_value = value;
    
    if (resetInterpolation)
    {
        m_speed = 0.0f;
        m_target = value;
    }
}

pfloat32 PInterpolatedValue::getValue() const
{
    return m_value;
}
    
void PInterpolatedValue::setSpeed(pfloat32 speed)
{
    m_speed = speed;
}
    
pfloat32 PInterpolatedValue::getSpeed() const
{
    return m_speed;
}
    
void PInterpolatedValue::modifySpeed(pfloat32 speedDelta)
{
    m_speed += speedDelta;
}
    
void PInterpolatedValue::setTarget(pfloat32 target)
{
    m_target = target;
}
    
pfloat32 PInterpolatedValue::getTarget() const
{
    return m_target;
}
    
pfloat32 PInterpolatedValue::getTargetNormalized() const
{
    if (m_isRepeated)
    {
        pfloat32 range = m_range[1] - m_range[0];
        return fmod(m_target - m_range[0], range) + m_range[0];
    }

    return m_target;
}
    
void PInterpolatedValue::modifyTarget(pfloat32 targetDelta)
{
    m_target += targetDelta;
}

void PInterpolatedValue::update(puint32 elapsedTime)
{
    // Convert to second
    pfloat32 deltaTime = (pfloat32)elapsedTime * 0.001f;

    // Integrate in suitable time steps. 
    while (deltaTime > 2.0f * P_FLOAT_EPSILON)
    {
        deltaTime -= integrateBS(deltaTime);
        //deltaTime -= integrateRK(deltaTime);
    }

    // Normalize interpolator. 
    if (m_isRepeated)
    {
        pfloat32 newValue = m_range[0] + fmod(m_value - m_range[0], m_range[1] - m_range[0]);
        pfloat32 normalizationDistance = newValue - m_value;
        m_target += normalizationDistance;
        m_value = newValue;
    }
}

void PInterpolatedValue::setAngularFrequency(pfloat32 value)
{
    m_angluarFrequency = pClamp(value, 0.0f, P_INTERPOLATED_VALUE_MAXIMUM_ANGULAR_FREQUENCY);

    // Reset the motion.
    m_value = m_target;
    m_speed = 0;
}

pfloat32 PInterpolatedValue::integrateBS(pfloat32 deltaTime)
{
    // Constants for Bogacki-Shampine sample calculation. 
    static const pfloat32 P_BS_B21 = (1.0f / 2.0f);
    static const pfloat32 P_BS_B32 = (3.0f / 4.0f);
    static const pfloat32 P_BS_B41 = (2.0f / 9.0f);
    static const pfloat32 P_BS_B42 = (1.0f / 3.0f);
    static const pfloat32 P_BS_B43 = (4.0f / 9.0f);

    // Constants for Bogacki-Shampine lower order sample. 
    static const pfloat32 P_BS_C1 = 2.0f / 9.0f;
    static const pfloat32 P_BS_C2 = 1.0f / 3.0f;
    static const pfloat32 P_BS_C3 = 4.0f / 9.0f;

    // Constants for Bogacki-Shampine higher order sample. 
    static const pfloat32 P_BS_D1 = 7.0f / 24.0f;
    static const pfloat32 P_BS_D2 = 1.0f / 4.0f;
    static const pfloat32 P_BS_D3 = 1.0f / 3.0f;
    static const pfloat32 P_BS_D4 = 1.0f / 8.0f;

    // Error tolerance for Bogacki-Shampine. 
    static const pfloat32 P_MAX_ESTIMATE_ERROR = 0.005f; ///< Upper bound for maximum error estimate. 
    static const pfloat32 P_MIN_ESTIMATE_ERROR = 0.00001f; ///< Lower bound for maximum error estimate. 
    static const pfloat32 P_CONVERGENCE_DISTANCE = 0.00005f; ///< Relative distance from convergence point where the interpolation is seen to have converged. 
    static const pfloat32 P_CONVERGENCE_SPEED = 0.0005f; ///< Relative maximum speed where the interpolation is seen to have converged. 
    static const pfloat32 P_SCALE_MINIMUM = 0.001f; ///< Minimum possible scale for calculations. 
    static const pfloat32 P_TIMESTEP_MINIMUM = 0.000001f; ///< Minimum possible timestep. 

    pfloat32 timeStep = pMin(deltaTime, m_timeStep);

    // Integration step.
    pfloat32 value1 = m_value;
    pfloat32 speed1 = m_speed;
    pfloat32 k1Value = timeStep * speed1;
    pfloat32 k1Speed = timeStep * getAcceleration(value1, speed1);

    pfloat32 value2 = value1 + P_BS_B21 * k1Value;
    pfloat32 speed2 = speed1 + P_BS_B21 * k1Speed;
    pfloat32 k2Value = timeStep * speed2;
    pfloat32 k2Speed = timeStep * getAcceleration(value2, speed2);

    pfloat32 value3 = value1 + P_BS_B32 * k2Value;
    pfloat32 speed3 = speed1 + P_BS_B32 * k2Speed;
    pfloat32 k3Value = timeStep * speed3;
    pfloat32 k3Speed = timeStep * getAcceleration(value3, speed3);

    pfloat32 value4 = value1 + P_BS_B41 * k1Value + P_BS_B42 * k2Value + P_BS_B43 * k3Value;
    pfloat32 speed4 = speed1 + P_BS_B41 * k1Speed + P_BS_B42 * k2Speed + P_BS_B43 * k3Speed;
    pfloat32 k4Speed = timeStep * getAcceleration(value4, speed4);
    pfloat32 k4Value = timeStep * speed4;
    
    pfloat32 lowerPrecisionSpeed = P_BS_C1 * k1Speed + P_BS_C2 * k2Speed + P_BS_C3 * k3Speed;

    pfloat32 higherPrecisionValue = P_BS_D1 * k1Value + P_BS_D2 * k2Value + P_BS_D3 * k3Value + P_BS_D4 * k4Value;
    pfloat32 higherPrecisionSpeed = P_BS_D1 * k1Speed + P_BS_D2 * k2Speed + P_BS_D3 * k3Speed + P_BS_D4 * k4Speed;

    // Convergence help. 
    bool converged = false;

    pfloat32 errorEstimate = pAbs(lowerPrecisionSpeed - higherPrecisionSpeed);
    pfloat32 errorScale = pMax(pMax(P_SCALE_MINIMUM, pAbs(m_target)), pAbs(m_value));
    
    pfloat32 distance = pAbs(m_value + higherPrecisionValue - m_target);

    pfloat32 maximumError = pMax(P_MIN_ESTIMATE_ERROR * errorScale, pMin(P_MAX_ESTIMATE_ERROR * errorScale, distance));

    pfloat32 convergenceScale = pMax(P_SCALE_MINIMUM, pAbs(m_target));
    pfloat32 convergenceError = P_CONVERGENCE_DISTANCE * convergenceScale;
    pfloat32 speedConvergenceError = P_CONVERGENCE_SPEED * convergenceScale;

    if (distance < convergenceError)
    {
        pfloat32 speedDistance = pAbs(m_speed + higherPrecisionSpeed);
        if (errorEstimate < convergenceError && speedDistance < speedConvergenceError)
        {
            m_speed = 0.0f;
            m_value = m_target;
            
            converged = true;
            timeStep = P_MAX_FLOAT32;
        }
    }

    // Step size adaption. 
    if (!converged)
    {
        /* TODO: Playing with these values may drastically alter interpolation efficiency. Not too much effort
         *       has been put into finding the most optimal numbers. Possible performance gain could range from
         *       almost zero to 30 % or even higher. */
        if (errorEstimate > maximumError)
        {
            if(m_timeStep > P_TIMESTEP_MINIMUM)
            {
                m_timeStep = pMax(P_TIMESTEP_MINIMUM, m_timeStep * 0.5f);
                timeStep = 0.0f;
            }
        }
        else if (errorEstimate < 0.17f * maximumError)
        {
            m_timeStep = pMin(2.0f * m_timeStep, m_maxTimeStep);
        }

        if (timeStep > 0.0f)
        {
            m_value += higherPrecisionValue;
            m_speed += higherPrecisionSpeed;
        }
    }

    return timeStep;
}

pfloat32 PInterpolatedValue::integrateRK(pfloat32 deltaTime)
{
    pfloat32 timeStep = pMin(m_maxTimeStep, deltaTime);

    pfloat32 value1 = m_value;
    pfloat32 speed1 = m_speed;
    pfloat32 accel1 = getAcceleration(value1, speed1);

    pfloat32 value2 = value1 + 0.5f * timeStep * speed1;
    pfloat32 speed2 = speed1 + 0.5f * timeStep * accel1;
    pfloat32 accel2 = getAcceleration(value2, speed2);

    pfloat32 value3 = value1 + 0.5f * timeStep * speed2;
    pfloat32 speed3 = speed1 + 0.5f * timeStep * accel2;
    pfloat32 accel3 = getAcceleration(value3, speed3);

    pfloat32 value4 = value1 + timeStep * speed3;
    pfloat32 speed4 = speed1 + timeStep * accel3;
    pfloat32 accel4 = getAcceleration(value4, speed4);

    pfloat32 speed = (speed1 + 2.0f * speed2 + 2.0f * speed3 + speed4) / 6.0f;
    pfloat32 accel = (accel1 + 2.0f * accel2 + 2.0f * accel3 + accel4) / 6.0f;

    m_value += timeStep * speed;
    m_speed += timeStep * accel;

    return timeStep;
}

pfloat32 PInterpolatedValue::getAcceleration(pfloat32 value, pfloat32 speed) const
{
    // Modified critically damped harmonic oscillator. 
    // A critically damped system converges to zero as fast as possible without oscillating
    // http://en.wikipedia.org/wiki/Damping
    pfloat32 w = m_angluarFrequency;
    pfloat32 b = m_dampRatio;
    pfloat32 dx = value - m_target;
    pfloat32 ddx = speed;

    // Don't decelerate faster due to having speed outwards. 
    if (dx * ddx > 0.0f)
    {
        ddx = 0.0f;
    }

    return -w * w * dx - (2.0f * w * b) * ddx;
}


