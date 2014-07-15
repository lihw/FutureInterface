// pvariant.cpp
// A container that stores a variant of data types.
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com and Hongwei Li lihw81@gmail.com
//
// 

#include "pvariant.h"
 
PVariant::PVariant()
{
    m_type = P_VARIANT_NIL;

    pmemset(&m_content, 0, sizeof(m_content));
}

PVariant::PVariant(const PVariant &value)
{
    m_type = P_VARIANT_NIL;
    setValue(value);
}
    
const PVariant &PVariant::operator=(const PVariant &value)
{
    if (&value != this)
    {
        setValue(value);
    }

    return *this;
}

PVariant::PVariant(const void *value)
{
    m_type = P_VARIANT_POINTER;
    m_content.otherValue = const_cast<void *>(value);
}

PVariant::PVariant(const pbool &value)
{
    m_type = P_VARIANT_BOOL;
    m_content.boolValue = value;
}

PVariant::PVariant(const pint32 &value)
{
    m_type = P_VARIANT_INT;
    m_content.intValue = value;
}

PVariant::PVariant(const puint32 &value)
{
    m_type = P_VARIANT_UINT32;
    m_content.uintValue = value;
}
PVariant::PVariant(const pfloat32 &value)
{
    m_type = P_VARIANT_FLOAT;
    m_content.floatValue = value;
}

PVariant::PVariant(const PVector2 &value)
{
    m_type = P_VARIANT_VECTOR2;
    m_content.otherValue = PNEW(PVector2(value));
}

PVariant::PVariant(const PVector3 &value)
{
    m_type = P_VARIANT_VECTOR3;
    m_content.otherValue = PNEW(PVector3(value));
}

PVariant::PVariant(const PVector4 &value)
{
    m_type = P_VARIANT_VECTOR4;
    m_content.otherValue = PNEW(PVector4(value));
}

PVariant::PVariant(const PMatrix3x3 &value)
{
    m_type = P_VARIANT_MATRIX3X3;
    m_content.otherValue = PNEW(PMatrix3x3(value));
}

PVariant::PVariant(const PMatrix4x4 &value)
{
    m_type = P_VARIANT_MATRIX4X4;
    m_content.otherValue = PNEW(PMatrix4x4(value));
}

PVariant::PVariant(const PColorRGBA &value)
{
    m_type = P_VARIANT_COLOR;
    m_content.otherValue = PNEW(PColorRGBA(value));
}

PVariant::~PVariant()
{
    destroy();
}

pbool PVariant::toBool() const
{
    PASSERT(m_type == P_VARIANT_BOOL);
    if (m_type == P_VARIANT_BOOL)
    {
        return m_content.boolValue;
    }

    PLOG_WARNING("the variant is not a pbool value");
    return false;
}

pint32 PVariant::toInt() const
{
    PASSERT(m_type == P_VARIANT_INT);
    if (m_type == P_VARIANT_INT)
    {
        return m_content.intValue;
    }

    PLOG_WARNING("the variant is not a int32 value");
    return 0;
}

puint32 PVariant::toUint32() const
{
    PASSERT(m_type == P_VARIANT_UINT32);
    if (m_type == P_VARIANT_UINT32)
    {
        return m_content.uintValue;
    }

    PLOG_WARNING("the variant is not a uint32 value");
    return 0;
}
pfloat32 PVariant::toFloat() const
{
    PASSERT(m_type == P_VARIANT_FLOAT);
    if (m_type == P_VARIANT_FLOAT)
    {
        return m_content.floatValue;
    }

    PLOG_WARNING("the variant is not a float32 value");
    return 0.0f;
}

PVector2 PVariant::toVector2() const
{
    PASSERT(m_type == P_VARIANT_VECTOR2);
    if (m_type == P_VARIANT_VECTOR2)
    {
        return *reinterpret_cast<PVector2 *>(m_content.otherValue);
    }

    PLOG_WARNING("the variant is not a PVector2");
    return PVector2();
}

PVector3 PVariant::toVector3() const
{
    PASSERT(m_type == P_VARIANT_VECTOR3);
    if (m_type == P_VARIANT_VECTOR3)
    {
        return *reinterpret_cast<PVector3 *>(m_content.otherValue);
    }

    PLOG_WARNING("the variant is not a PVector3");
    return PVector3();
}

