// pmatrix4x4.cpp
// 4x4 matrix math. The matrix is represented in column order as
// OpenGL
//
// Copyright 2012 - 2014 Future Interface Li. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pmatrix4x4.h"

#include <PFoundation/pmathutility.h>
#include <PFoundation/pquaternion.h>
#include <PFoundation/pplane.h>

#include <PFoundation/plog.h>
#include <PFoundation/passert.h>

#include <math.h>

pfloat32 * P_APIENTRY pMatrix4x4Identity(pfloat32* matrix)
{
    matrix[0] = 1;
    matrix[1] = 0;
    matrix[2] = 0;
    matrix[3] = 0;

    matrix[4] = 0;
    matrix[5] = 1;
    matrix[6] = 0;
    matrix[7] = 0;
    
    matrix[8]  = 0;
    matrix[9]  = 0;
    matrix[10] = 1;
    matrix[11] = 0;
    
    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;

    return matrix;
}

pfloat32 * P_APIENTRY pMatrix4x4SetTranslation(pfloat32 x, 
    pfloat32 y, 
    pfloat32 z, 
    pfloat32* inout)
{
    inout[12] = x;
    inout[13] = y;
    inout[14] = z;

    return inout;
}

pfloat32 * P_APIENTRY pMatrix4x4CreateTranslation(pfloat32 x, 
    pfloat32 y, 
    pfloat32 z,
    pfloat32 *out)
{
    out[0] = 1;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;

    out[4] = 0;
    out[5] = 1;
    out[6] = 0;
    out[7] = 0;
    
    out[8]  = 0;
    out[9]  = 0;
    out[10] = 1;
    out[11] = 0;
    
    out[12] = x;
    out[13] = y;
    out[14] = z;
    out[15] = 1;

    return out;
}

pfloat32 * P_APIENTRY pMatrix4x4Translate(pfloat32 *inout, pfloat32 x, pfloat32 y, pfloat32 z)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    inout[12] += x;
    inout[13] += y;
    inout[14] += z;
#endif

    return inout;
}

pfloat32 * P_APIENTRY pMatrix4x4Multiply(const pfloat32 *a, const pfloat32 *b, pfloat32 *out)
{
    PASSERT(a != out && b != out);

#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3]; 
    out[1] = a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3]; 
    out[2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3]; 
    out[3] = a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3]; 
    
    out[4] = a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7]; 
    out[5] = a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7]; 
    out[6] = a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7]; 
    out[7] = a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7]; 
    
    out[8]  = a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11]; 
    out[9]  = a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11]; 
    out[10] = a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11]; 
    out[11] = a[3] * b[8] + a[7] * b[9] + a[11] * b[10] + a[15] * b[11]; 
    
    out[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15]; 
    out[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15]; 
    out[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15]; 
    out[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15]; 

#endif

    return out;
}

pfloat32 * P_APIENTRY pMatrix4x4RotateRxRyRz(pfloat32 *inout, pfloat32 rx, pfloat32 ry, pfloat32 rz)
{
    pfloat32 tempMatrix1[16];
    pfloat32 tempMatrix2[16];

    pMatrix4x4CreateRotationRxRyRz(rx, ry, rz, tempMatrix1);
    pMatrix4x4Multiply(tempMatrix1, inout, tempMatrix2);
    pMatrix4x4Copy(tempMatrix2, inout);

    return inout;
}

pfloat32 * P_APIENTRY pMatrix4x4RotateRzRyRx(pfloat32 *inout, pfloat32 rx, pfloat32 ry, pfloat32 rz)
{
    pfloat32 tempMatrix1[16];
    pfloat32 tempMatrix2[16];

    pMatrix4x4CreateRotationRzRyRx(rx, ry, rz, tempMatrix1);
    pMatrix4x4Multiply(tempMatrix1, inout, tempMatrix2);
    pMatrix4x4Copy(tempMatrix2, inout);

    return inout;
}

pfloat32 * P_APIENTRY pMatrix4x4RotateRx(pfloat32 *inout, pfloat32 radians)
{
    pfloat32 rotation[16];
    pMatrix4x4CreateRotationRx(radians, rotation);
    
    pfloat32 tempMatrix[16];
    pMatrix4x4Multiply(rotation, inout, tempMatrix);

    pMatrix4x4Copy(tempMatrix, inout);

    return inout;
}

