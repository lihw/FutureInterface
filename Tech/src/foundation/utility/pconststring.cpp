// pconststring.cpp
// The C string manipulation
//
// Copyright 2012 - 2014 Future Interface . 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pconststring.h"

#include <PFoundation/passert.h>

pchar P_APIENTRY pCharToLower(pchar c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return c - 'A' + 'a';
    }

    return c;
}

pchar P_APIENTRY pCharToUpper(pchar c)
{
    if (c >= 'a' && c <= 'z')
    {
        return c - 'a' + 'A';
    }

    return c;
}

const pchar * P_APIENTRY pStringToLower(const pchar *in, puint32 bufferSize, pchar *out)
{
    PASSERT(in != P_NULL && out != P_NULL);

    const pchar *p = in;
    puint32 i = 0;
    while (*p != 0 && i < bufferSize - 1)
    {
        out[i] = pCharToLower(*p);
        p++;
        i++;
    }
    out[i] = 0;
    return out;
}

const pchar * P_APIENTRY pStringToUpper(const pchar *in, puint32 bufferSize, pchar *out)
{
    PASSERT(in != P_NULL && out != P_NULL);

    const pchar *p = in;
    puint32 i = 0;
    while (*p != 0 && i < bufferSize - 1)
    {
        out[i] = pCharToUpper(*p);
        p++;
        i++;
    }

    out[i] = 0;
    return out;
}

const pchar * P_APIENTRY pStringToTitle(const pchar *in, puint32 bufferSize, pchar *out)
{
    PASSERT(in != P_NULL && out != P_NULL);

    out[0] = pCharToUpper(in[0]);

    const pchar *p = &in[1];
    puint32 i = 1;
    while (*p != 0 && i < bufferSize - 1)
    {
        out[i] = pCharToLower(*p);
        p++;
        i++;
    }

    out[i] = 0;
    return out;
}

const pchar * P_APIENTRY pStringTrim(const pchar *in)
{
    const pchar *ret = in;
    while (*ret != 0)
    {
        if (*ret != ' ' && *ret != '\t')
        {
            break;
        }

        ++ret;
    }

    return ret;
}

const pchar * P_APIENTRY pStringUnpackFloat(const pchar *in, pfloat32 *out)
{
    *out = 0;

    in = pStringTrim(in);

    if (*in != '.' && *in != '-' && !(*in >= '0' && *in <= '9'))
    {
        PASSERTINFO(P_NULL, "Not a valid float number");
        return P_NULL;
    }

    pfloat32 sign = 1.0f;
    if (*in == '-')
    {
        sign = -1.0f;
        ++in;
    }

    pfloat32 number = 0;
    pbool hasFraction = false;
    puint32 fraction = 0;
    
    while (*in != 0 && *in != ',')
    {
        if (*in == '.')
        {
            if (hasFraction)
            {
                PASSERTINFO(P_NULL, "Not a valid float number");
                return P_NULL;
            }
            hasFraction = true;
        }
        else if (*in >= '0' && *in <= '9')
        {
            pfloat32 digit = pfloat32(*in - '0');
            number = number * 10.0f + digit;
            if (hasFraction)
            {
                fraction++;
            }
        }
        else
        {
            PASSERTINFO(P_NULL, "Not a valid float number");
            return P_NULL;
        }
        ++in;
    }

    if (hasFraction && fraction >= 1)
    {
        pfloat32 power = 1.0f;
        while (fraction > 0)
        {
            power *= 10.0f;
            fraction--;
        }
        *out = sign * number / power;
    }
    else
    {
        *out = sign * number;
    }

    if (*in != 0)
    {
        return in + 1;
    }

    return in;
}

const pchar * P_APIENTRY pStringUnpackInt(const pchar *in, pint32 *out)
{
    *out = 0;

    in = pStringTrim(in);

    if (*in != '-' && !(*in >= '0' && *in <= '9'))
    {
        PASSERTINFO(P_NULL, "Not a valid int number");
        return P_NULL;
    }
    
    pint32 sign = 1;
    if (*in == '-')
    {
        sign = -1;
        ++in;
    }

    pint32 number = 0;
    
    while (*in != 0 && *in != ',')
    {
        if (*in >= '0' && *in <= '9')
        {
            pint32 digit = *in - '0';
            number = number * 10 + digit;
        }
        else
        {
            PASSERTINFO(P_NULL, "Not a valid int number");
            return P_NULL;
        }
        ++in;
    }
    
    *out = sign * number;

    if (*in != 0)
    {
        return in + 1;
    }

    return in;
}

const pchar * P_APIENTRY pStringUnpackString(const pchar *in, pchar *out)
{
    in = pStringTrim(in);

    while (*in != 0 && *in != ',')
    {
        *out++ = *in++;
    }
    *out = 0;
    
    if (*in != 0)
    {
        return in + 1;
    }

    return in;
}

const pchar * P_APIENTRY pStringUnpackTrimAnnotation(const pchar *in)
{
    while (*in != 0 && *in != ':')
    {
        in++;
    }

    if (*in != 0)
    {
        return in + 1;
    }

    return in;
}