PVector4 PVariant::toVector4() const
{
    PASSERT(m_type == P_VARIANT_VECTOR4);
    if (m_type == P_VARIANT_VECTOR4)
    {
        return *reinterpret_cast<PVector4 *>(m_content.otherValue);
    }

    PLOG_WARNING("the variant is not a PVector4");
    return PVector4();
}

PMatrix3x3 PVariant::toMatrix3x3() const
{
    PASSERT(m_type == P_VARIANT_MATRIX3X3);
    if (m_type == P_VARIANT_MATRIX3X3)
    {
        return *reinterpret_cast<PMatrix3x3 *>(m_content.otherValue);
    }

    PLOG_WARNING("the variant is not a PMatrix3x3");
    return PMatrix3x3();
}

PMatrix4x4 PVariant::toMatrix4x4() const
{
    PASSERT(m_type == P_VARIANT_MATRIX4X4);
    if (m_type == P_VARIANT_MATRIX4X4)
    {
        return *reinterpret_cast<PMatrix4x4 *>(m_content.otherValue);
    }

    PLOG_WARNING("the variant is not a PMatrix4x4");
    return PMatrix4x4();
}

PColorRGBA PVariant::toColorRGBA() const
{
    PASSERT(m_type == P_VARIANT_COLOR);
    if (m_type == P_VARIANT_COLOR)
    {
        return *reinterpret_cast<PColorRGBA *>(m_content.otherValue);
    }

    PLOG_WARNING("the variant is not a PColorRGBA");
    return PColorRGBA();
}

void PVariant::operator=(const void *value)
{
    PASSERT(m_type == P_VARIANT_POINTER);
    if (m_type == P_VARIANT_POINTER)
    {
        m_content.otherValue = const_cast<void *>(value);
    }
}

void PVariant::operator=(pbool value)
{
    PASSERT(m_type == P_VARIANT_BOOL);
    if (m_type == P_VARIANT_BOOL)
    {
        m_content.boolValue = value;
    }
}

void PVariant::operator=(pint32 value)
{
    PASSERT(m_type == P_VARIANT_INT);
    if (m_type == P_VARIANT_INT)
    {
        m_content.intValue = value;
    }
}

void PVariant::operator=(puint32 value)
{
    PASSERT(m_type == P_VARIANT_UINT32);
    if (m_type == P_VARIANT_UINT32)
    {
        m_content.uintValue = value;
    }
}
void PVariant::operator=(pfloat32 value)
{
    PASSERT(m_type == P_VARIANT_FLOAT);
    if (m_type == P_VARIANT_FLOAT)
    {
        m_content.floatValue = value;
    }
}

void PVariant::operator=(const PVector2 &value)
{
    PASSERT(m_type == P_VARIANT_VECTOR2);
    if (m_type == P_VARIANT_VECTOR2)
    {
        pmemcpy(m_content.otherValue, &value, sizeof(value));
    }
}

void PVariant::operator=(const PVector3 &value)
{
    PASSERT(m_type == P_VARIANT_VECTOR3);

    if (m_type == P_VARIANT_VECTOR3)
    {
        pmemcpy(m_content.otherValue, &value, sizeof(value));
    }
}

void PVariant::operator=(const PVector4 &value)
{
    PASSERT(m_type == P_VARIANT_VECTOR4);

    if (m_type == P_VARIANT_VECTOR4)
    {
        pmemcpy(m_content.otherValue, &value, sizeof(value));
    }
}

void PVariant::operator=(const PMatrix3x3 &value)
{
    PASSERT(m_type == P_VARIANT_MATRIX3X3);
    if (m_type == P_VARIANT_MATRIX3X3)
    {
        pmemcpy(m_content.otherValue, &value, sizeof(value));
    }
}

void PVariant::operator=(const PMatrix4x4 &value)
{
    PASSERT(m_type == P_VARIANT_MATRIX4X4);
    if (m_type == P_VARIANT_MATRIX4X4)
    {
        pmemcpy(m_content.otherValue, &value, sizeof(value));
    }
}

void PVariant::operator=(const PColorRGBA &value)
{
    PASSERT(m_type == P_VARIANT_COLOR);
    if (m_type == P_VARIANT_COLOR)
    {
        pmemcpy(m_content.otherValue, &value, sizeof(value));
    }
}

