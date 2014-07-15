// pquaternion.cpp
// Quaternion.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//
//


#include "pquaternion.h"

#include <PFoundation/pvector4.h>
#include <PFoundation/pvector3.h>
#include <PFoundation/pmatrix4x4.h>

#include <math.h>

pfloat32 * P_APIENTRY pQuaternionNormalize(const pfloat32 *in, pfloat32 *out)
{
    pfloat32 norm = pQuaternionNorm(in);

    if (norm == 0.0f)
    {
        return P_NULL;
    }

    return pVector4Multiply(in, 1.0f / norm, out);
}

pfloat32 * P_APIENTRY pQuaternionMultiply(const pfloat32 *a, const pfloat32 *b, pfloat32 *out)
{
    out[0] = a[3] * b[0] + a[0] * b[3] + a[1] * b[2] - a[2] * b[1];
    out[1] = a[3] * b[1] - a[0] * b[2] + a[1] * b[3] + a[2] * b[0];
    out[2] = a[3] * b[2] + a[0] * b[1] - a[1] * b[0] + a[2] * b[3];
    out[3] = a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2];

    return out;
}

pfloat32 * P_APIENTRY pQuaternionInverse(pfloat32* inout)
{
    pfloat32 normSquared = inout[0] * inout[0] + inout[1] * inout[1] + inout[2] * inout[2] + inout[3] * inout[3];

    if (normSquared == 0.0f)
    {
        return P_NULL;
    }

    pfloat32 inv = 1.0f / normSquared;

    inout[0] *= -inv;
    inout[1] *= -inv;
    inout[2] *= -inv;
    inout[3] *=  inv;

    return inout;
}

pfloat32 * P_APIENTRY pQuaternionCreateRotation(pfloat32 x, pfloat32 y, pfloat32 z, pfloat32* out)
{
    pfloat32 rotZ[4];
    pfloat32 rotY[4];
    pfloat32 rotX[4];

    pQuaternionCreateRotationX(x, rotX);
    pQuaternionCreateRotationY(y, rotY);
    pQuaternionCreateRotationZ(z, rotZ);

    pfloat32 temp[4];
    pQuaternionMultiply(rotZ, rotY, temp);
    pQuaternionMultiply(temp, rotX, out);

    return out;
}

pfloat32 * P_APIENTRY pQuaternionCreateRotationX(pfloat32 angle, pfloat32 *out)
{
    out[0] = sinf(angle * 0.5f); 
    out[1] = 0.0f; 
    out[2] = 0.0f;
    out[3] = cosf(angle * 0.5f);

    return out;
}

pfloat32 * P_APIENTRY pQuaternionCreateRotationY(pfloat32 angle, pfloat32 *out)
{
    out[0] = 0.0f; 
    out[1] = sinf(angle * 0.5f); 
    out[2] = 0.0f; 
    out[3] = cosf(angle * 0.5f);

    return out;
}

pfloat32 * P_APIENTRY pQuaternionCreateRotationZ(pfloat32 angle, pfloat32 *out)
{
    out[0] = 0.0f; 
    out[1] = 0.0f; 
    out[2] = sinf(angle * 0.5f); 
    out[3] = cosf(angle * 0.5f);

    return out;
}

void P_APIENTRY pQuaternionGetRotation(const pfloat32 *in, pfloat32 &anglex, 
    pfloat32 &angley, pfloat32 &anglez)
{
    pfloat32 x = in[0];
    pfloat32 y = in[1];
    pfloat32 z = in[2];
    pfloat32 w = in[3];

    pfloat32 f00 = 1.0f - 2.0f * y * y - 2.0f * z * z;
    pfloat32 f20 = 2.0f * x * z - 2.0f * w * y;

    if (f00 != 0.0f)
    {
    	pfloat32 f10 = 2.0f * x * y + 2.0f * w * z;
    	pfloat32 f21 = 2.0f * y * z + 2.0f * w * x;
    	pfloat32 f22 = 1.0f - 2.0f * x * x - 2.0f * y * y;

        anglex = atan2f(f21, f22);
    	angley = asinf(-f20);
    	anglez = atan2f(f10, f00);
    }
    else
    {
    	pfloat32 f01 = 2.0f * x * y - 2.0f * w * z;
    	pfloat32 f11 = 1.0f - 2.0f * x * x - 2.0f * z * z;

        anglex = atan2f(f01, f11);
    	angley = asinf(-f20);
    	anglez = 0.0f;
    }
}

