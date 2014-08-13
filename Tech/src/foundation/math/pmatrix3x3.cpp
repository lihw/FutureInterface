// pmatrix3x3.cpp
// 3x3 matrix math used for 2D translation, scaling and rotation. The matrix is
// represented in column order as OpenGL
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pmatrix3x3.h"

#include <PFoundation/pmathutility.h>

#include <math.h>

pfloat32 * P_APIENTRY pMatrix3x3Identity(pfloat32* inout)
{
    inout[0] = 1.0f;
    inout[1] = 0.0f;
    inout[2] = 0.0f;

    inout[3] = 0.0f;
    inout[4] = 1.0f;
    inout[5] = 0.0f;
    
    inout[6] = 0.0f;
    inout[7] = 0.0f;
    inout[8] = 1.0f;

    return inout;
}

pfloat32 * P_APIENTRY pMatrix3x3Copy(const pfloat32 *in, pfloat32 *out)
{
    if (in == out)
    {
        return out;
    }
    out[0] = in[0];  
    out[1] = in[1];  
    out[2] = in[2];  
    out[3] = in[3];  
    out[4] = in[4];  
    out[5] = in[5];  
    out[6] = in[6];  
    out[7] = in[7];  
    out[8] = in[8];  

    return out;
}

pfloat32 * P_APIENTRY pMatrix3x3Multiply(const pfloat32 *a, const pfloat32 *b, 
    pfloat32* out)
{
    PASSERT(a != out && b != out);

#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = a[0] * b[0] + a[3] * b[1] + a[6] * b[2]; 
    out[1] = a[1] * b[0] + a[4] * b[1] + a[7] * b[2]; 
    out[2] = a[2] * b[0] + a[5] * b[1] + a[8] * b[2]; 
    
    out[3] = a[0] * b[3] + a[3] * b[4] + a[6] * b[5]; 
    out[4] = a[1] * b[3] + a[4] * b[4] + a[7] * b[5]; 
    out[5] = a[2] * b[3] + a[5] * b[4] + a[8] * b[5]; 
    
    out[6] = a[0] * b[6] + a[3] * b[7] + a[6] * b[8]; 
    out[7] = a[1] * b[6] + a[4] * b[7] + a[7] * b[8]; 
    out[8] = a[2] * b[6] + a[5] * b[7] + a[8] * b[8]; 
#endif

    return out;
}

pfloat32 * P_APIENTRY pMatrix3x3Add(const pfloat32 *a, const pfloat32 *b, pfloat32 *out)
{

#if defined P_USE_SSE

#elif defined P_USE_NEON

#else
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];

    out[3] = a[3] + b[3];
    out[4] = a[4] + b[4];
    out[5] = a[5] + b[5];

    out[6] = a[6] + b[6];
    out[7] = a[7] + b[7];
    out[8] = a[8] + b[8];
#endif

    return out;
}

pfloat32 * P_APIENTRY pMatrix3x3Subtract(const pfloat32 *a, const pfloat32 *b, 
    pfloat32 *out)
{
#if defined P_USE_SSE

#elif defined P_USE_NEON

#else
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    out[2] = a[2] - b[2];

    out[3] = a[3] - b[3];
    out[4] = a[4] - b[4];
    out[5] = a[5] - b[5];

    out[6] = a[6] - b[6];
    out[7] = a[7] - b[7];
    out[8] = a[8] - b[8];

#endif

    return out;
}

pfloat32 * P_APIENTRY pMatrix3x3Transpose(const pfloat32 *in, pfloat32 *out)
{
    if (in == out)
    {
        pSwap(out[1], out[3]);
        pSwap(out[2], out[6]);
        pSwap(out[5], out[7]);
    }
    else
    {
        out[0] = in[0];  
        out[1] = in[3];  
        out[2] = in[6];  
        out[3] = in[1];  
        out[4] = in[4];  
        out[5] = in[7];  
        out[6] = in[2];  
        out[7] = in[5];  
        out[8] = in[8];  
    }

    return out;
}