void PVariant::destroy()
{
    switch (m_type)
    {
    case P_VARIANT_VECTOR2:
        {
            PVector2 *vector2 = reinterpret_cast<PVector2 *>(m_content.otherValue);
            PDELETE(vector2);
            break;
        }
    case P_VARIANT_VECTOR3:
        {
            PVector3 *vector3 = reinterpret_cast<PVector3 *>(m_content.otherValue);
            PDELETE(vector3);
            break;
        }
    case P_VARIANT_VECTOR4:
        {
            PVector4 *vector4 = reinterpret_cast<PVector4 *>(m_content.otherValue);
            PDELETE(vector4);
            break;
        }
    case P_VARIANT_MATRIX3X3:
        {
            PMatrix3x3 *matrix3x3 = reinterpret_cast<PMatrix3x3 *>(m_content.otherValue);
            PDELETE(matrix3x3);
            break;
        }
    case P_VARIANT_MATRIX4X4:
        {
            PMatrix4x4 *matrix4x4 = reinterpret_cast<PMatrix4x4 *>(m_content.otherValue);
            PDELETE(matrix4x4);
            break;
        }
    case P_VARIANT_COLOR:
        {
            PColorRGBA *color = reinterpret_cast<PColorRGBA *>(m_content.otherValue);
            PDELETE(color);
            break;
        }
    case P_VARIANT_POINTER:
    case P_VARIANT_BOOL:
    case P_VARIANT_INT:
    case P_VARIANT_UINT32:
    case P_VARIANT_FLOAT:
    case P_VARIANT_NIL:
        break;
    default:
        PASSERT_NOTREACHABLE("Unknown variant type.");
        break;
    }
}

void PVariant::setNil()
{
    destroy();
    m_type = P_VARIANT_NIL;
}

void PVariant::setValue(const PVariant &value)
{
    if (m_type != value.m_type)
    {
        destroy();
        m_type = value.m_type;

        switch (m_type)
        {
        case P_VARIANT_VECTOR2:
            m_content.otherValue = PNEW(PVector2(value.toVector2()));
            break;
        case P_VARIANT_VECTOR3:
            m_content.otherValue = PNEW(PVector3(value.toVector3()));
            break;
        case P_VARIANT_VECTOR4:
            m_content.otherValue = PNEW(PVector4(value.toVector4()));
            break;
        case P_VARIANT_MATRIX3X3:
            m_content.otherValue = PNEW(PMatrix3x3(value.toMatrix3x3()));
            break;
        case P_VARIANT_MATRIX4X4:
            m_content.otherValue = PNEW(PMatrix4x4(value.toMatrix4x4()));
            break;
        case P_VARIANT_COLOR:
            m_content.otherValue = PNEW(PColorRGBA(value.toColorRGBA()));
            break;
        case P_VARIANT_POINTER:
        case P_VARIANT_BOOL:
        case P_VARIANT_INT:
        case P_VARIANT_UINT32:
        case P_VARIANT_FLOAT:
        case P_VARIANT_NIL:
            m_content = value.m_content;
            break;
        default:
            PASSERT_NOTREACHABLE("Unknown variant type.");
            break;
        }
    }
    else
    {
        switch (m_type)
        {
        case P_VARIANT_VECTOR2:
            pmemcpy(m_content.otherValue, value.m_content.otherValue, sizeof(PVector2));
            break;
        case P_VARIANT_VECTOR3:
            pmemcpy(m_content.otherValue, value.m_content.otherValue, sizeof(PVector3));
            break;
        case P_VARIANT_VECTOR4:
            pmemcpy(m_content.otherValue, value.m_content.otherValue, sizeof(PVector4));
            break;
        case P_VARIANT_MATRIX3X3:
            pmemcpy(m_content.otherValue, value.m_content.otherValue, sizeof(PMatrix3x3));
            break;
        case P_VARIANT_MATRIX4X4:
            pmemcpy(m_content.otherValue, value.m_content.otherValue, sizeof(PMatrix4x4));
            break;
        case P_VARIANT_COLOR:
            pmemcpy(m_content.otherValue, value.m_content.otherValue, sizeof(PColorRGBA));
            break;
        case P_VARIANT_POINTER:
        case P_VARIANT_BOOL:
        case P_VARIANT_INT:
        case P_VARIANT_UINT32:
        case P_VARIANT_FLOAT:
        case P_VARIANT_NIL:
            m_content = value.m_content;
            break;
        default:
            PASSERT_NOTREACHABLE("Unknown variant type.");
            break;
        }
    }
}