pfloat32 * P_APIENTRY pMatrix4x4RotateRy(pfloat32 *inout, pfloat32 radians)
{
    pfloat32 rotation[16];
    pMatrix4x4CreateRotationRy(radians, rotation);

    pfloat32 tempMatrix[16];
    pMatrix4x4Multiply(rotation, inout, tempMatrix);

    pMatrix4x4Copy(tempMatrix, inout);

    return inout;
}

pfloat32 * P_APIENTRY pMatrix4x4RotateRz(pfloat32 *inout, pfloat32 radians)
{
    pfloat32 rotation[16];
    pMatrix4x4CreateRotationRz(radians, rotation);

    pfloat32 tempMatrix[16];
    pMatrix4x4Multiply(rotation, inout, tempMatrix);

    pMatrix4x4Copy(tempMatrix, inout);

    return inout;
}

pfloat32 * P_APIENTRY pMatrix4x4Rotate(pfloat32 *inout, pfloat32 radians, pfloat32 x, pfloat32 y, pfloat32 z)
{
    pfloat32 tempMatrix1[16];
    pfloat32 tempMatrix2[16];

    pMatrix4x4CreateRotation(radians, x, y, z, tempMatrix1);
    pMatrix4x4Multiply(tempMatrix1, inout, tempMatrix2);
    pMatrix4x4Copy(tempMatrix2, inout);

    return inout;
}

pfloat32 * P_APIENTRY pMatrix4x4CreateRotationRx(pfloat32 radians, pfloat32 *out)
{
    pfloat32 cx = cosf(radians);
    pfloat32 sx = sinf(radians);

    out[0] = 1.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
    out[3] = 0.0f;

    out[4] = 0.0f;
    out[5] = cx;
    out[6] = sx;
    out[7] = 0.0f;

    out[8] = 0.0f;
    out[9] = -sx;
    out[10] = cx;
    out[11] = 0.0f;

    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = 0.0f;
    out[15] = 1.0f;

    return out;
}

pfloat32 * P_APIENTRY pMatrix4x4CreateRotationRy(pfloat32 radians, pfloat32 *out)
{
    pfloat32 cy = cosf(radians);
    pfloat32 sy = sinf(radians);

    out[0] = cy;
    out[1] = 0.0f;
    out[2] = -sy;
    out[3] = 0.0f;

    out[4] = 0.0f;
    out[5] = 1.0f;
    out[6] = 0.0f;
    out[7] = 0.0f;

    out[8] = sy;
    out[9] = 0.0f;
    out[10] = cy;
    out[11] = 0.0f;

    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = 0.0f;
    out[15] = 1.0f;

    return out;
}

pfloat32 * P_APIENTRY pMatrix4x4CreateRotationRz(pfloat32 radians, pfloat32 *out)
{
    pfloat32 cz = cosf(radians);
    pfloat32 sz = sinf(radians);

    out[0] = cz;
    out[1] = sz;
    out[2] = 0.0f;
    out[3] = 0.0f;

    out[4] = -sz;
    out[5] = cz;
    out[6] = 0.0f;
    out[7] = 0.0f;

    out[8] = 0.0f;
    out[9] = 0.0f;
    out[10] = 1.0f;
    out[11] = 0.0f;

    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = 0.0f;
    out[15] = 1.0f;

    return out;
}
 
pfloat32 * P_APIENTRY pMatrix4x4CreateRotationRzRyRx(pfloat32 rx, pfloat32 ry, pfloat32 rz, pfloat32 *out)
{
    pfloat32 sx = sinf(rx);
    pfloat32 cx = cosf(rx);
    pfloat32 sy = sinf(ry);
    pfloat32 cy = sinf(ry);
    pfloat32 sz = sinf(rz);
    pfloat32 cz = cosf(rz);

    out[0] = cy * cz;
    out[1] = sz * cy; 
    out[2] = -sy;
    out[3] = 0.0f;

    out[4] = sx * sy * cz - cx * sz;
    out[5] = sx * sy * sz + cx * cz;
    out[6] = sx * cy;
    out[7] = 0.0f;

    out[8] =  cx * sy * cz + sx * sz;
    out[9] =  sx * cy * sz - sx * cz;
    out[10] = cx * cy;
    out[11] = 0.0f;

    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = 0.0f;
    out[15] = 1.0f;

    return out;
}