pfloat32 * P_APIENTRY pMatrix3x3MultiplyVector3(const pfloat32 *m, const pfloat32* v, pfloat32 *out)
{
#if defined P_USE_SSE

#elif defined P_USE_NEON

#else
    out[0] = m[0] * v[0] + m[3] * v[1] + m[6] * v[2];
    out[1] = m[1] * v[0] + m[4] * v[1] + m[7] * v[2];
    out[2] = m[2] * v[0] + m[5] * v[1] + m[8] * v[2];
#endif

    return out;
}

P_INLINE pfloat32 P_APIENTRY pMatrix3x3Determinant(const pfloat32* in)
{
    return in[0] * in[4] * in[8] + 
           in[3] * in[7] * in[2] + 
           in[6] * in[1] * in[5] - 
           in[6] * in[4] * in[2] - 
           in[3] * in[1] * in[8] - 
           in[0] * in[7] * in[5];
}

pfloat32 * P_APIENTRY pMatrix3x3Inverse(const pfloat32 *in, pfloat32 *out)
{
    // TODO: can this be accelerated with SSE or NEON?

    const pfloat32 determinant = pMatrix3x3Determinant(in);

    // Check if determinant for this matrix is zero. 
    if (determinant < P_FLOAT_EPSILON && determinant > -P_FLOAT_EPSILON)
    {
        return P_NULL;
    }
    else
    {
        // Actual inverse of 
        pfloat32 determinant11, determinant12, determinant13;
        pfloat32 determinant21, determinant22, determinant23;
        pfloat32 determinant31, determinant32, determinant33;

        // Transpose of matrix.
        pfloat32 transpose[9];
        pMatrix3x3Transpose(in, transpose);

        // Actual inverse of 3x3 matrix is defined: 1 / det(M) * (adjugate(M)) 
        determinant11 = transpose[4] * transpose[8] - transpose[7] * transpose[5];
        determinant12 = transpose[3] * transpose[8] - transpose[6] * transpose[5];
        determinant13 = transpose[3] * transpose[7] - transpose[6] * transpose[4];

        determinant21 = transpose[1] * transpose[8] - transpose[7] * transpose[2];
        determinant22 = transpose[0] * transpose[8] - transpose[6] * transpose[2];
        determinant23 = transpose[0] * transpose[7] - transpose[6] * transpose[1];

        determinant31 = transpose[1] * transpose[5] - transpose[4] * transpose[2];
        determinant32 = transpose[0] * transpose[5] - transpose[2] * transpose[3];
        determinant33 = transpose[0] * transpose[4] - transpose[3] * transpose[1];

        // 4x4 part of matrix can be safely be identity only, since rest of the
        // matrix content is filled with sub determinants 
        pfloat32 detInv = 1.0f / determinant;
        out[0] = determinant11 * detInv;
        out[1] = -determinant12 * detInv;
        out[2] = determinant13 * detInv;
        out[3] = -determinant21 * detInv;
        out[4] = determinant22 * detInv;
        out[5] = -determinant23 * detInv;
        out[6] = determinant31 * detInv;
        out[7] = -determinant32 * detInv;
        out[8] = determinant33 * detInv;
    }

    return out;
}

