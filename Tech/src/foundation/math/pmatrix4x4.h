// pmatrix4x4.h 
// 4x4 matrix math used for 3D scaling, rotation and translation. The matrix is
// represented in column order as OpenGL
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PMATRIX4X4_H
#define PMATRIX4X4_H

#include <PFoundation/plog.h>
#include <PFoundation/passert.h>


class PPlane;

// Set the matrix to be identity.
pfloat32 * P_APIENTRY pMatrix4x4Identity(pfloat32 *inout);

// out = a + b
pfloat32 * P_APIENTRY pMatrix4x4Add(const pfloat32 *a, const pfloat32 *b, pfloat32 *out);

// out = a - b
pfloat32 * P_APIENTRY pMatrix4x4Subtract(const pfloat32 *a, const pfloat32 *b, pfloat32 *out);


// out = a * b (out cant be either a or b for performance consideration)
pfloat32 * P_APIENTRY pMatrix4x4Multiply(const pfloat32 *a, const pfloat32 *b, pfloat32 *out);

// Create a translation matrix.
pfloat32 * P_APIENTRY pMatrix4x4CreateTranslation(pfloat32 x,
    pfloat32 y,
    pfloat32 z, 
    pfloat32 *out);

// Apply translation to a matrix, like glTranslatef() does.
pfloat32 * P_APIENTRY pMatrix4x4Translate(pfloat32 *inout, pfloat32 x, pfloat32 y, pfloat32 z);


// Directly modify the translation of the matrix. 
pfloat32 * P_APIENTRY pMatrix4x4SetTranslation(pfloat32 x,
    pfloat32 y, 
    pfloat32 z, 
    pfloat32 *inout);

// Apply rotation to a matrix (rotate with z axis, then with y, then with x)
pfloat32 * P_APIENTRY pMatrix4x4RotateRxRyRz(pfloat32 *inout, pfloat32 rx, pfloat32 ry, pfloat32 rz);

// Apply rotation to a matrix (rotate with x axis, then with y, then with z)
pfloat32 * P_APIENTRY pMatrix4x4RotateRzRyRx(pfloat32 *inout, pfloat32 rx, pfloat32 ry, pfloat32 rz);

// Apply rotation to a matrix, like glRotatef() does.
pfloat32 * P_APIENTRY pMatrix4x4Rotate(pfloat32 *inout, pfloat32 radians, pfloat32 x, pfloat32 y, pfloat32 z);

// Apply rotation with x axis to a matrix
pfloat32 * P_APIENTRY pMatrix4x4RotateRx(pfloat32 *inout, pfloat32 radians);

// Apply rotation with y axis to a matrix
pfloat32 * P_APIENTRY pMatrix4x4RotateRy(pfloat32 *inout, pfloat32 radians);

// Apply rotation with z axis to a matrix
pfloat32 * P_APIENTRY pMatrix4x4RotateRz(pfloat32 *inout, pfloat32 radians);

// Create a rotation matrix that rotate with x axis.
pfloat32 * P_APIENTRY pMatrix4x4CreateRotationRx(pfloat32 radians, pfloat32 *out);

// Create a rotation matrix that rotate with y axis.
pfloat32 * P_APIENTRY pMatrix4x4CreateRotationRy(pfloat32 radians, pfloat32 *out);

// Create a rotation matrix that rotate with z axis.
pfloat32 * P_APIENTRY pMatrix4x4CreateRotationRz(pfloat32 radians, pfloat32 *out);

// Create a rotation matrix that first rotate with x axis, then with y axis and finally with z axis.
pfloat32 * P_APIENTRY pMatrix4x4CreateRotationRzRyRx(pfloat32 rx, pfloat32 ry, pfloat32 rz, pfloat32 *out);

// Create a rotation matrix that first rotate with z axis, then with y axis and finally with y axis.
pfloat32 * P_APIENTRY pMatrix4x4CreateRotationRxRyRz(pfloat32 rx, pfloat32 ry, pfloat32 rz, pfloat32 *out);

