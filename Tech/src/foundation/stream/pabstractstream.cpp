// pabstractstream.cpp
// The interface of stream.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
// 
// Hongwei Li lihw81@gmail.com
// 

#include "pabstractstream.h"

#include "pstreamobject_private.h"
    
PAbstractStream::PAbstractStream()
{
    m_object = P_NULL;
}

PAbstractStream::~PAbstractStream()
{
    if (m_object != P_NULL)
    {
        PDELETE(m_object);
    }
}

void PAbstractStream::setEndianness(PStreamEndianessEnum endianness)
{
    if (endianness != P_STREAM_ENDIANNESS_UNKNOWN)
    {
        m_endianness = endianness;
    }
    else
    {
        PLOG_DEBUG("the current endianness is specified");
    }
}