pfloat32 * P_APIENTRY pQuaternionRotate(pfloat32 angle, pfloat32 x, pfloat32 y, pfloat32 z, 
    pfloat32 *out)
{
    pfloat32 halfAngle = angle * 0.5f;

    pfloat32 s = sinf(halfAngle);

    pfloat32 normalized[3] = {x, y, z};

    pVector3Normalize(normalized, normalized);

    out[0] = normalized[0] * s;
    out[1] = normalized[1] * s;
    out[2] = normalized[2] * s;
    out[3] = cosf(halfAngle);
    
    return out;
}

pfloat32 * P_APIENTRY pQuaternionGetMatrix4x4(const pfloat32 *inQuat, pfloat32 *outMatrix)
{
    /* Quaternion -> matrix3x3 is defined as:
    | 1 - 2*qy2 - 2*qz2      2*qx*qy - 2*qz*qw       2*qx*qz + 2*qy*qw |
    | 2*qx*qy + 2*qz*qw      1 - 2*qx2 - 2*qz2       2*qy*qz - 2*qx*qw |
    | 2*qx*qz - 2*qy*qw      2*qy*qz + 2*qx*qw       1 - 2*qx2 - 2*qy2 |
    
    Source: http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm 1.6.2009
    */

    pfloat32 x = inQuat[0];
    pfloat32 y = inQuat[1];
    pfloat32 z = inQuat[2];
    pfloat32 w = inQuat[3];

    outMatrix[0] = 1.0f - 2.0f * y * y - 2.0f * z * z;
    outMatrix[1] = 2.0f * x * y + 2.0f * w * z;
    outMatrix[2] = 2.0f * x * z - 2.0f * w * y;
    outMatrix[3] = 0.0f;

    outMatrix[4] = 2.0f * x * y - 2.0f * w * z;
    outMatrix[5] = 1.0f - 2.0f * x * x - 2.0f * z * z;
    outMatrix[6] = 2.0f * y * z + 2.0f * w * x;
    outMatrix[7] = 0.0f;

    outMatrix[8] = 2.0f * x * z + 2.0f * w * y;
    outMatrix[9] = 2.0f * y * z - 2.0f * w * x;
    outMatrix[10] = 1.0f - 2.0f * x * x - 2.0f * y * y;
    outMatrix[11] = 0.0f;

    outMatrix[12] = 0.0f;
    outMatrix[13] = 0.0f;
    outMatrix[14] = 0.0f;
    outMatrix[15] = 1.0f;

    return outMatrix;
}

pfloat32 * P_APIENTRY pQuaternionGetMatrix3x3(const pfloat32 *inQuat, pfloat32 *outMatrix)
{
    pfloat32 x = inQuat[0];
    pfloat32 y = inQuat[1];
    pfloat32 z = inQuat[2];
    pfloat32 w = inQuat[3];

    outMatrix[0] = 1.0f - 2.0f * y * y - 2.0f * z * z;
    outMatrix[1] = 2.0f * x * y + 2.0f * w * z;
    outMatrix[2] = 2.0f * x * z - 2.0f * w * y;

    outMatrix[3] = 2.0f * x * y - 2.0f * w * z;
    outMatrix[4] = 1.0f - 2.0f * x * x - 2.0f * z * z;
    outMatrix[5] = 2.0f * y * z + 2.0f * w * x;

    outMatrix[6] = 2.0f * x * z + 2.0f * w * y;
    outMatrix[7] = 2.0f * y * z - 2.0f * w * x;
    outMatrix[8] = 1.0f - 2.0f * x * x - 2.0f * y * y;

    return outMatrix;
}

void P_APIENTRY pQuaternionMultiplyVector3(const pfloat32* q, pfloat32* v, pfloat32* result)
{
    // Quaternion from the vector.
    pfloat32 q2[] = 
    {
        v[0],
        v[1],
        v[2],
        0,
    };
    pVector3Normalize(q2, q2);
    // The inverse of quaternion.
    pfloat32 inverse[4] = 
    {
        q[0],
        q[1],
        q[2],
        q[3],
    };

    // result = q*v*q^-1
    pQuaternionInverse(inverse);
    pfloat32 resultQuaternion[4];
    pQuaternionMultiply(q2, inverse, resultQuaternion);
    pQuaternionMultiply(q, resultQuaternion, q2);

    result[0] = q2[0];
    result[1] = q2[1];
    result[2] = q2[2];
}