// Create a rotation matrix that rotates with an axis for an angle.
pfloat32 * P_APIENTRY pMatrix4x4CreateRotation(pfloat32 radians, pfloat32 x, pfloat32 y, pfloat32 z, pfloat32 *out);

// Get the rotation angle in x axis, in y axis and in z axis from a matrix containing rotation and
// scaling only; suppose the rotation is generated in RxRyRz order.
void P_APIENTRY pMatrix4x4GetRotationRxRyRz(const pfloat32 *matrix, pfloat32 *rx, pfloat32 *ry, pfloat32 *rz);

// Get the rotation angle in x axis, in y axis and in z axis from a matrix containing rotation and
// scaling only; suppose the rotation is generated in RzRyRx order.
void P_APIENTRY pMatrix4x4GetRotationRzRyRx(const pfloat32 *matrix, pfloat32 *rx, pfloat32 *ry, pfloat32 *rz);

// Scale a matrix.
pfloat32 * P_APIENTRY pMatrix4x4Scale(pfloat32 *inout, pfloat32 sx, pfloat32 sy, pfloat32 sz);

// Create a scaling matrix.
pfloat32 * P_APIENTRY pMatrix4x4CreateScaling(pfloat32 sx, pfloat32 sy, pfloat32 sz, pfloat32 *out);

// Get the scaling factor in x , in y and in z from a matrix containing rotation and
// scaling only.
void P_APIENTRY pMatrix4x4GetScaling(const pfloat32 *matrix, pfloat32 *sx, pfloat32 *sy, pfloat32 *sz);

// out = transpose(in)
pfloat32 * P_APIENTRY pMatrix4x4Transpose(const pfloat32 *in, pfloat32 *out);

// result = m * v
pfloat32 * P_APIENTRY pMatrix4x4MultiplyVector4(const pfloat32* m, const pfloat32* v, pfloat32 *out);

// result = m * v
pfloat32 * P_APIENTRY pMatrix4x4MultiplyVector3(const pfloat32* m, const pfloat32* v, pfloat32* out);

pfloat32 P_APIENTRY pMatrix4x4Determinant3x3(const pfloat32 *m);

// Reflection matrix for a given plane
pfloat32 * P_APIENTRY pMatrix4x4CreateReflection(const PPlane &plane, pfloat32 *out);

// Inverse of the transformation matrix which has only translate, rotate and scale.
// When the matrix is not invertible, an identity matrix will be returned.
pfloat32 * P_APIENTRY pMatrix4x4AffineInverse(const pfloat32 *in, pfloat32 *out);

// Transpose and inverse of the transformation matrix which has only translate, rotate and scale.
// When the matrix is not invertible, an identity matrix will be returned.
pfloat32 * P_APIENTRY pMatrix4x4InverseTranspose(const pfloat32 *in, pfloat32 *out);

// Inverse of the transformation matrix which has only translate and rotate.
// When the matrix is not invertible, an identity matrix will be returned.
pfloat32 * P_APIENTRY pMatrix4x4RigidBodyInverse(const pfloat32* in, pfloat32* out);
 
// Create a lookat matrix.
pfloat32 * P_APIENTRY pMatrix4x4LookAt(const pfloat32 *eye, const pfloat32 *center, const pfloat32 *up, 
    pfloat32 *out);

// Create a perspective projection matrix.
pfloat32 * P_APIENTRY pMatrix4x4PerspectiveProjection(pfloat32 fovY, pfloat32 aspect, pfloat32 zNear, 
    pfloat32 zFar, pfloat32 *out);

// Inverse a perspective projection matrix.
pfloat32 * P_APIENTRY pMatrix4x4InversedPerspectiveProjection(pfloat32 fovY, pfloat32 aspect, 
    pfloat32 zNear, pfloat32 zFar, pfloat32 *out);

// Create a orthographic projection matrix.
pfloat32 * P_APIENTRY pMatrix4x4OrthographicProjection(pfloat32 left, pfloat32 right, 
        pfloat32 bottom, pfloat32 top, pfloat32 nearVal, pfloat32 farVal, pfloat32 *out);