pfloat32 * P_APIENTRY pMatrix4x4CreateRotationRxRyRz(pfloat32 rx, pfloat32 ry, pfloat32 rz, pfloat32 *out)
{
    pfloat32 sx = sinf(rx);
    pfloat32 cx = cosf(rx);
    pfloat32 sy = sinf(ry);
    pfloat32 cy = cosf(ry);
    pfloat32 sz = sinf(rz);
    pfloat32 cz = cosf(rz);

    out[0] = cy * cz;
    out[1] = sx * sy * cz + cx * sz;
    out[2] = -cx * sy * cz + sx * sz;
    out[3] = 0.0f;

    out[4] = -cy * sz;
    out[5] = -sx * sy * sz + cx * cz;
    out[6] = cx * sy * sz + sx * cz;
    out[7] = 0.0f;

    out[8] =  sy;
    out[9] =  -sx * cy;
    out[10] = cx * cy;
    out[11] = 0.0f;

    out[12] = 0.0f;
    out[13] = 0.0f;
    out[14] = 0.0f;
    out[15] = 1.0f;

    return out;
}

pfloat32 * P_APIENTRY pMatrix4x4CreateRotation(pfloat32 radians, pfloat32 x, pfloat32 y, pfloat32 z, pfloat32 *out)
{
    pfloat32 axisLengthSquared = x * x + y * y + z * z;
    const pfloat32 epsilon = 0.0001f;

    if (axisLengthSquared < epsilon)
    {
        pMatrix4x4Identity(out);
    }
    else
    {
        pfloat32 cosTerm = cosf(radians);
        pfloat32 sinTerm = sinf(radians);
        pfloat32 cosMinusTerm = 1.0f - cosTerm;
        pfloat32 axisLengthInverse = 1.0f / sqrtf(axisLengthSquared);

        x *= axisLengthInverse;
        y *= axisLengthInverse;
        z *= axisLengthInverse;

        /*  OpenGL specification defines glRotatef as: 
        ( xx(1-c)+c     xy(1-c)-zs  xz(1-c)+ys  0  )
        | yx(1-c)+zs    yy(1-c)+c   yz(1-c)-xs  0  |
        | xz(1-c)-ys    yz(1-c)+xs  zz(1-c)+c   0  |
        (   0                 0         0       1)*/
        out[0] = x * x * cosMinusTerm + cosTerm;
        out[1] = x * y * cosMinusTerm + z * sinTerm;
        out[2] = x * z * cosMinusTerm - y * sinTerm;
        out[3] = 0.0f;
        out[4] = y * x * cosMinusTerm - z * sinTerm;
        out[5] = y * y * cosMinusTerm + cosTerm;
        out[6] = y * z * cosMinusTerm + x * sinTerm;
        out[7] = 0.0f;
        out[8] = z * x * cosMinusTerm + y * sinTerm;
        out[9] = z * y * cosMinusTerm - x * sinTerm;
        out[10] = z * z * cosMinusTerm + cosTerm;
        out[11] = 0.0f;
        out[12] = 0.0f;
        out[13] = 0.0f;
        out[14] = 0.0f;
        out[15] = 1.0f;
    }

    return out;
}

// The transformation matrix master follow the order like:
// Transformation matrix = translation_matrix * rotation_matrix * scaling_matrix
void P_APIENTRY pMatrix4x4GetRotationRzRyRx(const pfloat32 *in, pfloat32 *rx, pfloat32 *ry, pfloat32 *rz)
{
    pfloat32 scales[3];

    pMatrix4x4GetScaling(in, &scales[0], &scales[1], &scales[2]);
    
    // Rz * Ry * Rx
    //
    // cos(y) * cos(z)  -cos(x) * sin(z) + cos(z) * sin(x) * sin(y)    sin(z) * sin(x) + cos(x) * cos(z) * sin(y)
    // cos(y) * sin(z)  cos(x) * cos(z) + sin(x) * sin(y) * sin(z)     -cos(z) * sin(x) + cos(x) * sin(y) * sin(z)
    // -sin(y)          cos(y) * sin(x)                                cos(x) * cos(y)

    if (in[0] != 0)
    {
        *rx = atan2f(in[6] / scales[1], in[10] / scales[2]);
        *ry = asinf(-in[2] / scales[0]);
        *rz = atan2f(in[1] / scales[0], in[0] / scales[0]);
    }
    else
    {
        *rx = atan2f(in[6] / scales[1], in[10] / scales[2]);
        *ry = asinf(-in[2] / scales[0]);
        *rz = 0.0f;
    }
}


