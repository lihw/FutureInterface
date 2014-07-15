// ptypes.h
// The basic data types.
// 
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
// 
// Hongwei Li lihw81@gmail.com
// 


#ifndef PTYPES_H
#define PTYPES_H

typedef char            pint8;
typedef unsigned char   puint8;
typedef short           pint16;
typedef unsigned short  puint16;
typedef int             pint32;
typedef unsigned int    puint32;
typedef float           pfloat32;
typedef double          pfloat64;
typedef char            pchar;
typedef bool            pbool;

enum PTypeEnum
{
    P_BYTE,            ///< pint8
    P_UNSIGNED_BYTE,   ///< puint8
    P_SHORT,           ///< pint16
    P_UNSIGNED_SHORT,  ///< puint16
    P_INT,             ///< pint32
    P_UNSIGNED_INT,    ///< puint32
    P_FLOAT32,         ///< pfloat32
    P_FLOAT64,         ///< pfloat64
    P_CHAR,            ///< pchar
    P_BOOLEAN,         ///< pbool
};

#if !defined P_NULL
# define P_NULL 0       
#endif

template<typename T>
class PTypeInfo
{
public:
    enum 
    {
        isPointer = false,
        isComplex = true,
        isLarge   = (sizeof(T) > sizeof(void*)),
        isDummy   = false,
    };
};

template <typename T>
class PTypeInfo<T*>
{
public:
    enum 
    {
        isPointer = true,
        isComplex = false,
        isLarge   = false,
        isDummy   = false,
    };
};

enum 
{
    P_PRIMITIVE_TYPE = 0x01,
    P_DUMMY_TYPE     = 0x02,
};

#define P_DECLARE_TYPEINFO(TYPE, ENUM, FLAGS) \
template<> \
class PTypeInfo<TYPE> \
{ \
public: \
    enum \
    { \
        isComplex = (((FLAGS) & P_PRIMITIVE_TYPE) == 0), \
        isLarge = (sizeof(TYPE)>sizeof(void*)), \
        isPointer = false, \
        isDummy = (((FLAGS) & P_DUMMY_TYPE) != 0) \
    }; \
    static P_INLINE const pchar* name() { return #TYPE; } \
    static P_INLINE const PTypeEnum type() { return ENUM; } \
};

P_DECLARE_TYPEINFO(pint8,    P_BYTE,           P_PRIMITIVE_TYPE)
P_DECLARE_TYPEINFO(puint8,   P_UNSIGNED_BYTE,  P_PRIMITIVE_TYPE)
P_DECLARE_TYPEINFO(pint16,   P_SHORT,          P_PRIMITIVE_TYPE)
P_DECLARE_TYPEINFO(puint16,  P_UNSIGNED_SHORT, P_PRIMITIVE_TYPE)
P_DECLARE_TYPEINFO(pint32,   P_INT,            P_PRIMITIVE_TYPE)
P_DECLARE_TYPEINFO(puint32,  P_UNSIGNED_INT,   P_PRIMITIVE_TYPE)
P_DECLARE_TYPEINFO(pfloat32, P_FLOAT32,        P_PRIMITIVE_TYPE)
P_DECLARE_TYPEINFO(pfloat64, P_FLOAT64,        P_PRIMITIVE_TYPE)
P_DECLARE_TYPEINFO(pbool,     P_BOOLEAN,        P_PRIMITIVE_TYPE)

template<typename T> struct PTypeTrait           { typedef T data_type; };
template<typename T> struct PTypeTrait<const T>  { typedef T data_type; };
template<typename T> struct PTypeTrait<T&>       { typedef T data_type; };
template<typename T> struct PTypeTrait<T*>       { typedef T data_type; };
template<typename T> struct PTypeTrait<const T*> { typedef T data_type; };
template<typename T> struct PTypeTrait<T* const> { typedef T data_type; };
template<typename T> struct PTypeTrait<const T&> { typedef T data_type; };


#endif // !PTYPES_H 