// Inverse an orthographic projection matrix.
pfloat32 * P_APIENTRY pMatrix4x4InversedOrthographicProjection(pfloat32 left, pfloat32 right, 
    pfloat32 bottom, pfloat32 top, pfloat32 nearVal, pfloat32 farVal, pfloat32 *out);

// Matrix copy
P_INLINE pfloat32 * P_APIENTRY pMatrix4x4Copy(const pfloat32 *in, pfloat32 *out)
{
    out[0] = in[0];
    out[1] = in[1];
    out[2] = in[2];
    out[3] = in[3];
    out[4] = in[4];
    out[5] = in[5];
    out[6] = in[6];
    out[7] = in[7];
    out[8] = in[8];
    out[9] = in[9];
    out[10] = in[10];
    out[11] = in[11];
    out[12] = in[12];
    out[13] = in[13];
    out[14] = in[14];
    out[15] = in[15];

    return out;
}

pfloat32 * P_APIENTRY pMatrix4x4Interpolate(const pfloat32 *matrix1, const pfloat32 *matrix2, pfloat32 t, pfloat32 *out);

#if defined P_DEBUG
void P_APIENTRY pMatrix4x4Print(const pfloat32 *matrix);
#endif

#include <PFoundation/pvector3.h>
#include <PFoundation/pvector4.h>

struct PMatrix4x4
{
    pfloat32 m_m[16];

    P_INLINE void copy(const PMatrix4x4 &other)
    {
        m_m[0] = other.m_m[0];
        m_m[1] = other.m_m[1];
        m_m[2] = other.m_m[2];
        m_m[3] = other.m_m[3];
        m_m[4] = other.m_m[4];
        m_m[5] = other.m_m[5];
        m_m[6] = other.m_m[6];
        m_m[7] = other.m_m[7];
        m_m[8] = other.m_m[8];
        m_m[9] = other.m_m[9];
        m_m[10] = other.m_m[10];
        m_m[11] = other.m_m[11];
        m_m[12] = other.m_m[12];
        m_m[13] = other.m_m[13];
        m_m[14] = other.m_m[14];
        m_m[15] = other.m_m[15];
    }

    P_INLINE void copy(const pfloat32 *other)
    {
        m_m[0] = other[0];
        m_m[1] = other[1];
        m_m[2] = other[2];
        m_m[3] = other[3];
        m_m[4] = other[4];
        m_m[5] = other[5];
        m_m[6] = other[6];
        m_m[7] = other[7];
        m_m[8] = other[8];
        m_m[9] = other[9];
        m_m[10] = other[10];
        m_m[11] = other[11];
        m_m[12] = other[12];
        m_m[13] = other[13];
        m_m[14] = other[14];
        m_m[15] = other[15];
    }

    P_INLINE pfloat32 operator[](puint32 index) const
    {
        PASSERT(index < 16);
        return m_m[index];
    }

    P_INLINE pfloat32& operator[](puint32 index) 
    {
        PASSERT(index < 16);
        return m_m[index];
    }

    P_INLINE void identity() { pMatrix4x4Identity(m_m); }

    P_INLINE PMatrix4x4 operator+(const PMatrix4x4& rightop) const
    { PMatrix4x4 out; pMatrix4x4Add(m_m, rightop.m_m, out.m_m); return out; }
    P_INLINE PMatrix4x4 operator-(const PMatrix4x4& rightop) const
    { PMatrix4x4 out; pMatrix4x4Subtract(m_m, rightop.m_m, out.m_m); return out; }
    P_INLINE PMatrix4x4 operator*(const PMatrix4x4 &rightop) const
    { PMatrix4x4 out; pMatrix4x4Multiply(m_m, rightop.m_m, out.m_m); return out; }
    P_INLINE PVector3 operator*(const PVector3 &rightop) const
    { PVector3 out; pMatrix4x4MultiplyVector3(m_m, rightop.m_v, out.m_v); return out; }
    P_INLINE PVector4 operator*(const PVector4 &rightop) const
    { PVector4 out; pMatrix4x4MultiplyVector4(m_m, rightop.m_v, out.m_v); return out; }