void P_APIENTRY pMatrix4x4GetRotationRxRyRz(const pfloat32 *in, pfloat32 *rx, pfloat32 *ry, pfloat32 *rz)
{
    pfloat32 scales[3];

    pMatrix4x4GetScaling(in, &scales[0], &scales[1], &scales[2]);
    
    if (in[0] != 0)
    {
        *rx = atan2f(-in[9] / scales[2], in[10] / scales[2]);
        *ry = asinf(in[8] / scales[2]);
        *rz = atan2f(in[4] / scales[1], in[0] / scales[0]);
    }
    else
    {
        *rx = atan2f(-in[9] / scales[2], in[10] / scales[2]);
        *ry = asinf(in[8] / scales[2]);
        *rz = 0.0f;
    }
}

// The transformation matrix master follow the order like:
// transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix
void P_APIENTRY pMatrix4x4GetScaling(const pfloat32 *matrix, pfloat32 *sx, pfloat32 *sy, pfloat32 *sz)
{
    *sx = sqrtf(matrix[0] * matrix[0] + matrix[1] * matrix[1] + matrix[2] * matrix[2]);
    *sy = sqrtf(matrix[4] * matrix[4] + matrix[5] * matrix[5] + matrix[6] * matrix[6]);
    *sz = sqrtf(matrix[8] * matrix[8] + matrix[9] * matrix[9] + matrix[10] * matrix[10]);
}

pfloat32 * P_APIENTRY pMatrix4x4LookAt(const pfloat32 *eye, 
    const pfloat32 *center, 
    const pfloat32 *up, 
    pfloat32 *out)
{
    // Reference: http://3dgep.com/?p=1700
    pfloat32 xaxis[3];
    pfloat32 yaxis[3];
    pfloat32 zaxis[3];

    pVector3Subtract(eye, center, zaxis);
    pVector3Normalize(zaxis, zaxis);

    pVector3Cross(up, zaxis, xaxis);
    pVector3Normalize(xaxis, xaxis);

    pVector3Cross(zaxis, xaxis, yaxis);
    pVector3Normalize(yaxis, yaxis);

    out[0] = xaxis[0];
    out[1] = yaxis[0];
    out[2] = zaxis[0];
    out[3] = 0;
    out[4] = xaxis[1];
    out[5] = yaxis[1];
    out[6] = zaxis[1];
    out[7] = 0;
    out[8] = xaxis[2];
    out[9] = yaxis[2];
    out[10] = zaxis[2];
    out[11] = 0;
    out[12] = -pVector3Dot(xaxis, eye);
    out[13] = -pVector3Dot(yaxis, eye);
    out[14] = -pVector3Dot(zaxis, eye);
    out[15] = 1;

    return out;
}


pfloat32 * P_APIENTRY pMatrix4x4PerspectiveProjection(pfloat32 fovY, 
    pfloat32 aspect, 
    pfloat32 zNear, 
    pfloat32 zFar, 
    pfloat32 *out)
{
    pfloat32 r = pDegreesToRadians(fovY / 2);
    pfloat32 deltaZ = zFar - zNear;
    pfloat32 s = sinf(r);
    pfloat32 cotangent = 0;

    if (deltaZ == 0 || s == 0 || aspect == 0)
    {
        return P_NULL;
    }

    //cos(r) / sin(r) = cot(r)
    cotangent = cosf(r) / s;

    pMatrix4x4Identity(out);
    out[0] = cotangent / aspect;
    out[5] = cotangent;
    out[10] = -(zFar + zNear) / deltaZ;
    out[11] = -1;
    out[14] = -2.0f * zNear * zFar / deltaZ;
    out[15] = 0;

    return out;
}
pfloat32 * P_APIENTRY pMatrix4x4InversedPerspectiveProjection(pfloat32 fovY,
    pfloat32 aspect,
    pfloat32 zNear, 
    pfloat32 zFar, 
    pfloat32 *out)
{
    // See Appendix F of the OpenGL red book
    pfloat32 r = pDegreesToRadians(fovY / 2);
    pfloat32 deltaZ = zFar - zNear;
    pfloat32 s = sinf(r);
    pfloat32 cotangent = 0;

    if (deltaZ == 0 || s == 0 || aspect == 0)
    {
        return P_NULL;
    }

    //cos(r) / sin(r) = cot(r)
    cotangent = cosf(r) / s;

    pMatrix4x4Identity(out);
    out[0] = aspect / cotangent;
    out[5] = 1.0f / cotangent;
    out[10] = 0.0f;
    out[11] = -deltaZ / (2.0f * zNear * zFar);
    out[14] = -1;
    out[15] = (zFar + zNear) / (2.0f * zFar * zNear);

    return out;
}


