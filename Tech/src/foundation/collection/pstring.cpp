// pstring.h
// Ascii string in C++.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pstring.h"

#include <PFoundation/pcrt.h>

#include <ctype.h>

PString::PString(const pchar* o, pint32 l)
{
    if (o != P_NULL)
    {
        if (l < 0)
        {
            l = getLength(o);
        }

        m_l = l; 
        m_s = PNEWARRAY(pchar, m_l + 1);
        pmemcpy(m_s, o, m_l);
        m_s[m_l] = 0;
    }
    else
    {
        m_s = P_NULL;   
        m_l = -1;
    }
}

PString::PString(const PArray<pchar>& o)
{
    m_l = o.size();
    m_s = PNEWARRAY(pchar, m_l + 1);
    pmemcpy(m_s, &o[0], m_l);
    m_s[m_l] = 0;
}

PString::PString(const PString& o)
{
    m_l = o.m_l;
    m_s = PNEWARRAY(pchar, m_l + 1);
    pmemcpy(m_s, o.m_s, o.m_l);
    m_s[m_l] = 0;
}
    
PString::PString(puint32 l)
{
    m_l = l;
    m_s = PNEWARRAY(pchar, l + 1);
    pmemset(m_s, 0, m_l + 1);
}

PString::PString(pchar c, puint32 n)
{
    m_l = n;
    m_s = PNEWARRAY(pchar, m_l + 1);
    pmemset(m_s, *(puint8*)&c, m_l);
    m_s[m_l] = 0;
}

PString::~PString()
{
    PDELETEARRAY(m_s);
}

PString PString::left(puint32 n) const
{
    PASSERTINFO(n <= m_l, "not enough characters in the string");
    return PString(m_s, n);
}

PString PString::right(puint32 n) const
{
    pint32 position = pint32(m_l) - pint32(n);
    PASSERTINFO(position >= 0, "not enough characters in the string");
    return PString(m_s + position, n);
}

PString PString::mid(puint32 position, pint32 n) const
{
    PASSERTINFO(position + n <= m_l, "not enough characters in the string");
    return PString(m_s + position, n);
}

pint32 PString::find(pchar c) const
{
    for (puint32 i = 0; i < m_l; ++i)
    {
        if (m_s[i] == c)
        {
            return i;
        }
    }

    return -1;
}

pint32 PString::find(const PString& str) const
{
    PASSERT_NOTIMPLEMENTED();
    return -1;
}

pint32 PString::find(const char* str) const
{
    PASSERT_NOTIMPLEMENTED();
    return -1;
}

pint32 PString::rfind(pchar c) const
{
    for (pint32 i = (pint32)m_l; i >= 0; --i)
    {
        if (m_s[i] == c)
        {
            return i;
        }
    }

    return -1;
}

pint32 PString::rfind(const PString& str) const
{
    PASSERT_NOTIMPLEMENTED();
    return -1;
}

pint32 PString::rfind(const char* str) const
{
    PASSERT_NOTIMPLEMENTED();
    return -1;
}

void PString::append(const pchar* str, pint32 length)
{
    if (length < 0)
    {
        length = getLength(str);
    }

    pchar* newString = PNEWARRAY(pchar, length + m_l + 1);
    pmemcpy(newString, m_s, m_l);
    pmemcpy(newString + m_l, str, length);
    newString[length + m_l] = 0;

    PDELETEARRAY(m_s);
    m_s = newString;
    m_l += puint32(length);
}

void PString::append(const PString& str)
{
    pchar* newString = PNEWARRAY(pchar, str.m_l + m_l + 1);
    pmemcpy(newString, m_s, m_l);
    pmemcpy(newString + m_l, str.m_s, str.m_l);
    newString[str.m_l + m_l] = 0;

    PDELETEARRAY(m_s);
    m_s = newString;
    m_l += str.m_l;
}

void PString::prepend(const pchar* str, pint32 length)
{
    if (length < 0)
    {
        length = getLength(str);
    }

    pchar* newString = PNEWARRAY(pchar, length + m_l + 1);
    pmemcpy(newString, str, length);
    pmemcpy(newString + length, m_s, m_l);
    newString[length + m_l] = 0;

    PDELETEARRAY(m_s);
    m_s = newString;
}

void PString::prepend(const PString& str) 
{
    pchar* newString = PNEWARRAY(pchar, str.m_l + m_l + 1);
    pmemcpy(newString, str.m_s, str.m_l);
    pmemcpy(newString + str.m_l, m_s, m_l);
    newString[str.m_l + m_l] = 0;

    PDELETEARRAY(m_s);
    m_s = newString;
}

