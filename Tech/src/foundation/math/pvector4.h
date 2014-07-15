// pvector3.h 
// vector3 fuctions
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei lihw81@gmail.com
//

#ifndef PVECTOR4_H
#define PVECTOR4_H

#include <PFoundation/pvector3.h>

#include <PFoundation/plog.h>
#include <PFoundation/passert.h>

// Construct a vector3.
P_INLINE pfloat32 * P_APIENTRY pVector4Fill(pfloat32 x, pfloat32 y, pfloat32 z, pfloat32 w, pfloat32 *out)
{
    out[0] = x;
    out[1] = y;
    out[2] = z;
    out[3] = w;

    return out;
}

// The magnitude of the vector3, i.e., |in|
pfloat32 P_APIENTRY pVector4Length(const pfloat32 *in);

// out = v1 - v2;
P_INLINE pfloat32 * P_APIENTRY pVector4Subtract(const pfloat32 *v1, const pfloat32 *v2, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = v1[0] - v2[0];
    out[1] = v1[1] - v2[1];
    out[2] = v1[2] - v2[2];
    out[3] = v1[3] - v2[3];
#endif

    return out;
}

// out = v1 + v2;
P_INLINE pfloat32 * P_APIENTRY pVector4Add(const pfloat32 *v1, const pfloat32 *v2, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = v1[0] + v2[0];
    out[1] = v1[1] + v2[1];
    out[2] = v1[2] + v2[2];
    out[3] = v1[3] + v2[3];
#endif

    return out;
}

// component-wise multiplication out = v1 * v2;
P_INLINE pfloat32 * P_APIENTRY pVector4Multiply(const pfloat32 *v1, const pfloat32 *v2, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = v1[0] * v2[0];
    out[1] = v1[1] * v2[1];
    out[2] = v1[2] * v2[2];
    out[3] = v1[3] * v2[3];
#endif

    return out;
}

// scale multiplication out = v1 * c;
P_INLINE pfloat32 * P_APIENTRY pVector4Multiply(const pfloat32 *v1, pfloat32 c, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = v1[0] * c;
    out[1] = v1[1] * c;
    out[2] = v1[2] * c;
    out[3] = v1[3] * c;
#endif

    return out;
}

// Returns the vector passed in set to unit length 
P_INLINE pfloat32 * P_APIENTRY pVector4Normalize(const pfloat32 *in, pfloat32 *out)
{
    pfloat32 length = pVector4Length(in);
    if (length == 0)
    {
        return P_NULL;
    }
    
    pfloat32 lenInv = 1.0f / length;

    return pVector4Multiply(in, lenInv, out);    
}

// Assigns in to out. Returns out. If in and out are the same
// then nothing happens but out is still returned
P_INLINE pfloat32 * P_APIENTRY pVector4Assign(const pfloat32 *in, pfloat32 *out)
{
    if (in == out)
    {
        return out;
    }

    out[0] = in[0];
    out[1] = in[1];
    out[2] = in[2];
    out[3] = in[3];

    return out;
}

// Dot product
P_INLINE pfloat32 P_APIENTRY pVector4Dot(const pfloat32 *v1, const pfloat32 *v2)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2] + v1[3] * v2[3]; 
#endif
}

// component-wise maximum
P_INLINE pfloat32 * P_APIENTRY pVector4Max(const pfloat32 *a, const pfloat32 *b, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = pMax(a[0], b[0]); 
    out[1] = pMax(a[1], b[1]); 
    out[2] = pMax(a[2], b[2]);
    out[3] = pMax(a[3], b[3]);
#endif

    return out;
}

// component-wise minimum
P_INLINE pfloat32 * P_APIENTRY pVector4Min(const pfloat32 *a, const pfloat32 *b, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = pMin(a[0], b[0]); 
    out[1] = pMin(a[1], b[1]); 
    out[2] = pMin(b[2], b[2]);
    out[3] = pMin(b[3], b[3]);
#endif
    
    return out;
}

// Component-wise equal
P_INLINE pbool P_APIENTRY pVector4IsEqual(const pfloat32 *a, const pfloat32 *b)
{
    // FIXME: we doesn't use epsilon in the equivalence comparison. The IEEE754 floating
    // number is not uniformly distributed in the 1D. It is not easy to find a proper
    // epsilon for the whole range of floating numbers.
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
}

//
// C++ port
// 
struct P_DLLEXPORT PVector4 
{
    P_INLINE pfloat32 &x() { return m_v[0]; }
    P_INLINE pfloat32 &y() { return m_v[1]; }
    P_INLINE pfloat32 &z() { return m_v[2]; }
    P_INLINE pfloat32 &w() { return m_v[3]; }
    P_INLINE pfloat32 x() const { return m_v[0]; }
    P_INLINE pfloat32 y() const { return m_v[1]; }
    P_INLINE pfloat32 z() const { return m_v[2]; }
    P_INLINE pfloat32 w() const { return m_v[3]; }
    P_INLINE PVector3 xyz() const 
    { PVector3 ret = {x(), y(), z()}; return ret; }
    
    P_INLINE pfloat32& operator[](puint32 i) 
    { PASSERT(i < 4); return m_v[i]; }
    P_INLINE pfloat32 operator[](puint32 i) const
    { PASSERT(i < 4); return m_v[i]; }

    P_INLINE pfloat32 dot(const PVector4 &other) const
    { return pVector4Dot(m_v, other.m_v); }

    P_INLINE void normalize() { pVector4Normalize(m_v, m_v); }

    P_INLINE PVector4 operator+(const PVector4 &rightop) const
    { PVector4 out; pVector4Add(m_v, rightop.m_v, out.m_v); return out; }
    P_INLINE PVector4 operator-(const PVector4 &rightop) const
    { PVector4 out; pVector4Subtract(m_v, rightop.m_v, out.m_v); return out; }
    P_INLINE PVector4 operator*(const PVector4 &rightop) const
    { PVector4 out; pVector4Multiply(m_v, rightop.m_v, out.m_v); return out; }
    P_INLINE PVector4 operator*(pfloat32 rightop) const
    { PVector4 out; pVector4Multiply(m_v, rightop, out.m_v); return out; }
    
    P_INLINE const PVector4 &operator+=(const PVector4 &rightop) 
    { pVector4Add(m_v, rightop.m_v, m_v); return *this; }
    P_INLINE const PVector4 &operator-=(const PVector4 &rightop) 
    { pVector4Subtract(m_v, rightop.m_v, m_v); return *this; }
    P_INLINE const PVector4 &operator*=(const PVector4 &rightop) 
    { pVector4Multiply(m_v, rightop.m_v, m_v); return *this; }
    P_INLINE const PVector4 &operator*=(pfloat32 rightop) 
    { pVector4Multiply(m_v, rightop, m_v); return *this; }

    P_INLINE PVector4 min(const PVector4 &rightop) const
    { PVector4 out; pVector4Min(m_v, rightop.m_v, out.m_v); return out; }
    P_INLINE PVector4 max(const PVector4 &rightop) const
    { PVector4 out; pVector4Max(m_v, rightop.m_v, out.m_v); return out; }
    P_INLINE pbool operator==(const PVector4 &other) const
    { return pVector4IsEqual(m_v, other.m_v); }

    pfloat32  m_v[4];
};

P_INLINE PVector4 P_APIENTRY pVector4(pfloat32 x, pfloat32 y, pfloat32 z, pfloat32 w)
{
    PVector4 ret = {x, y, z, w};
    return ret;
}

#endif // !PVECTOR4_H