pfloat32 * P_APIENTRY pMatrix4x4OrthographicProjection(pfloat32 left, 
    pfloat32 right, 
    pfloat32 bottom, 
    pfloat32 top, 
    pfloat32 nearVal, 
    pfloat32 farVal,
    pfloat32 *out)
{
    pfloat32 tx = -((right + left) / (right - left));
    pfloat32 ty = -((top + bottom) / (top - bottom));
    pfloat32 tz = -((farVal + nearVal) / (farVal - nearVal));

    pMatrix4x4Identity(out);
    out[0] = 2.0f / (right - left);
    out[5] = 2.0f / (top - bottom);
    out[10] = -2.0f / (farVal - nearVal);
    out[12] = tx;
    out[13] = ty;
    out[14] = tz;

    return out;
}

pfloat32 * P_APIENTRY pMatrix4x4InversedOrthographicProjection(pfloat32 left,
    pfloat32 right, 
    pfloat32 bottom, 
    pfloat32 top, 
    pfloat32 nearVal,
    pfloat32 farVal,
    pfloat32 *out)
{
    pMatrix4x4Identity(out);
    out[0] = (right - left) / 2.0f;
    out[5] = (top - bottom) / 2.0f;
    out[10] = (farVal - nearVal) / -2.0f;
    out[12] = (right + left) / 2.0f;
    out[13] = (top + bottom) / 2.0f;
    out[14] = (nearVal + farVal) / 2.0f;

    return out;
}

pfloat32 * P_APIENTRY pMatrix4x4Scale(pfloat32 *inout, pfloat32 sx, pfloat32 sy, pfloat32 sz)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    inout[0] *= sx; inout[1] *= sy; inout[2] *= sz;
    inout[4] *= sx; inout[5] *= sy; inout[6] *= sz;
    inout[8] *= sx; inout[9] *= sy; inout[10] *= sz;
#endif

    return inout;
}

pfloat32 * P_APIENTRY pMatrix4x4CreateScaling(pfloat32 sx, pfloat32 sy, pfloat32 sz, pfloat32 *out)
{
    out[0] = sx;
    out[1] = 0;
    out[2] = 0;
    out[3] = 0;

    out[4] = 0;
    out[5] = sy;
    out[6] = 0;
    out[7] = 0;
    
    out[8]  = 0;
    out[9]  = 0;
    out[10] = sz;
    out[11] = 0;
    
    out[12] = 0;
    out[13] = 0;
    out[14] = 0;
    out[15] = 1;

    return out;
}

pfloat32 * P_APIENTRY pMatrix4x4Transpose(const pfloat32 *in, pfloat32 *out)
{
    if (in == out)
    {
        pSwap(out[1], out[4]);
        pSwap(out[2], out[8]);
        pSwap(out[3], out[12]);
        pSwap(out[6], out[9]);
        pSwap(out[7], out[13]);
        pSwap(out[11], out[14]);
    }
    else
    {
        out[0] = in[0];  
        out[1] = in[4];  
        out[2] = in[8];  
        out[3] = in[12];  
        out[4] = in[1];  
        out[5] = in[5];  
        out[6] = in[9];  
        out[7] = in[13];  
        out[8] = in[2];  
        out[9] = in[6];  
        out[10] = in[10];  
        out[11] = in[14];  
        out[12] = in[3];  
        out[13] = in[7]; 
        out[14] = in[11];  
        out[15] = in[15];  
    }

    return out;
}

pfloat32 * P_APIENTRY pMatrix4x4MultiplyVector4(const pfloat32* m, const pfloat32* v, pfloat32 *out)
{    
    PASSERT(v != out);
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = m[0] * v[0] + m[4] * v[1] + m[8] * v[2] + m[12] * v[3];
    out[1] = m[1] * v[0] + m[5] * v[1] + m[9] * v[2] + m[13] * v[3];
    out[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14] * v[3];
    out[3] = m[3] * v[0] + m[7] * v[1] + m[11] * v[2] + m[15] * v[3];
#endif

    return out;
}