PVariant PVariant::interpolate(const PVariant &left, const PVariant &right, pfloat32 t)
{
    PASSERT(left.getType() == right.getType());

    switch (left.getType())
    {
        case P_VARIANT_NIL: return PVariant(); break;
        case P_VARIANT_POINTER: return PVariant(); break;
        case P_VARIANT_BOOL: 
            return t < 0.5f? PVariant(left.toBool()) : PVariant(right.toBool());
            break;
        case P_VARIANT_INT:
            {
                pint32 v = 
                    pint32((pfloat32)left.m_content.intValue * (1.0f - t) + 
                           (pfloat32)right.m_content.intValue * t);
                return PVariant(v);
            }
            break;
        case P_VARIANT_UINT32:
            {
                puint32 v = 
                    puint32((pfloat32)left.m_content.intValue * (1.0f - t) + 
                           (pfloat32)right.m_content.intValue * t);
                return PVariant(v);
            }
            break;
        case P_VARIANT_FLOAT:
            {
                pfloat32 v = 
                    left.m_content.floatValue * (1.0f - t) + 
                    right.m_content.floatValue * t;
                return PVariant(v);
            }
            break;
        case P_VARIANT_VECTOR2:
            {
                PVector2 v;
                PVector2 leftVector2 = left.toVector2();
                PVector2 rightVector2 = right.toVector2();

                v.m_v[0] = 
                    leftVector2[0] * (1.0f - t) + 
                    rightVector2[0] * t;
                v.m_v[1] = 
                    leftVector2[1] * (1.0f - t) + 
                    rightVector2[1] * t;
                return PVariant(v);
            }
            break;
        case P_VARIANT_VECTOR3:
            {
                PVector3 v;
                PVector3 leftVector3 = left.toVector3();
                PVector3 rightVector3 = right.toVector3();

                v.m_v[0] = 
                    leftVector3[0] * (1.0f - t) + 
                           rightVector3[0] * t;
                v.m_v[1] = 
                    leftVector3[1] * (1.0f - t) + 
                           rightVector3[1] * t;
                v.m_v[2] = 
                    leftVector3[2] * (1.0f - t) + 
                           rightVector3[2] * t;
                return PVariant(v);
            }
            break;
        case P_VARIANT_VECTOR4:
            {
                PVector4 v;
                PVector4 leftVector4 = left.toVector4();
                PVector4 rightVector4 = right.toVector4();

                v.m_v[0] = 
                    leftVector4[0] * (1.0f - t) + 
                    rightVector4[0] * t;
                v.m_v[1] = 
                    leftVector4[1] * (1.0f - t) + 
                    rightVector4[1] * t;
                v.m_v[2] = 
                    leftVector4[2] * (1.0f - t) + 
                    rightVector4[2] * t;
                v.m_v[3] = 
                    leftVector4[3] * (1.0f - t) + 
                    rightVector4[3] * t;
                return PVariant(v);
            }
            break;
        case P_VARIANT_COLOR:
            {
                PColorRGBA leftColor = left.toColorRGBA();
                PColorRGBA righColor = right.toColorRGBA();

                pfloat32 r = 
                    leftColor.getRFloat() * (1.0f - t) + 
                    righColor.getRFloat() * t;
                pfloat32 g = 
                    leftColor.getGFloat() * (1.0f - t) + 
                    righColor.getGFloat() * t;
                pfloat32 b = 
                    leftColor.getBFloat() * (1.0f - t) + 
                    righColor.getBFloat() * t;
                pfloat32 a = 
                    leftColor.getAFloat() * (1.0f - t) + 
                    righColor.getAFloat() * t;
                PColorRGBA v;
                v.setRFloat(r);
                v.setGFloat(g);
                v.setBFloat(b);
                v.setAFloat(a);

                return PVariant(v);
            }
            break;
        case P_VARIANT_MATRIX3X3:
        case P_VARIANT_MATRIX4X4:
            PASSERT_NOTIMPLEMENTED();
            break;
        default:
            PASSERT_NOTREACHABLE("Unknown variant type.");
            break;
    }

    return PVariant();
}

PString PVariant::toString() const
{
    PASSERT_NOTIMPLEMENTED();
    return PString();
}
