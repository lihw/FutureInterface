// ppropertynameindexmap.cpp
// The property name to index mapping. Each PData derived class should contain one
// PPropertyNameIndexMap to manage its property names.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "ppropertynameindexmap.h"

#include <PFoundation/pnew.h>
#include <PFoundation/plog.h>
#include <PFoundation/passert.h>


PPropertyNameIndexMap::PPropertyNameIndexMap()
{
    m_ref = 0;
}

PPropertyNameIndexMap::~PPropertyNameIndexMap()
{
    PASSERT(m_nameIndexMap == P_NULL);
}

void PPropertyNameIndexMap::addPropertyName(const pchar *name, puint32 index)
{
    // Duplicate the name
    Iterator it = m_nameIndexMap->insert(const_cast<pchar*>(name), index);
    PASSERT(it != m_nameIndexMap->end());
    PASSERT(it.key() == name);
}
    
void PPropertyNameIndexMap::removePropertyName(const pchar *name)
{
    Iterator it = m_nameIndexMap->find(const_cast<pchar*>(name));
    PASSERTINFO(it != m_nameIndexMap->end(), "Failed to find property name");
    if (it != m_nameIndexMap->end())
    {
        m_nameIndexMap->erase(it);
    }
}

puint32 PPropertyNameIndexMap::propertyIndex(const pchar *name) const
{
    Iterator it = m_nameIndexMap->find(const_cast<pchar*>(name));
    if (it != m_nameIndexMap->end())
    {
        return it.value();
    }

    return 0xffffffff;
}

puint32 PPropertyNameIndexMap::numberOfPropertyNames() const
{
    if (m_nameIndexMap == P_NULL)
    {
        return 0;
    }

    return m_nameIndexMap->count();
}

void PPropertyNameIndexMap::initialize(const PPropertyNameIndexMap *base)
{
    if (m_ref == 0)
    {
        m_nameIndexMap = PNEW(Map);

        if (base != P_NULL)
        {
            // Copy the other properties into this one.
            Iterator it = base->m_nameIndexMap->begin(); 
            Iterator ie = base->m_nameIndexMap->end(); 
            while (it != ie)
            {
                addPropertyName(it.key(), it.value());
                ++it;
            }
        }
    }

    m_ref++;
}

void PPropertyNameIndexMap::release()
{
    PASSERT(m_ref != 0);

    --m_ref;

    if (m_ref == 0)
    {
        PDELETE(m_nameIndexMap);
    }
}