pfloat32 * P_APIENTRY pMatrix4x4MultiplyVector3(const pfloat32* m, const pfloat32* v, pfloat32* out)
{
    PASSERT(v != out);

#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    pfloat32 w[4] = {v[0], v[1], v[2], 1.0f};
    out[0] = m[0] * w[0] + m[4] * w[1] + m[8] * w[2] + m[12] * w[3];
    out[1] = m[1] * w[0] + m[5] * w[1] + m[9] * w[2] + m[13] * w[3];
    out[2] = m[2] * w[0] + m[6] * w[1] + m[10] * w[2] + m[14] * w[3];
#endif

    return out;
}

pfloat32 P_APIENTRY pMatrix4x4Determinant3x3(const pfloat32 *m)
{
    pfloat32 determinant1 = m[5] * m[10] - m[9] * m[6];
    pfloat32 determinant2 = m[4] * m[10] - m[8] * m[6];
    pfloat32 determinant3 = m[4] * m[9]  - m[8] * m[5];

    return  (m[0] * determinant1 - m[1] * determinant2 + m[2] * determinant3);
}

pfloat32 * P_APIENTRY pMatrix4x4AffineInverse(const pfloat32 *in, pfloat32 *out)
{
    const pfloat32 determinant = pMatrix4x4Determinant3x3(in);

    // Check if determinant for this matrix is zero. 
    if (determinant < P_FLOAT_EPSILON && determinant > -P_FLOAT_EPSILON)
    {
        return P_NULL;
    }
    else
    {
        // 2x2 square determinants 
        pfloat32 determinant11, determinant12, determinant13;
        pfloat32 determinant21, determinant22, determinant23;
        pfloat32 determinant31, determinant32, determinant33;
        pfloat32 detInv;

        // Transpose of matrix.
        pfloat32 transpose[16];
        pMatrix4x4Transpose(in, transpose);

        // Actual inverse of upper 3x3 matrix is defined: 1 / det(M) * (adjugate(M)) 
        determinant11 = transpose[5] * transpose[10] - transpose[9] * transpose[6];
        determinant12 = transpose[4] * transpose[10] - transpose[8] * transpose[6];
        determinant13 = transpose[4] * transpose[9] - transpose[8] * transpose[5];

        determinant21 = transpose[1] * transpose[10] - transpose[9] * transpose[2];
        determinant22 = transpose[0] * transpose[10] - transpose[8] * transpose[2];
        determinant23 = transpose[0] * transpose[9] - transpose[8] * transpose[1];

        determinant31 = transpose[1] * transpose[6] - transpose[5] * transpose[2];
        determinant32 = transpose[0] * transpose[6] - transpose[2] * transpose[4];
        determinant33 = transpose[0] * transpose[5] - transpose[4] * transpose[1];

        // 4x4 part of matrix can be safely be identity only, since rest of the
        // matrix content is filled with sub determinants 
        detInv = 1.0f / determinant;
        out[0] = determinant11 * detInv;
        out[1] = -determinant12 * detInv;
        out[2] = determinant13 * detInv;
        out[3] = 0;
        out[4] = -determinant21 * detInv;
        out[5] = determinant22 * detInv;
        out[6] = -determinant23 * detInv;
        out[7] = 0;
        out[8] = determinant31 * detInv;
        out[9] = -determinant32 * detInv;
        out[10] = determinant33 * detInv;
        out[11] = 0;
        out[12] = 0;
        out[13] = 0;
        out[14] = 0;
        out[15] = 1.0f;

        pMatrix4x4Translate(out, -in[12], -in[13], -in[14]);
    }

    return out;
}