pfloat32 * P_APIENTRY pMatrix3x3AffineInverse(const pfloat32 *in, pfloat32 *out)
{
    const pfloat32 determinant = pMatrix3x3Determinant(in);

    // Check if determinant for this matrix is zero. 
    if (determinant < P_FLOAT_EPSILON && determinant > -P_FLOAT_EPSILON)
    {
        return P_NULL;
    }
    else if (in == out)
    {
        // In case in == out.
        pfloat32 temp7 = -(in[1] * in[6] + in[4] * in[7]);
        pfloat32 temp6 = -(in[0] * in[6] + in[3] * in[7]);
        out[7] = temp7;
        out[6] = temp6;
        
        pSwap(out[3], out[1]);
    }
    else
    {
        // Invert affine orthogonal. 
        out[8] = 1.0f;
        out[7] = -(in[1] * in[6] + in[4] * in[7]);
        out[6] = -(in[0] * in[6] + in[3] * in[7]);
        
        out[5] = 0;
        out[4] = in[4];
        out[3] = in[1];

        out[2] = 0;
        out[1] = in[3];
        out[0] = in[0];
    }

    return out;
}

pfloat32 * P_APIENTRY pMatrix3x3Translate(pfloat32 *inout, pfloat32 x, pfloat32 y)
{
    inout[6] += inout[0] * x + inout[3] * y;
    inout[7] += inout[1] * x + inout[4] * y;

    return inout;
}

pfloat32 * P_APIENTRY pMatrix3x3Rotate(pfloat32 *inout, pfloat32 radians)
{
    pfloat32 rotationMatrix[9];
    pMatrix3x3CreateRotation(radians, rotationMatrix);
        
    pfloat32 tempMatrix[9];
    pMatrix3x3Multiply(inout, rotationMatrix, tempMatrix);

    return pMatrix3x3Copy(tempMatrix, inout);
}

pfloat32 * P_APIENTRY pMatrix3x3Scale(pfloat32 *inout, pfloat32 x, pfloat32 y)
{
    inout[0] *= x; inout[3] *= y; 
    inout[1] *= x; inout[4] *= y; 

    return inout;
}

void P_APIENTRY pMatrix3x3GetScale(const pfloat32 *in, pfloat32 &x, pfloat32 &y)
{
    x = sqrtf(in[0] * in[0] + in[1] * in[1]);  
    y = sqrtf(in[3] * in[3] + in[4] * in[4]);  
}

pfloat32 * P_APIENTRY pMatrix3x3CreateTranslation(pfloat32 x, pfloat32 y, pfloat32 *out)
{
    out[0] = 1.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;

    out[3] = 0.0f;
    out[4] = 1.0f;
    out[5] = 0.0f;

    out[6] = x;
    out[7] = y;
    out[8] = 1.0f;

    return out;
}

pfloat32 * P_APIENTRY pMatrix3x3CreateRotation(pfloat32 radians, pfloat32 *out)
{
    pfloat32 s = sinf(radians);
    pfloat32 c = cosf(radians);

    out[0] = c;
    out[1] = s;
    out[2] = 0;

    out[3] = -s;
    out[4] = c;
    out[5] = 0;
    
    out[6] = 0;
    out[7] = 0;
    out[8] = 1;

    return out;
}

pfloat32 * P_APIENTRY pMatrix3x3CreateScaling(pfloat32 x, pfloat32 y, pfloat32 *out)
{
    out[0] = x;
    out[1] = 0.0f;
    out[2] = 0.0f;

    out[3] = 0.0f;
    out[4] = y;
    out[5] = 0.0f;

    out[6] = 0.0f;
    out[7] = 0.0f;
    out[8] = 1.0f;

    return out;
}

pfloat32 * P_APIENTRY pMatrix3x3Interpolate(const pfloat32* matrix1, const pfloat32* matrix2, pfloat32 t, pfloat32* out)
{
#define INTERPOLATE(a, b, t) (a * (1.0f - t) + b * t)

    // Interpolate the scale.
    pfloat32 scale1[2], scale2[2], scale[2];
    pMatrix3x3GetScale(matrix1, scale1[0], scale1[1]);
    pMatrix3x3GetScale(matrix2, scale2[0], scale2[1]);

    scale[0] = INTERPOLATE(scale1[0], scale2[0], t);
    scale[1] = INTERPOLATE(scale1[1], scale2[1], t);

    pfloat32 theta1 = atan2f(matrix1[4], matrix1[1]);
    pfloat32 theta2 = atan2f(matrix2[4], matrix2[1]);
    pfloat32 theta = INTERPOLATE(theta1, theta2, t);

    pMatrix3x3CreateRotation(theta, out);

    out[0] *= scale[0];
    out[1] *= scale[1];
    
    out[3] *= scale[0];
    out[4] *= scale[1];
    
    out[6] = INTERPOLATE(matrix1[6], matrix2[6], t);
    out[7] = INTERPOLATE(matrix1[7], matrix2[7], t);

#undef INTERPOLATE

    return out;
}