    P_INLINE const PMatrix4x4 &operator+=(const PMatrix4x4& rightop) 
    { pMatrix4x4Add(m_m, rightop.m_m, m_m); return *this; }
    P_INLINE const PMatrix4x4 &operator-=(const PMatrix4x4& rightop) 
    { pMatrix4x4Subtract(m_m, rightop.m_m, m_m); return *this; }
    P_INLINE const PMatrix4x4 &operator*=(const PMatrix4x4 &rightop) 
    { pMatrix4x4Multiply(m_m, rightop.m_m, m_m); return *this; }

    P_INLINE PVector4 getColumn(puint32 i) const
    {  PASSERT(i < 4); return pVector4(m_m[i * 4], m_m[i * 4 + 1], m_m[i * 4 + 2], m_m[i * 4 + 3]); }

    P_INLINE PMatrix4x4 rigidBodyInverse() const
    { PMatrix4x4 out; pMatrix4x4RigidBodyInverse(m_m, out.m_m); return out; }
    
    P_INLINE PMatrix4x4 affineInverse() const
    { PMatrix4x4 out; pMatrix4x4AffineInverse(m_m, out.m_m); return out; }
    
    P_INLINE PMatrix4x4 transpose() const
    { PMatrix4x4 out; pMatrix4x4Transpose(m_m, out.m_m); return out; }
    
    P_INLINE void translate(pfloat32 x, pfloat32 y, pfloat32 z) { pMatrix4x4Translate(m_m, x, y, z); }
    P_INLINE void rotateRxRyRz(pfloat32 rx, pfloat32 ry, pfloat32 rz) { pMatrix4x4RotateRxRyRz(m_m, rx, ry, rz); }
    P_INLINE void rotateRzRyRx(pfloat32 rx, pfloat32 ry, pfloat32 rz) { pMatrix4x4RotateRzRyRx(m_m, rx, ry, rz); }
    P_INLINE void rotate(pfloat32 radians, pfloat32 x, pfloat32 y, pfloat32 z) { pMatrix4x4Rotate(m_m, radians, x, y, z); }
    P_INLINE void scale(pfloat32 x, pfloat32 y, pfloat32 z) { pMatrix4x4Scale(m_m, x, y, z); }
    
    P_INLINE const PVector3 translation() const
    { return pVector3(m_m[12], m_m[13], m_m[14]); }
    
    P_INLINE void createTranslation(pfloat32 x, pfloat32 y, pfloat32 z) 
    { pMatrix4x4CreateTranslation(x, y, z, m_m); }
    
    P_INLINE void createRotationRzRyRx(pfloat32 rx, pfloat32 ry, pfloat32 rz) 
    { pMatrix4x4CreateRotationRzRyRx(rx, ry, rz, m_m); }
    P_INLINE void createRotationRxRyRz(pfloat32 rx, pfloat32 ry, pfloat32 rz) 
    { pMatrix4x4CreateRotationRxRyRz(rx, ry, rz, m_m); }

    P_INLINE void createRotationRx(pfloat32 radians)
    { pMatrix4x4CreateRotationRx(radians, m_m); }
    P_INLINE void createRotationRy(pfloat32 radians)
    { pMatrix4x4CreateRotationRy(radians, m_m); }
    P_INLINE void createRotationRz(pfloat32 radians)
    { pMatrix4x4CreateRotationRz(radians, m_m); }

    P_INLINE void createScaling(pfloat32 x, pfloat32 y, pfloat32 z) 
    { pMatrix4x4CreateScaling(x, y, z, m_m); }

    P_INLINE void createReflection(const PPlane &plane)
    { pMatrix4x4CreateReflection(plane, m_m); }

#if defined P_DEBUG
    P_INLINE void print() const
    { pMatrix4x4Print(m_m); }
#endif
};





#endif // !PMATRIX4X4_H