pfloat32 * P_APIENTRY pMatrix4x4RigidBodyInverse(const pfloat32* in, pfloat32* out)
{
    const pfloat32 determinant = pMatrix4x4Determinant3x3(in);

    /* Check if determinant for this matrix is zero. */
    if (determinant < P_FLOAT_EPSILON && determinant > -P_FLOAT_EPSILON)
    {
        return P_NULL;
    }
    else
    {
        // Invert affine orthogonal. 
        if (in != out)
        {
            out[0] = in[0];
            out[1] = in[4];
            out[2] = in[8];
            out[3] = 0.0f;
            out[4] = in[1];
            out[5] = in[5];
            out[6] = in[9];
            out[7] = 0.0f;
            out[8] = in[2];
            out[9] = in[6];
            out[10] = in[10];
            out[11] = 0.0f;
            out[12] = -(in[0] * in[12] + in[1] * in[13] + in[2] * in[14]);
            out[13] = -(in[4] * in[12] + in[5] * in[13] + in[6] * in[14]);
            out[14] = -(in[8] * in[12] + in[9] * in[13] + in[10] * in[14]);
            out[15] = 1.0f;
        }
        else
        {
            pfloat32 temp12 = -(in[0] * in[12] + in[1] * in[13] + in[2] * in[14]);
            pfloat32 temp13 = -(in[4] * in[12] + in[5] * in[13] + in[6] * in[14]);
            pfloat32 temp14 = -(in[8] * in[12] + in[9] * in[13] + in[10] * in[14]);

            out[12] = temp12;
            out[13] = temp13;
            out[14] = temp14;
            
            pSwap(out[1], out[4]);
            pSwap(out[2], out[8]);
            pSwap(out[6], out[9]);
        }

        return out;
    }
}

pfloat32 * P_APIENTRY pMatrix4x4InverseTranspose(const pfloat32 *in, pfloat32 *out)
{
    pfloat32 subFactor00 = in[10] * in[15] - in[14] * in[11];
    pfloat32 subFactor01 = in[9]  * in[15] - in[13] * in[11];
    pfloat32 subFactor02 = in[9]  * in[14] - in[13] * in[10];
    pfloat32 subFactor03 = in[8]  * in[15] - in[12] * in[11];
    pfloat32 subFactor04 = in[8]  * in[14] - in[12] * in[10];
    pfloat32 subFactor05 = in[8]  * in[13] - in[12] * in[9];
    pfloat32 subFactor06 = in[6]  * in[15] - in[14] * in[7];
    pfloat32 subFactor07 = in[5]  * in[15] - in[13] * in[7];
    pfloat32 subFactor08 = in[5]  * in[14] - in[13] * in[6];
    pfloat32 subFactor09 = in[4]  * in[15] - in[12] * in[7];
    pfloat32 subFactor10 = in[4]  * in[14] - in[12] * in[6];
    pfloat32 subFactor11 = in[5]  * in[15] - in[13] * in[7];
    pfloat32 subFactor12 = in[4]  * in[13] - in[12] * in[5];
    pfloat32 subFactor13 = in[6]  * in[11] - in[10] * in[7];
    pfloat32 subFactor14 = in[5]  * in[11] - in[9]  * in[7];
    pfloat32 subFactor15 = in[5]  * in[10] - in[9]  * in[6];
    pfloat32 subFactor16 = in[4]  * in[11] - in[8]  * in[7];
    pfloat32 subFactor17 = in[4]  * in[10] - in[8]  * in[6];
    pfloat32 subFactor18 = in[4]  * in[9]  - in[8]  * in[5];

    out[0]  = + (in[5] * subFactor00 - in[6] * subFactor01 + in[7] * subFactor02);
    out[1]  = - (in[4] * subFactor00 - in[6] * subFactor03 + in[7] * subFactor04);
    out[2]  = + (in[4] * subFactor01 - in[5] * subFactor03 + in[7] * subFactor05);
    out[3]  = - (in[4] * subFactor02 - in[5] * subFactor04 + in[6] * subFactor05);

    out[4]  = - (in[1] * subFactor00 - in[2] * subFactor01 + in[3] * subFactor02);
    out[5]  = + (in[0] * subFactor00 - in[2] * subFactor03 + in[3] * subFactor04);
    out[6]  = - (in[0] * subFactor01 - in[1] * subFactor03 + in[3] * subFactor05);
    out[7]  = + (in[0] * subFactor02 - in[1] * subFactor04 + in[2] * subFactor05);

    out[8]  = + (in[1] * subFactor06 - in[2] * subFactor07 + in[3] * subFactor08);
    out[9]  = - (in[0] * subFactor06 - in[2] * subFactor09 + in[3] * subFactor10);
    out[10] = + (in[0] * subFactor11 - in[1] * subFactor09 + in[3] * subFactor12);
    out[11] = - (in[0] * subFactor08 - in[1] * subFactor10 + in[2] * subFactor12);

    out[12] = - (in[1] * subFactor13 - in[2] * subFactor14 + in[3] * subFactor15);
    out[13] = + (in[0] * subFactor13 - in[2] * subFactor16 + in[3] * subFactor17);
    out[14] = - (in[0] * subFactor14 - in[1] * subFactor16 + in[3] * subFactor18);
    out[15] = + (in[0] * subFactor15 - in[1] * subFactor17 + in[2] * subFactor18);

    pfloat32 inv = 1.0f / 
        (in[0] * out[0] + in[1] * out[1] + in[2] * out[2] + in[3] * out[3]);

    out[0]  *= inv; 
    out[1]  *= inv; 
    out[2]  *= inv; 
    out[3]  *= inv; 
    
    out[4]  *= inv; 
    out[5]  *= inv; 
    out[6]  *= inv; 
    out[7]  *= inv; 
    
    out[8]  *= inv; 
    out[9]  *= inv; 
    out[10] *= inv; 
    out[11] *= inv; 
   
    out[12] *= inv; 
    out[13] *= inv; 
    out[14] *= inv; 
    out[15] *= inv; 

    return out;
}

