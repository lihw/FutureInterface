// pstring.h
// Ascii string in C++.
//
// Copyright 2012 - 2014 Future Interface Li. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PSTRING_H
#define PSTRING_H

#include <PFoundation/parray.h>

#include <PFoundation/plog.h>
#include <PFoundation/passert.h>
#include <PFoundation/pnew.h>


class P_DLLEXPORT PString
{
public:
    P_INLINE PString() { m_s = P_NULL; m_l = 0; }
    explicit PString(const PArray<pchar>& o);
    explicit PString(const pchar* o, pint32 l = -1);
    PString(const PString& o);
    explicit PString(puint32 l);
    PString(pchar c, puint32 n);

    ~PString();

    P_INLINE const pchar* data() const { return m_s; }
    P_INLINE pchar* data() {return m_s; }
    P_INLINE puint32 length() const { return m_l; }
    P_INLINE pbool empty() const { return m_l == 0; }

    PString left(puint32 n) const;
    PString right(puint32 n) const;
    PString mid(puint32 position, pint32 n = -1) const;
    
    void insert(pchar* before, pchar* str, pint32 length = -1);
    void insert(pchar* before, const PString& str);

    pint32 find(pchar c) const; // return -1 if not found
    pint32 find(const PString& str) const; // return -1 if not found
    pint32 find(const char* str) const; // return -1 if not found.

    pint32 rfind(pchar c) const;
    pint32 rfind(const PString& str) const;
    pint32 rfind(const char* str) const;
    
    void append(const pchar* str, pint32 length = -1); // length = -1 when it is a c string.
    void append(const PString& str); // length = -1 when it is a c string.
    void prepend(const pchar* str, pint32 length = -1); // length = -1 when it is a c string.
    void prepend(const PString& str); 
    void combine(const pchar *string, ...);

    pbool isEqual(const pchar* str, pint32 length = -1) const;
    P_INLINE pbool isEqual(const PString& str) const
    { return m_l == str.m_l? isEqual(str.m_s, str.m_l) : false; }
    
    P_INLINE pbool operator==(const pchar* o) const
    { return isEqual(o, -1); }
    P_INLINE pbool operator==(const PString& o) const
    { return isEqual(o.m_s); }
    P_INLINE pbool operator==(const PArray<pchar>& o) const
    { return isEqual(&o[0], o.size()); }
    P_INLINE pbool operator!=(const pchar* o) const
    { return !isEqual(o, -1); }
    P_INLINE pbool operator!=(const PString& o) const
    { return !isEqual(o.m_s); }
    P_INLINE pbool operator!=(const PArray<pchar>& o) const
    { return !isEqual(&o[0], o.size()); }

    pbool operator<(const PString& o) const;
    pbool operator>(const PString& o) const;

    P_INLINE const PString& operator=(const pchar* o)
    { assign(o, getLength(o)); return *this; }
    P_INLINE const PString& operator=(const PString& o)
    { if (&o != this) assign(o.m_s, o.length()); return *this; }
    P_INLINE const PString& operator=(const PArray<pchar>& o)
    { assign(&o[0], o.size()); return *this; }

    P_INLINE const PString& operator+=(const pchar* o)
    { append(o, -1); return *this; }
    P_INLINE const PString& operator+=(const PString& o)
    { append(o); return *this; }
    P_INLINE const PString& operator+=(const PArray<pchar>& o)
    { append(&o[0], o.size()); return *this; }

    pchar& operator[](puint32 i) { PASSERTINFO(i <= m_l, "index out of range"); return m_s[i]; }
    pchar operator[](puint32 i) const { PASSERTINFO(i <= m_l, "index out of range"); return m_s[i]; }
    
    P_INLINE PString operator+(const pchar* o) const
    { PString ret(*this); ret += o; return ret; }
    P_INLINE PString operator+(const PString& o) const
    { PString ret(*this); ret += o; return ret; }
    P_INLINE PString operator+(const PArray<pchar>& o) const
    { PString ret(*this); ret += o; return ret; }

    // Compatible with STL
    pchar* c_str() { return m_s; }
    const pchar* c_str() const { return m_s; }

    const PString& sprintf(const pchar *format, ...);

    puint32  toUint() const;
    pint32   toInt() const;
    pfloat32 toFloat() const;

    static PString fromUint(puint32 value);
    static PString fromInt(pint32 value);
    static PString fromFloat(pfloat32 value);

    PString& toLower();
    PString& toUpper();

    PString& replace(const pchar old, const pchar replacement);

private:
    pchar* m_s;
    puint32 m_l;

    void assign(const pchar* string, puint32 length);
    pint32 getLength(const pchar* s);
};
    
#endif // !PSTRING_H