void PString::combine(const pchar *string, ...)
{
    va_list ap;
    va_start(ap, string);
    do
    {
        string = va_arg(ap, const pchar *);
        append(string, -1);
    } while (string != P_NULL);
}

pbool PString::operator<(const PString& o) const
{
    if (!this->m_s && !o.m_s) 
    {
        return false;
    }
    else if (!this->m_s) 
    {
        return true;
    }
    else if (!o.m_s) 
    {
        return false;
    }

    return pstrcmp(this->m_s, o.m_s) < 0; 
}

pbool PString::operator>(const PString& o) const
{ 
    if (!this->m_s && !o.m_s) 
    {
        return false;
    }
    else if (!this->m_s) 
    {
        return false;
    }
    else if (!o.m_s) 
    {
        return true;
    }

    return pstrcmp(this->m_s, o.m_s) > 0; 
}

pbool PString::isEqual(const pchar* str, pint32 length) const
{
    puint32 l = pMin(m_l, *(puint32*)&length);

    if (length >= 0)
    {
        for (puint32 i = 0; i < l; ++i)
        {
            if (str[i] != m_s[i])
            {
                return false;
            }
        }
    }
    else
    {
        puint32 i;
        for (i = 0; i < m_l && str[i] != 0; ++i)
        {
            if (str[i] != m_s[i])
            {
                return false;
            }
        }
        
        // The input string has the different length
        // as this one.
        if (!(i == l && str[i] == 0))
        {
            return false;
        }
    }

    return true;
}
    
const PString& PString::sprintf(const pchar *format, ...)
{
    PASSERT_NOTIMPLEMENTED();
    return *this;
}

void PString::assign(const pchar* string, puint32 length)
{
    PDELETEARRAY(m_s);
    m_s = PNEWARRAY(pchar, length + 1);
    pmemcpy(m_s, string, length);
    m_s[length] = 0;
    m_l = pint32(length);
}

pint32 PString::getLength(const pchar* s)
{
    // FIXME: optimize this function.
    pint32 l = 0;
    const pchar* p = s;
    while (*p)
    {
        l++;
        p++;
    }

    return l;
}

puint32 PString::toUint() const
{
    const pchar* p = m_s;
    while (isdigit(*p) && *p != 0)
    {
        p++;
    }

    puint32 ret = 0;
    // It is a hex
    if (*p == '0' && *(p + 1) == 'x')
    {
        p += 2;
        while (*p != 0 && *p != ' ' && *p != '\t')
        {
            if ((*p >= 'a' && *p <= 'f'))
            { 
                ret += ret * 16 + (*p - 'a' + 10);
            }
            else if (*p >= 'A' && *p <= 'F')
            {
                ret += ret * 16 + (*p - 'a' + 10);
            }
            else if (*p >= '0' && *p <= '9')
            {
                ret += ret * 16 + (*p - '0');
            }
            else
            {
                break;
            }
        }

    }
    else
    {
        // It is an oct.
        pint32 x = patoi(m_s);
        if (x < 0)
        {
            ret = *((puint32*)&x);
        }
        else
        {
            ret = x;
        }
    }

    return ret;
}

pint32 PString::toInt() const
{
    return patoi(m_s);
}

pfloat32 PString::toFloat() const
{
    return patof(m_s);
}

PString PString::fromUint(puint32 value)
{
    pchar buffer[64];
    psprintf(buffer, 64, "%u", value);
    return PString(buffer);
}

PString PString::fromInt(pint32 value)
{
    pchar buffer[64];
    psprintf(buffer, 64, "%d", value);
    return PString(buffer);
}

PString PString::fromFloat(pfloat32 value)
{
    pchar buffer[64];
    psprintf(buffer, 64, "%f", value);
    return PString(buffer);
}
    
PString& PString::toLower()
{
    if (m_s == P_NULL)
    {
        return *this;
    }

    for (puint32 i = 0 ; i < m_l; ++i)
    {
        if (m_s[i] >= 'A' && m_s[i] <= 'Z')
        {
            m_s[i] = m_s[i] - 'A' + 'a';
        }
    }
    
    return *this;
}

PString& PString::toUpper()
{
    if (m_s == P_NULL)
    {
        return *this;
    }

    for (puint32 i = 0 ; i < m_l; ++i)
    {
        if (m_s[i] >= 'a' && m_s[i] <= 'z')
        {
            m_s[i] = m_s[i] - 'a' + 'A';
        }
    }

    return *this;
}

PString& PString::replace(const pchar old, const pchar replacement)
{
    if (m_s == P_NULL)
    {
        return *this;
    }

    for (puint32 i = 0 ; i < m_l; ++i)
    {
        if (m_s[i] == old)
        {
            m_s[i] = replacement;
        }
    }

    return *this;
}