pfloat32 * P_APIENTRY pQuaternionFromMatrix4x4(const pfloat32 *inMatrix, pfloat32 *outQuat)
{
    // Remove the scaling component from the matrix.
    pfloat32 rotMat[16];
    pMatrix4x4Copy(inMatrix, rotMat);

    pfloat32 scale[3];
    pMatrix4x4GetScaling(rotMat, &scale[0], &scale[1], &scale[2]);
    pMatrix4x4Scale(rotMat, 1.0f / scale[0], 1.0f / scale[1], 1.0f / scale[2]);
    
    pfloat32 trace = rotMat[0] + rotMat[5] + rotMat[10];

    // Quaternion is initialized from matrix by following manner:
    //   MatToQuat @ http://www.cs.indiana.edu/~hanson/quatvis/code/quatcodekit.txt 1.6.2009.

    // Positive matrix trace. 
    if (trace > 1.0f)
    {
        pfloat32 sqrtTraceDouble = sqrtf(trace) * 2.0f;

        outQuat[0] = (rotMat[9] - rotMat[6]) / sqrtTraceDouble;
        outQuat[1] = (rotMat[2] - rotMat[8]) / sqrtTraceDouble;
        outQuat[2] = (rotMat[4] - rotMat[1]) / sqrtTraceDouble;
        outQuat[3] = 0.25f * sqrtTraceDouble;
    }
    // Negative or zero trace 
    else
    {
        // See which column has the biggest value in diagonal. 
        if (rotMat[0] > rotMat[5] && rotMat[0] > rotMat[10])
        {
            pfloat32 divisor = sqrtf(1.0f + rotMat[0] - rotMat[5] - rotMat[10]) * 2.0f;

            outQuat[0] = 0.25f * divisor;
            outQuat[1] = (rotMat[4] + rotMat[1]) / divisor;
            outQuat[2] = (rotMat[2] + rotMat[8]) / divisor;
            outQuat[3] = (rotMat[9] - rotMat[6]) / divisor;

        }
        else if (rotMat[5] > rotMat[10])
        {
            pfloat32 divisor = sqrtf(1.0f + rotMat[5] - rotMat[0] - rotMat[10]) * 2.0f;

            outQuat[0] = (rotMat[2] - rotMat[8]) / divisor;
            outQuat[1] = (rotMat[4] + rotMat[1]) / divisor;
            outQuat[2] = 0.25f * divisor;
            outQuat[3] = (rotMat[9] + rotMat[6]) / divisor;
        }
        else 
        {
            pfloat32 divisor = sqrtf(1.0f + rotMat[10] - rotMat[0] - rotMat[5]) * 2.0f;

            outQuat[0] = (rotMat[4] - rotMat[1]) / divisor;
            outQuat[1] = (rotMat[2] + rotMat[8]) / divisor;
            outQuat[2] = (rotMat[9] + rotMat[6]) / divisor;
            outQuat[3] = 0.25f * divisor;
        }
    }

    return outQuat;
}

pfloat32 * P_APIENTRY pQuaternionSlerp(const pfloat32 *q1, const pfloat32 *q2, pfloat32 t, pfloat32* out)
{
    pfloat32 cosAlpha = q1[0] * q2[0] + q1[1] * q2[1] + q1[2] * q2[2] + q1[3] * q2[3];

    pfloat32 sinAlpha = sqrtf(1.0f - cosAlpha * cosAlpha);

    pfloat32 alpha = acosf(cosAlpha);

    pfloat32 a = sinf(alpha * (1.0f - t)) / sinAlpha;
    pfloat32 b = sinf(alpha * t) / sinAlpha;

    out[0] = a * q1[0] + b * q2[0];
    out[1] = a * q1[1] + b * q2[1];
    out[2] = a * q1[2] + b * q2[2];
    out[3] = a * q1[3] + b * q2[3];

    return out;
}
