// pvariant.h
// A container that stores a variant of data types.
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com and Hongwei Li lihw81@gmail.com
//
// 

#ifndef PVARIANT_H
#define PVARIANT_H

#include <PFoundation/pvector2.h>
#include <PFoundation/pvector3.h>
#include <PFoundation/pvector4.h>
#include <PFoundation/pmatrix3x3.h>
#include <PFoundation/pmatrix4x4.h>
#include <PFoundation/pcolorrgba.h>
#include <PFoundation/pstring.h>
    
enum PVariantTypeEnum
{
    P_VARIANT_NIL = 0,
    P_VARIANT_POINTER,
    P_VARIANT_BOOL,
    P_VARIANT_INT,
    P_VARIANT_UINT32,
    P_VARIANT_FLOAT,
    P_VARIANT_VECTOR2,
    P_VARIANT_VECTOR3,
    P_VARIANT_VECTOR4,
    P_VARIANT_MATRIX3X3,
    P_VARIANT_MATRIX4X4,
    P_VARIANT_STRING,
    P_VARIANT_COLOR,

    P_VARIANT_OTHER_START = P_VARIANT_VECTOR2,
    P_VARIANT_TYPE_FIRST = P_VARIANT_NIL,
    P_VARIANT_TYPE_LAST = P_VARIANT_COLOR,
    P_VARIANT_TYPE_NUMBER = P_VARIANT_TYPE_LAST - P_VARIANT_TYPE_FIRST + 1,
};

// FIXME: use a more efficient way instead of PArray<T>.
template<typename T>
struct PVariantArray
{
    PVariantTypeEnum    type;
    PArray<T>           data;
    // copy data to a buffer for upload uniform.
    void               *value;
    puint32             count;

    // we only get function glUniform1iv(), no glUniform1bv().
    #define CONSTRUCTOR_BOOL(name) \
        PVariantArray(const PArray<pbool> &other) \
            : type(name) \
            , data(other) \
        { \
            count = data.size(); \
            value = PNEWARRAY(pint32, count); \
            pint32 *v = (pint32 *)value; \
            for (puint32 i = 0; i < count; i++) \
            { \
                v[i] = data[i]; \
            } \
        }

    #define CONSTRUCTOR_SIMPLE(name, ttype) \
        PVariantArray(const PArray<ttype> &other) \
            : type(name) \
            , data(other) \
        { \
            count = data.size(); \
            value = PNEWARRAY(ttype, count); \
            ttype *v = (ttype *)value; \
            for (puint32 i = 0; i < count; i++) \
            { \
                v[i] = data[i]; \
            } \
        }

    #define CONSTRUCTOR_COMPLEX(name, ttype, vtype, vsize) \
        PVariantArray(const PArray<ttype> &other) \
            : type(name) \
            , data(other) \
        { \
            count = data.size(); \
            value = PNEWARRAY(vtype, count * vsize); \
            vtype *v = (vtype *)value; \
            for (puint32 i = 0; i < count; i++) \
            { \
                for (puint32 j = 0; j < vsize; j++) \
                { \
                    v[i * vsize + j] = data[i][j]; \
                } \
            } \
        }

    CONSTRUCTOR_BOOL(P_VARIANT_BOOL)
    CONSTRUCTOR_SIMPLE(P_VARIANT_INT,           pint32)
    CONSTRUCTOR_SIMPLE(P_VARIANT_FLOAT,         pfloat32)
    CONSTRUCTOR_COMPLEX(P_VARIANT_VECTOR2,      PVector2,   pfloat32,   2)
    CONSTRUCTOR_COMPLEX(P_VARIANT_VECTOR3,      PVector3,   pfloat32,   3)
    CONSTRUCTOR_COMPLEX(P_VARIANT_VECTOR4,      PVector4,   pfloat32,   4)
    CONSTRUCTOR_COMPLEX(P_VARIANT_MATRIX3X3,    PMatrix3x3, pfloat32,   9)
    CONSTRUCTOR_COMPLEX(P_VARIANT_MATRIX3X3,    PMatrix4x4, pfloat32,   16)

    #undef CONSTRUCTOR_BOOL
    #undef CONSTRUCTOR_SIMPLE
    #undef CONSTRUCTOR_COMPLEX
};

class P_DLLEXPORT PVariant
{
public:
    PVariant(); // init with nil 
    PVariant(const PVariant &value);
    explicit PVariant(const void *value);
    explicit PVariant(const pbool &value);
    explicit PVariant(const pint32 &value);
    explicit PVariant(const puint32 &value);
    explicit PVariant(const pfloat32 &value);
    explicit PVariant(const PVector2 &value);
    explicit PVariant(const PVector3 &value);
    explicit PVariant(const PVector4 &value);
    explicit PVariant(const PMatrix3x3 &value);
    explicit PVariant(const PMatrix4x4 &value);
    explicit PVariant(const PColorRGBA &value);
    virtual ~PVariant();

    pbool      toBool() const;
    pint32     toInt() const;
    puint32    toUint32() const;
    pfloat32   toFloat() const;
    PVector2   toVector2() const;
    PVector3   toVector3() const;
    PVector4   toVector4() const;
    PMatrix3x3 toMatrix3x3() const;
    PMatrix4x4 toMatrix4x4() const;
    PColorRGBA toColorRGBA() const;
    PString    toString() const;   
    template<typename T>
    T toPointer() const
    {
        PASSERT(m_type == P_VARIANT_POINTER);
        if (m_type == P_VARIANT_POINTER)
        {
            return reinterpret_cast<T>(m_content.otherValue);
        }
        
        PLOG_WARNING("The variant is not a pointer value");
        return P_NULL;
    }
    
    void operator=(pbool value);
    void operator=(pint32 value);
    void operator=(puint32 value);
    void operator=(pfloat32 value);
    void operator=(const void *value);
    void operator=(const PVector2 &value);
    void operator=(const PVector3 &value);
    void operator=(const PVector4 &value);
    void operator=(const PMatrix3x3 &value);
    void operator=(const PMatrix4x4 &value);
    void operator=(const PColorRGBA &value);
    void operator=(const PString &value);
    const PVariant &operator=(const PVariant &value);

    P_INLINE PVariantTypeEnum getType() const { return m_type; }
    P_INLINE pbool isNil() const { return m_type == P_VARIANT_NIL; }
    P_INLINE pbool canConvert(PVariantTypeEnum type) const { return m_type == type; }

    static PVariant interpolate(const PVariant &left, const PVariant &right, pfloat32 t);

private:
    void setNil();
    void setValue(const PVariant &value);
    void destroy(); // Release the memory.

protected:
    PVariantTypeEnum  m_type;
    union PVariantContent
    {
        pbool       boolValue;
        pint32      intValue;
        puint32     uintValue;
        pfloat32    floatValue;
        void        *otherValue;
    } m_content; 
};


#endif // PVARIANT_H
