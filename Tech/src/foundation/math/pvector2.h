// pvector2.h 
// vector2 fuctions
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei lihw81@gmail.com
//

#ifndef PVECTOR2_H
#define PVECTOR2_H

#include <PFoundation/plog.h>
#include <PFoundation/passert.h>


// Construct a vector3.
P_INLINE pfloat32 * P_APIENTRY pVector2Fill(pfloat32 x, pfloat32 y, pfloat32 *out)
{
    out[0] = x;
    out[1] = y;

    return out;
}

// out = v1 - v2;
P_INLINE pfloat32 * P_APIENTRY pVector2Subtract(const pfloat32 *v1, const pfloat32 *v2, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = v1[0] - v2[0];
    out[1] = v1[1] - v2[1];
#endif

    return out;
}

// out = v1 + v2;
P_INLINE pfloat32 * P_APIENTRY pVector2Add(const pfloat32 *v1, const pfloat32 *v2, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = v1[0] + v2[0];
    out[1] = v1[1] + v2[1];
#endif

    return out;
}

// component-wise multiplication out = v1 * v2;
P_INLINE pfloat32 * P_APIENTRY pVector2Multiply(const pfloat32 *v1, const pfloat32 *v2, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = v1[0] * v2[0];
    out[1] = v1[1] * v2[1];
#endif

    return out;
}

// scale multiplication out = v1 * c;
P_INLINE pfloat32 * P_APIENTRY pVector2Multiply(const pfloat32 *v1, pfloat32 c, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = v1[0] * c;
    out[1] = v1[1] * c;
#endif

    return out;
}

// The magnitude of the vector3, i.e., |in|
pfloat32 P_APIENTRY pVector2Length(const pfloat32 *in);

// Returns the vector passed in set to unit length 
P_INLINE pfloat32 * P_APIENTRY pVector2Normalize(const pfloat32 *in, pfloat32 *out) 
{
    pfloat32 length = pVector2Length(in);
    if (length == 0)
    {
        return P_NULL;
    }
    
    return pVector2Multiply(in, 1.0f / length, out);
}

// Assigns in to out. Returns out. If in and out are the same
// then nothing happens but out is still returned
P_INLINE pfloat32 * P_APIENTRY pVector2Assign(const pfloat32 *in, pfloat32 *out)
{
    if (out == in)
    {
        return out;
    }

    out[0] = in[0];
    out[1] = in[1];

    return out;
}

// Cross product in right hand order
P_INLINE pfloat32 P_APIENTRY pVector2Cross(const pfloat32 *v1, const pfloat32 *v2)
{
    return v1[0] * v2[1] - v1[1] * v2[0];
}

// Dot product
P_INLINE pfloat32 P_APIENTRY pVector2Dot(const pfloat32 *v1, const pfloat32 *v2)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    return v1[0] * v2[0] + v1[1] * v2[1];
#endif
}

// Component-wise maximum
P_INLINE pfloat32 * P_APIENTRY pVector2Max(const pfloat32 *a, const pfloat32 *b, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = pMax(a[0], b[0]); 
    out[1] = pMax(a[1], b[1]); 
#endif

    return out;
}

// Component-wise minimum
P_INLINE pfloat32 * P_APIENTRY pVector2Min(const pfloat32 *a, const pfloat32 *b, pfloat32 *out)
{
#if defined P_USE_SSE
#elif defined P_USE_NEON
#else
    out[0] = pMin(a[0], b[0]); 
    out[1] = pMin(a[1], b[1]); 
#endif
    
    return out;
}

// Component-wise equal
P_INLINE pbool P_APIENTRY pVector2IsEqual(const pfloat32 *a, const pfloat32 *b)
{
    // FIXME: we doesn't use epsilon in the equivalence comparison. The IEEE754 floating
    // number is not uniformly distributed in the 1D. It is not easy to find a proper
    // epsilon for the whole range of floating numbers.
    return a[0] == b[0] && a[1] == b[1];
}

//
// C++ port
// 
struct P_DLLEXPORT PVector2 
{
    P_INLINE pfloat32 &x() { return m_v[0]; }
    P_INLINE pfloat32 &y() { return m_v[1]; }
    P_INLINE const pfloat32 &x() const { return m_v[0]; }
    P_INLINE const pfloat32 &y() const { return m_v[1]; }

    P_INLINE pfloat32& operator[](puint32 i) 
    { PASSERT(i < 2); return m_v[i]; }
    P_INLINE pfloat32 operator[](puint32 i) const
    { PASSERT(i < 2); return m_v[i]; }

    P_INLINE void normalize() { pVector2Normalize(m_v, m_v); }

    P_INLINE pfloat32 dot(const PVector2 &rightop) const
    { return pVector2Dot(m_v, rightop.m_v); }
    P_INLINE pfloat32 cross(const PVector2 &rightop) const
    { return pVector2Cross(m_v, rightop.m_v); }

    P_INLINE PVector2 operator+(const PVector2 &rightop) const
    { PVector2 out; pVector2Add(m_v, rightop.m_v, out.m_v); return out; }
    P_INLINE PVector2 operator-(const PVector2 &rightop) const
    { PVector2 out; pVector2Subtract(m_v, rightop.m_v, out.m_v); return out; }
    P_INLINE PVector2 operator*(const PVector2 &rightop) const
    { PVector2 out; pVector2Multiply(m_v, rightop.m_v, out.m_v); return out; }
    P_INLINE PVector2 operator*(pfloat32 rightop) const
    { PVector2 out; pVector2Multiply(m_v, rightop, out.m_v); return out; }

    P_INLINE const PVector2 &operator+=(const PVector2 &rightop) 
    { pVector2Add(m_v, rightop.m_v, m_v); return *this; }
    P_INLINE const PVector2 &operator-=(const PVector2 &rightop) 
    { pVector2Subtract(m_v, rightop.m_v, m_v); return *this; }
    P_INLINE const PVector2 &operator*=(const PVector2 &rightop) 
    { pVector2Multiply(m_v, rightop.m_v, m_v); return *this; }
    P_INLINE const PVector2 &operator*=(pfloat32 rightop) 
    { pVector2Multiply(m_v, rightop, m_v); return *this; }

    P_INLINE PVector2 min(const PVector2 &rightop) const
    { PVector2 out; pVector2Min(m_v, rightop.m_v, out.m_v); return out; }
    P_INLINE PVector2 max(const PVector2 &rightop) const
    { PVector2 out; pVector2Max(m_v, rightop.m_v, out.m_v); return out; }
    P_INLINE pbool operator==(const PVector2 &other) const
    { return pVector2IsEqual(m_v, other.m_v); }

    pfloat32  m_v[2];
};

P_INLINE PVector2 P_APIENTRY pVector2(pfloat32 x, pfloat32 y)
{
    PVector2 ret = {x, y};
    return ret;
}

#endif // !PVECTOR2_H
