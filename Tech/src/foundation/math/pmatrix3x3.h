// pmatrix3x3.h 
// 3x3 matrix math used for 2D translation, scaling and rotation. The matrix is
// represented in column order as OpenGL
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PMATRIX3X3_H
#define PMATRIX3X3_H

#include <PFoundation/plog.h>
#include <PFoundation/passert.h>


// Set the matrix to be identity.
pfloat32 * P_APIENTRY pMatrix3x3Identity(pfloat32 *inout);

// dst = src
pfloat32 * P_APIENTRY pMatrix3x3Copy(const pfloat32 *in, pfloat32 *out);

// result = a * b. out can't be the same memory piece of a or b.
pfloat32 * P_APIENTRY pMatrix3x3Multiply(const pfloat32 *a, const pfloat32 *b, pfloat32 *out);

// result = a + b
pfloat32 * P_APIENTRY pMatrix3x3Add(const pfloat32 *a, const pfloat32 *b, pfloat32 *out);

// result = a - b
pfloat32 * P_APIENTRY pMatrix3x3Subtract(const pfloat32 *a, const pfloat32 *b, pfloat32 *out);

// a = transpose(a)
pfloat32 * P_APIENTRY pMatrix3x3Transpose(const pfloat32 *in, pfloat32 *out);

// result = m * v
pfloat32 * P_APIENTRY pMatrix3x3MultiplyVector3(const pfloat32 *m, const pfloat32 *v, pfloat32 *out);

// Return the determinannt of the matrix.
P_INLINE pfloat32 P_APIENTRY pMatrix3x3Determinant(const pfloat32* in);

// Inverse of the matrix. When the matrix is not inversible, an identity matrix
// will be returned.
pfloat32 * P_APIENTRY pMatrix3x3Inverse(const pfloat32 *in, pfloat32 *out);

// Inverse of the transformation matrix which has only translate and rotate.
// When the matrix is not inversible, P_NULL will be returned.
pfloat32 * P_APIENTRY pMatrix3x3AffineInverse(const pfloat32 *in, pfloat32 *out);

// Apply the rotation to the current matrix with values, like we do with glRotatef.
pfloat32 * P_APIENTRY pMatrix3x3Rotate(pfloat32 *inout, pfloat32 radians);

// Create a rotation matrix.
pfloat32 * P_APIENTRY pMatrix3x3CreateRotation(pfloat32 radians, pfloat32 *out);

// Apply the scaling to the current matrix with values, like we do with glScalef.
pfloat32 * P_APIENTRY pMatrix3x3Scale(pfloat32 *inout, pfloat32 x, pfloat32 y);

// Get the scaling factor in 2 axis from the 3x3 matrix.
void P_APIENTRY pMatrix3x3GetScale(const pfloat32 *out, pfloat32 &x, pfloat32 &y);

// Create a scaling matrix.
pfloat32 * P_APIENTRY pMatrix3x3CreateScaling(pfloat32 x, pfloat32 y, pfloat32 *out);

// Create a translation matrix.
pfloat32 * P_APIENTRY pMatrix3x3CreateTranslation(pfloat32 x, pfloat32 y, pfloat32 *out);

// Apply the translation to the current matrix with values, like we do with glTranslatef.
pfloat32 * P_APIENTRY pMatrix3x3Translate(pfloat32 *inout, pfloat32 x, pfloat32 y);

// Interpolate two matrices. The matrix is supposed to be SR matrix (it is conmposed of scaling and rotation)
pfloat32 * P_APIENTRY pMatrix3x3Interpolate(const pfloat32 *a, const pfloat32 *b, pfloat32 t, pfloat32 *out);

// Inverse tranpose the matrix (to get normal matrix)
pfloat32 * P_APIENTRY pMatrix3x3InverseTranspose(const pfloat32 *in, pfloat32 *out);

// Inverse tranpose the matrix (to get normal matrix)
pfloat32 * P_APIENTRY pMatrix3x3InverseTransposeMatrix4x4(const pfloat32 *in, pfloat32 *out);

//
// C++ wrapper
//
#include <PFoundation/pvector3.h>

struct PMatrix3x3
{
    pfloat32 m_m[16];

    P_INLINE pfloat32 operator[](puint32 index) const
    {
        PASSERT(index < 9);
        return m_m[index];
    };

    pfloat32& operator[](puint32 index) 
    {
        PASSERT(index < 9);
        return m_m[index];
    };

    P_INLINE void identity() { pMatrix3x3Identity(m_m); }; 

    P_INLINE PMatrix3x3 operator+(const PMatrix3x3& rightop) const
    { PMatrix3x3 out; pMatrix3x3Add(m_m, rightop.m_m, out.m_m); return out; }
    P_INLINE PMatrix3x3 operator-(const PMatrix3x3& rightop) const
    { PMatrix3x3 out; pMatrix3x3Subtract(m_m, rightop.m_m, out.m_m); return out; }
    P_INLINE PMatrix3x3 operator*(const PMatrix3x3 &rightop) const
    { PMatrix3x3 out; pMatrix3x3Multiply(m_m, rightop.m_m, out.m_m); return out; }
    P_INLINE PVector3 operator*(const PVector3 &rightop) const
    { PVector3 out; pMatrix3x3MultiplyVector3(m_m, rightop.m_v, out.m_v); return out; }

    P_INLINE const PMatrix3x3 &operator+=(const PMatrix3x3& rightop) 
    { pMatrix3x3Add(m_m, rightop.m_m, m_m); return *this; }
    P_INLINE const PMatrix3x3 &operator-=(const PMatrix3x3& rightop) 
    { pMatrix3x3Subtract(m_m, rightop.m_m, m_m); return *this; }
    P_INLINE const PMatrix3x3 &operator*=(const PMatrix3x3 &rightop) 
    { pMatrix3x3Multiply(m_m, rightop.m_m, m_m); return *this; }

    P_INLINE PMatrix3x3 inverse() const
    { PMatrix3x3 out; pMatrix3x3Inverse(m_m, out.m_m); return out; }
    
    P_INLINE PMatrix3x3 affineInverse() const
    { PMatrix3x3 out; pMatrix3x3AffineInverse(m_m, out.m_m); return out; }
    
    PMatrix3x3 transpose() const
    { PMatrix3x3 out; pMatrix3x3Transpose(m_m, out.m_m); return out; }
    
    P_INLINE void translate(pfloat32 x, pfloat32 y) { pMatrix3x3Translate(m_m, x, y); }
    P_INLINE void rotate(pfloat32 angle) { pMatrix3x3Rotate(m_m, angle); }
    P_INLINE void scale(pfloat32 x, pfloat32 y) { pMatrix3x3Scale(m_m, x, y); }

    P_INLINE void createTranslation(pfloat32 x, pfloat32 y) 
    { pMatrix3x3CreateTranslation(x, y, m_m); }
    
    P_INLINE void createRotation(pfloat32 angle) 
    { pMatrix3x3CreateRotation(angle, m_m); }

    P_INLINE void createScaling(pfloat32 x, pfloat32 y) 
    { pMatrix3x3CreateScaling(x, y, m_m); }
};


#endif // !PMATRIX3X3_H
