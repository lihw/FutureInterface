// pvector3.h 
// vector3 fuctions
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei lihw81@gmail.com
//


#ifndef PVECTOR3_H
#define PVECTOR3_H

#include <PFoundation/plog.h>
#include <PFoundation/passert.h>


// Construct a vector3.
P_INLINE pfloat32 * P_APIENTRY pVector3Fill(pfloat32 x, pfloat32 y, pfloat32 z, pfloat32 *out)
{
    out[0] = x;
    out[1] = y;
    out[2] = z;

    return out;
}

// out = v1 - v2;
P_INLINE pfloat32 * P_APIENTRY pVector3Subtract(const pfloat32 *v1, const pfloat32 *v2, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = v1[0] - v2[0];
    out[1] = v1[1] - v2[1];
    out[2] = v1[2] - v2[2];
#endif

    return out;
}

// out = v1 + v2;
P_INLINE pfloat32 * P_APIENTRY pVector3Add(const pfloat32 *v1, const pfloat32 *v2, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = v1[0] + v2[0];
    out[1] = v1[1] + v2[1];
    out[2] = v1[2] + v2[2];
#endif

    return out;
}

// component-wise multiplication out = v1 * v2;
P_INLINE pfloat32 * P_APIENTRY pVector3Multiply(const pfloat32 *v1, const pfloat32 *v2, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = v1[0] * v2[0];
    out[1] = v1[1] * v2[1];
    out[2] = v1[2] * v2[2];
#endif

    return out;
}

// scale multiplication out = v1 * c;
P_INLINE pfloat32 * P_APIENTRY pVector3Multiply(const pfloat32 *v1, pfloat32 c, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = v1[0] * c;
    out[1] = v1[1] * c;
    out[2] = v1[2] * c;
#endif

    return out;
}

// The magnitude of the vector3, i.e., |in|
pfloat32 P_APIENTRY pVector3Length(const pfloat32 *in);

#if defined P_DEBUG
void P_APIENTRY pVector3Print(const pfloat32 *vector3);
#endif

// Returns the vector passed in set to unit length 
P_INLINE pfloat32 * P_APIENTRY pVector3Normalize(const pfloat32 *in, pfloat32 *out) 
{
    pfloat32 length = pVector3Length(in);
    if (length == 0)
    {
        return P_NULL;
    }
    
    return pVector3Multiply(in, 1.0f / length, out);
}

// Assigns in to out. Returns out. If in and out are the same
// then nothing happens but out is still returned
P_INLINE pfloat32 * P_APIENTRY pVector3Assign(const pfloat32 *in, pfloat32 *out)
{
    if (out == in)
    {
        return out;
    }

    out[0] = in[0];
    out[1] = in[1];
    out[2] = in[2];

    return out;
}

// Cross product in right hand order
P_INLINE pfloat32 * P_APIENTRY pVector3Cross(const pfloat32 *v1, const pfloat32 *v2, pfloat32 *out)
{
    pfloat32 v[3];

    v[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    v[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    v[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);

    out[0] = v[0];
    out[1] = v[1];
    out[2] = v[2];

    return out;
}

// Dot product
P_INLINE pfloat32 P_APIENTRY pVector3Dot(const pfloat32 *v1, const pfloat32 *v2)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]; 
#endif
}

// Project vector a to vector b.
P_INLINE void P_APIENTRY pVector3Project(const pfloat32 *a, const pfloat32 *b, pfloat32 *out)
{
    pfloat32 dot = pVector3Dot(a, b);
    pVector3Multiply(a, dot, out);
}

// Calculate the reflect vector of an incident vector with given axis, same as reflect in GLSL.
P_INLINE pfloat32 * P_APIENTRY pVector3Reflect(const pfloat32 *i, const pfloat32 *axis, pfloat32 *out)
{
    pfloat32 d = pVector3Dot(i, axis) * 2.0f;
    pfloat32 temp[3];
    // out = i - d * axis;
    return pVector3Subtract(i, pVector3Multiply(axis, d, temp), out);
}

// component-wise maximum
P_INLINE pfloat32 * P_APIENTRY pVector3Max(const pfloat32 *a, const pfloat32 *b, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = pMax(a[0], b[0]); 
    out[1] = pMax(a[1], b[1]); 
    out[2] = pMax(a[2], b[2]);
#endif

    return out;
}