pfloat32 * P_APIENTRY pMatrix4x4Interpolate(const pfloat32 *matrix1, const pfloat32 *matrix2, pfloat32 t, pfloat32 *out)
{
#define INTERPOLATE(a, b, t) (a * (1.0f - t) + b * t)
    // Interpolate the rotation.
    pfloat32 q1[4], q2[4], q[4];
    pQuaternionFromMatrix4x4(matrix1, q1);
    pQuaternionFromMatrix4x4(matrix2, q2);
    pQuaternionSlerp(q1, q2, t, q);
    pQuaternionGetMatrix4x4(q, out);

    // Interpolate the scale.
    pfloat32 scale1[3], scale2[3], scale[3];
    pMatrix4x4GetScaling(matrix1, &scale1[0], &scale1[1], &scale1[2]);
    pMatrix4x4GetScaling(matrix2, &scale2[0], &scale2[1], &scale2[2]);

    scale[0] = INTERPOLATE(scale1[0], scale2[0], t);
    scale[1] = INTERPOLATE(scale1[1], scale2[1], t);
    scale[2] = INTERPOLATE(scale1[2], scale2[2], t);

    pMatrix4x4Scale(out, scale[0], scale[1], scale[2]);

    // Interpolate the translation.
    out[12] = INTERPOLATE(matrix1[12], matrix2[12], t);
    out[13] = INTERPOLATE(matrix1[13], matrix2[13], t);
    out[14] = INTERPOLATE(matrix1[14], matrix2[14], t);

#undef INTERPOLATE

    return out;
}

pfloat32 * P_APIENTRY pMatrix4x4CreateReflection(const PPlane &plane, pfloat32 *out)
{
    PVector3 n = plane.normal();

    out[0]  = 1.0f - 2.0f * n[0] * n[0];
    out[1]  = -2.0f * n[0] * n[1];
    out[2]  = -2.0f * n[0] * n[2];
    out[3]  = 0;
    
    out[4]  = -2.0f * n[0] * n[1];
    out[5]  = 1.0f - 2.0f * n[1] * n[1];
    out[6]  = -2.0f * n[1] * n[2];
    out[7]  = 0;
    
    out[8]  = -2.0f * n[0] * n[1];
    out[9]  = -2.0f * n[1] * n[2];
    out[10] = 1.0f - 2.0f * n[2] * n[2];
    out[11] = 0;

    out[12] = -2.0f * n[0] * plane.D();
    out[13] = -2.0f * n[1] * plane.D();
    out[14] = -2.0f * n[2] * plane.D();
    out[15] = 1.0f;

    return out;
}

#if defined P_DEBUG
void P_APIENTRY pMatrix4x4Print(const pfloat32 *matrix)
{
    PLOG_INFO("%6.3f %6.3f %6.3f %6.3f", matrix[0], matrix[4], matrix[8], matrix[12]);
    PLOG_INFO("%6.3f %6.3f %6.3f %6.3f", matrix[1], matrix[5], matrix[9], matrix[13]);
    PLOG_INFO("%6.3f %6.3f %6.3f %6.3f", matrix[2], matrix[6], matrix[10], matrix[14]);
    PLOG_INFO("%6.3f %6.3f %6.3f %6.3f", matrix[3], matrix[7], matrix[11], matrix[15]);
}
#endif
