// pquaternion.h 
// Quaternion.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//
//

#ifndef PQUATERNION_H
#define PQUATERNION_H

#include <PFoundation/pmathutility.h>

#include <math.h>


// A quaternion is at fact a 4-component vector (x, y, z, w).

// Set a quaternion to identity.
P_INLINE pfloat32 * P_APIENTRY pQuaternionIdentity(pfloat32 *inout)
{
    inout[0] = 0.0f;
    inout[1] = 0.0f;
    inout[2] = 0.0f;
    inout[3] = 1.0f;

    return inout;
}

// Copy a quaternion to anther.
P_INLINE pfloat32 * P_APIENTRY pQuaternionCopy(const pfloat32 *in, pfloat32 *out)
{
    out[0] = in[0];
    out[1] = in[1];
    out[2] = in[2];
    out[3] = in[3];

    return out;
}


// Get the norm of a quaternion
P_INLINE pfloat32 P_APIENTRY pQuaternionNorm(const pfloat32 *in)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    return sqrtf(in[0] * in[0] + in[1] * in[1] + in[2] * in[2] + in[3] * in[3]);
#endif
}

// Normalize a quaternion
pfloat32 * P_APIENTRY pQuaternionNormalize(const pfloat32 *in, pfloat32 *out);

// Multiply a quaternion with another (result = a * b)
pfloat32 * P_APIENTRY pQuaternionMultiply(const pfloat32 *a, const pfloat32 *b, pfloat32 *out);

// Inverse a quaternion 
pfloat32 * P_APIENTRY pQuaternionInverse(pfloat32 *inout);

// Get a quaternion from Euler angles, created by Rz * Ry * Rx
pfloat32 * P_APIENTRY pQuaternionCreateRotation(pfloat32 x, pfloat32 y, pfloat32 z, pfloat32 *out);

// Get a Euler angles from a quaternion.
void P_APIENTRY pQuaternionGetRotation(pfloat32& anglex, pfloat32& angley, pfloat32& anglez, const pfloat32 *in);

// Get a quaternion from a rotation (rotate an angle with an axis)
pfloat32 * P_APIENTRY pQuaternionCreateRotation(pfloat32 angle, pfloat32 x, pfloat32 y, pfloat32 z, pfloat32 *out);

// Get a quaternion from a rotation with x axis. 
pfloat32 * P_APIENTRY pQuaternionCreateRotationX(pfloat32 angle, pfloat32 *out);
// Get a quaternion from a rotation with y axis. 
pfloat32 * P_APIENTRY pQuaternionCreateRotationY(pfloat32 angle, pfloat32 *out);
// Get a quaternion from a rotation with z axis. 
pfloat32 * P_APIENTRY pQuaternionCreateRotationZ(pfloat32 angle, pfloat32 *out);

// Convert a quaternion to a 4x4 matrix.
pfloat32 * P_APIENTRY pQuaternionGetMatrix4x4(const pfloat32 *inQuat, pfloat32 *outMatrix);

// Convert a quaternion to a 3x3 matrix.
pfloat32 * P_APIENTRY pQuaternionGetMatrix3x3(const pfloat32 *inQuat, pfloat32 *outMatrix);

// Multiply a quaternion and a 3-component vector; rotate the vector by the quaternion.
pfloat32 * P_APIENTRY pQuaternionMultiplyVector3(const pfloat32 *inQuat, const pfloat32 *v, pfloat32 *outVector);

// Initialize a quaternion from a 4x4 matrix. Only the rotation part of the
// matrix is used to create the result quaternion.
pfloat32 * P_APIENTRY pQuaternionFromMatrix4x4(const pfloat32 *inMatrix, pfloat32 *outQuat);

// Spherical interpolate two quaternions.
pfloat32 * P_APIENTRY pQuaternionSlerp(const pfloat32 *q1, const pfloat32 *q2, pfloat32 t, pfloat32 *out);

//
// C++ wrapper
//

#include <PFoundation/pmatrix3x3.h>
#include <PFoundation/pmatrix4x4.h>

class P_DLLEXPORT PQuaternion
{
public:
    pfloat32 m_q[4];

    PQuaternion()
    {
        pQuaternionIdentity(m_q);
    };
    
    PQuaternion(const PQuaternion &q)
    {
        m_q[0] = q.m_q[0];
        m_q[1] = q.m_q[1];
        m_q[2] = q.m_q[2];
        m_q[3] = q.m_q[3];
    };
    
    const PQuaternion& operator=(const PQuaternion& q)
    {
        if (this != &q)
        {
            m_q[0] = q.m_q[0];
            m_q[1] = q.m_q[1];
            m_q[2] = q.m_q[2];
            m_q[3] = q.m_q[3];
        }
        return *this;
    };

    P_INLINE pfloat32 norm() const { return pQuaternionNorm(m_q); } 
    
    P_INLINE void normalize() { pQuaternionNormalize(m_q, m_q); }

    P_INLINE PQuaternion operator*(const PQuaternion &q) const
    { PQuaternion out; pQuaternionMultiply(m_q, q.m_q, out.m_q); return out; }
    P_INLINE PVector3 operator*(const PVector3 &v) const
    { PVector3 out; pQuaternionMultiplyVector3(m_q, v.m_v, out.m_v); return out; }
    
    P_INLINE void inverse() { pQuaternionInverse(m_q); }

    P_INLINE const PQuaternion &createRotation(pfloat32 x, pfloat32 y, pfloat32 z)
    { pQuaternionCreateRotation(x, y, z, m_q); return *this; }
    P_INLINE const PQuaternion &createRotation(pfloat32 angle, pfloat32 x, pfloat32 y, pfloat32 z)
    { pQuaternionCreateRotation(angle, x, y, z, m_q); return *this; }

    P_INLINE const PQuaternion &createRotationX(pfloat32 x)
    { pQuaternionCreateRotationX(x, m_q); return *this; }
    P_INLINE const PQuaternion &createRotationY(pfloat32 y)
    { pQuaternionCreateRotationY(y, m_q); return *this; }
    P_INLINE const PQuaternion &createRotationZ(pfloat32 z)
    { pQuaternionCreateRotationZ(z, m_q); return *this; }

    P_INLINE PMatrix4x4 getMatrix4x4() const
    { PMatrix4x4 out; pQuaternionGetMatrix4x4(m_q, out.m_m); return out; }
    P_INLINE PMatrix3x3 getMatrix3x3() const
    { PMatrix3x3 out; pQuaternionGetMatrix3x3(m_q, out.m_m); return out; }

    P_INLINE const PQuaternion &operator=(const PMatrix4x4 &matrix)
    { pQuaternionFromMatrix4x4(matrix.m_m, m_q); return *this; }
};


#endif // P_QUATERNION_H

