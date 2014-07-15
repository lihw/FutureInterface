// pinterpolatedvalue.h
//
// Intepolate two values by simulating a spring motion.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//


#ifndef PINTERPOLATEDVALUE_H
#define PINTERPOLATEDVALUE_H

#include <PFoundation/pglobal.h>

//
// A PInterpolatedValue will move from a start value to the target value
// gradually like a damping motion.
// See http://en.wikipedia.org/wiki/Damping
//
class PInterpolatedValue
{
public:
    // Constructor.
    // \param startingValue the starting value.
    // \param angularFrequency the angular frequency of this motion. 
    // A larger value renders the time for moving startingValue to
    // targetValue smaller. When it becomes zero, the acceleration is zero
    // and the motion becomes a uniform motion.
    // \param dampingRatio 
    //   dampRatio = 1 critical damping. No oscillating.
    //   dampRatio > 1 over damping. Take longer to get to target value than
    //   critical damping. No oscillating.
    //   dampRatio < 1 under damping with oscillating. 
    //   dampRatio = 0 harmonic oscillation.
    //   See http://en.wikipedia.org/wiki/Damping
    PInterpolatedValue(pfloat32 startingValue,
                        pfloat32 targetValue,
                        pfloat32 angularFrequency,
                        pfloat32 dampRatio = 1.0f);

    // Destructor.
    ~PInterpolatedValue();

    // Set the minimum and maximum of the values, the value range.
    // \param minimumValue the left side of the value range.
    // \param maximumValue the right side of the value range.
    void setRange(pfloat32 minimumValue, pfloat32 maximumValue);

    // Get the minimum of values.
    // \return the minimum of values.
    pfloat32 getMinimum() const;

    // Get the maximum of values.
    // \return the maximum of values.
    pfloat32 getMaximum() const;

    // Set the maximum time slice. The value update frequency is dependent
    // on this time slice.
    // \param timeStep the maximum time step between two value updatings in milliseconds.
    void setMaximumTimeStep(pfloat32 timeStep);

    // Get the time slice. 
    // \return the time slice.
    pfloat32 getMaximumTimeStep() const;

    // Set to the repeat mode. The default mode is clamp mode. Only effective
    // when it is an under-damping motion.
    // The values will be wrap to the valid range when go out the range.
    // \param enabled true to set to the repeat mode and false to clamp mode.
    void enableRepeat(bool enabled);

    // Check if the current mode is repeat.
    // \return true for yes.
    bool isRepeatEnabled() const;

    // Set the current interpolated value.
    // \param value the new current value.
    // \param resetInterpolation reset the current speed to zero and restart the interpolation.
    void setValue(pfloat32 value, bool resetInterpolation);

    // Get the current interpolated value.
    // \return the current interpolated value.
    pfloat32 getValue() const;

    // Set the current speed of the spring.
    // \param speed the new current interpolated speed.
    void setSpeed(pfloat32 speed);
    
    // Get the current speed of the spring.
    // \return the current interpolated speed.
    pfloat32 getSpeed() const;
    
    // Modify the current speed of the spring by adding an delta value.
    // \param speedDelta the delta value to add to the current speed.
    void modifySpeed(pfloat32 speedDelta);
    
    // Set the current target of the spring.
    // \param target the new target.
    void setTarget(pfloat32 target);
    
    // Get the current target of the spring.
    // \return the current target.
    pfloat32 getTarget() const;
    
    // Get the normalize target value in the repeat mode. The target value is warped to the range.
    // \return the normalized target which is always in the range.
    pfloat32 getTargetNormalized() const;
    
    // Modify the current target of the spring by adding an delta value.
    // \param targetDelta the delta value to add to the current target.
    void modifyTarget(pfloat32 targetDelta);

    // Set the angular frequency and this function will reset the motion.
    // \param value the new angular frequency.
    void setAngularFrequency(pfloat32 value);

    // Update the current interpolated value status.
    // \param elapsedTime the elapsed time since last update in milliseconds
    void update(puint32 elapsedTime);

private:
    // Bogacki-Shampine algorithm. 
    pfloat32 integrateBS(pfloat32 deltaTime);
    // Runge-Kutta 4 algorithm.
    pfloat32 integrateRK(pfloat32 deltaTime);
    // Get the acceleration with given distance value and speed.
    // \param value the distance value
    // \param speed the speed of the sprint.
    // \return the acceleration.
    pfloat32 getAcceleration(pfloat32 value, pfloat32 speed) const;
private:
    // The target.
    pfloat32 m_target;
    // The current value.
    pfloat32 m_value;
    // The value range.
    pfloat32 m_range[2];
    // The current speed.
    pfloat32 m_speed;
    // Is it repeating
    bool m_isRepeated;
    // Damp ratio.
    pfloat32 m_dampRatio;
    // Angular frequency.
    pfloat32 m_angluarFrequency;
    // Time step in second.
    pfloat32 m_timeStep;
    // Maximum time step in second.
    pfloat32 m_maxTimeStep;
};


#endif // !PINTERPOLATEDVALUE_H