// component-wise minimum
P_INLINE pfloat32 * P_APIENTRY pVector3Min(const pfloat32 *a, const pfloat32 *b, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = pMin(a[0], b[0]); 
    out[1] = pMin(a[1], b[1]); 
    out[2] = pMin(a[2], b[2]);
#endif
    
    return out;
}

// Component-wise equal
P_INLINE pbool P_APIENTRY pVector3IsEqual(const pfloat32 *a, const pfloat32 *b)
{
    // FIXME: we doesn't use epsilon in the equivalence comparison. The IEEE754 floating
    // number is not uniformly distributed in the 1D. It is not easy to find a proper
    // epsilon for the whole range of floating numbers.
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

//
// C++ port
// 
struct P_DLLEXPORT PVector3 
{
    P_INLINE pfloat32 &x() { return m_v[0]; }
    P_INLINE pfloat32 &y() { return m_v[1]; }
    P_INLINE pfloat32 &z() { return m_v[2]; }
    P_INLINE pfloat32 x() const { return m_v[0]; }
    P_INLINE pfloat32 y() const { return m_v[1]; }
    P_INLINE pfloat32 z() const { return m_v[2]; }
    
    P_INLINE pfloat32& operator[](puint32 i) 
    { PASSERT(i < 3); return m_v[i]; }
    P_INLINE pfloat32 operator[](puint32 i) const
    { PASSERT(i < 3); return m_v[i]; }

    P_INLINE void normalize() { pVector3Normalize(m_v, m_v); }

    P_INLINE pfloat32 dot(const PVector3 &rightop) const
    { return pVector3Dot(m_v, rightop.m_v); }
    P_INLINE PVector3 cross(const PVector3 &rightop) const
    { PVector3 out; pVector3Cross(m_v, rightop.m_v, out.m_v); return out; }
    P_INLINE pfloat32 length() const
    { return pVector3Length(m_v); }

    P_INLINE PVector3 operator+(const PVector3 &rightop) const
    { PVector3 out; pVector3Add(m_v, rightop.m_v, out.m_v); return out; }
    P_INLINE PVector3 operator-(const PVector3 &rightop) const
    { PVector3 out; pVector3Subtract(m_v, rightop.m_v, out.m_v); return out; }
    P_INLINE PVector3 operator*(const PVector3 &rightop) const
    { PVector3 out; pVector3Multiply(m_v, rightop.m_v, out.m_v); return out; }
    P_INLINE PVector3 operator*(pfloat32 rightop) const
    { PVector3 out; pVector3Multiply(m_v, rightop, out.m_v); return out; }
    
    P_INLINE const PVector3 &operator+=(const PVector3 &rightop) 
    { pVector3Add(m_v, rightop.m_v, m_v); return *this; }
    P_INLINE const PVector3 &operator-=(const PVector3 &rightop) 
    { pVector3Subtract(m_v, rightop.m_v, m_v); return *this; }
    P_INLINE const PVector3 &operator*=(const PVector3 &rightop) 
    { pVector3Multiply(m_v, rightop.m_v, m_v); return *this; }
    P_INLINE const PVector3 &operator*=(pfloat32 rightop) 
    { pVector3Multiply(m_v, rightop, m_v); return *this; }

    P_INLINE PVector3 project(const PVector3 &to) const
    { PVector3 out; pVector3Project(m_v, to.m_v, out.m_v); return out; }
    P_INLINE PVector3 reflect(const PVector3 &axis) const
    { PVector3 out; pVector3Reflect(m_v, axis.m_v, out.m_v); return out; }
    P_INLINE PVector3 min(const PVector3 &rightop) const
    { PVector3 out; pVector3Min(m_v, rightop.m_v, out.m_v); return out; }
    P_INLINE PVector3 max(const PVector3 &rightop) const
    { PVector3 out; pVector3Max(m_v, rightop.m_v, out.m_v); return out; }
    P_INLINE pbool operator==(const PVector3 &other) const
    { return pVector3IsEqual(m_v, other.m_v); }

#if defined P_DEBUG
    P_INLINE void print() const
    {
        pVector3Print(m_v); 
    }
#endif

    pfloat32  m_v[3];
};

P_INLINE PVector3 P_APIENTRY pVector3(pfloat32 x, pfloat32 y, pfloat32 z)
{
    PVector3 ret = {x, y, z};
    return ret;
}

#endif // !PVECTOR3_H