pfloat32 * P_APIENTRY pMatrix3x3InverseTranspose(const pfloat32 *in, pfloat32 *out)
{
    const pfloat32 inv = 1.0f / pMatrix3x3Determinant(in);

    out[0] = (+(in[4] * in[8] - in[7] * in[5])) * inv;
    out[1] = (-(in[3] * in[8] - in[6] * in[5])) * inv;
    out[2] = (+(in[3] * in[7] - in[6] * in[4])) * inv;
    out[3] = (-(in[1] * in[8] - in[7] * in[2])) * inv;
    out[4] = (+(in[0] * in[8] - in[6] * in[2])) * inv;
    out[5] = (-(in[0] * in[7] - in[6] * in[1])) * inv;
    out[6] = (+(in[1] * in[5] - in[4] * in[2])) * inv;
    out[7] = (-(in[0] * in[5] - in[3] * in[2])) * inv;
    out[8] = (+(in[0] * in[4] - in[3] * in[1])) * inv;
    
    return out;
}

pfloat32 * P_APIENTRY pMatrix3x3InverseTransposeMatrix4x4(const pfloat32 *in, pfloat32 *out)
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
    //pfloat32 subFactor13 = in[6]  * in[11] - in[10] * in[7];
    //pfloat32 subFactor14 = in[5]  * in[11] - in[9]  * in[7];
    //pfloat32 subFactor15 = in[5]  * in[10] - in[9]  * in[6];
    //pfloat32 subFactor16 = in[4]  * in[11] - in[8]  * in[7];
    //pfloat32 subFactor17 = in[4]  * in[10] - in[8]  * in[6];
    //pfloat32 subFactor18 = in[4]  * in[9]  - in[8]  * in[5];

    out[0] = + (in[5] * subFactor00 - in[6] * subFactor01 + in[7] * subFactor02);
    out[1] = - (in[4] * subFactor00 - in[6] * subFactor03 + in[7] * subFactor04);
    out[2] = + (in[4] * subFactor01 - in[5] * subFactor03 + in[7] * subFactor05);
    pfloat32 temp = - (in[4] * subFactor02 - in[5] * subFactor04 + in[6] * subFactor05);

    out[3] = - (in[1] * subFactor00 - in[2] * subFactor01 + in[3] * subFactor02);
    out[4] = + (in[0] * subFactor00 - in[2] * subFactor03 + in[3] * subFactor04);
    out[5] = - (in[0] * subFactor01 - in[1] * subFactor03 + in[3] * subFactor05);

    out[6] = + (in[1] * subFactor06 - in[2] * subFactor07 + in[3] * subFactor08);
    out[7] = - (in[0] * subFactor06 - in[2] * subFactor09 + in[3] * subFactor10);
    out[8] = + (in[0] * subFactor11 - in[1] * subFactor09 + in[3] * subFactor12);

    pfloat32 inv = 1.0f / 
        (in[0] * out[0] + in[1] * out[1] + in[2] * out[2] + in[3] * temp);

    out[0] *= inv; 
    out[1] *= inv; 
    out[2] *= inv; 
    
    out[3] *= inv; 
    out[4] *= inv; 
    out[5] *= inv; 
    
    out[6] *= inv; 
    out[7] *= inv; 
    out[8] *= inv; 
   
    return out;
}
