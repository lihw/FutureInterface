// pobject.cpp
// The base class of all objects which needs auto release.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com and Hongwei Li lihw81@gmail.com
//

#include "pobject.h"


#include <PFoundation/pxmlelement.h>
#include <PFoundation/pnew.h>
#include <PFoundation/passert.h>
#include <PFoundation/plog.h>

P_OBJECT_DEFINE(PObject)

 
PObject::PObject()
    : PEntity()
{
    m_propertyNameIndexMap.initialize(P_NULL);
}

PObject::~PObject()
{
    m_propertyNameIndexMap.release();
}
    
PAbstractProperty *PObject::property(const pchar *name) const
{
    const PPropertyNameIndexMap *nameIndexMap = propertyNameIndexMap();

    puint32 majorIndex = 0;

    const pchar *p = pstrchr(name, '/');
    if (p != P_NULL)
    {
        pchar majorName[1024];
        pstrncpy(majorName, name, p - name);
        majorIndex = nameIndexMap->propertyIndex(majorName);

        PASSERT(majorIndex < m_properties.count());
        PAbstractProperty *property  = m_properties[majorIndex];

#if defined P_DEBUG
        PComboProperty* complexProperty = dynamic_cast<PComboProperty*>(property);
        PASSERT(complexProperty != P_NULL);
#else
        PComboProperty* complexProperty = (PComboProperty*)property;
#endif
    
        return complexProperty->subproperty(p + 1);
    }
    else
    {
        puint32 index = nameIndexMap->propertyIndex(name);
        if (index == 0xffffffff)
        {
            PLOG_ERROR("Failed to find such  called %s", name);
            return P_NULL;
        }
        return m_properties[index];
    }

    PASSERT_NOTREACHABLE("Failed to find ");
    return P_NULL;
}

PAbstractProperty *PObject::property(puint32 index) const
{
    puint32 majorIndex = index & 0xff;
    puint32 minorIndex = ((index & 0xff00) >> 8);

    PASSERT(majorIndex < m_properties.count());
    PAbstractProperty *property = m_properties[majorIndex];
    
    if (minorIndex != 0)
    {
#if defined P_DEBUG
        PComboProperty* complexProperty = dynamic_cast<PComboProperty*>(property);
        PASSERT(complexProperty != P_NULL);
#else
        PComboProperty* complexProperty = (PComboProperty*)property;
#endif
        property = complexProperty->subproperty(minorIndex - 1);
    }

    return property;
}

PArray<pchar*> PObject::propertyNames() const
{
    PArray<pchar*> ret;

    puint32 count = m_properties.size();
    for (puint32 i = 0; i < count; ++i)
    {
        ret.append(const_cast<pchar*>(m_properties[i]->name()));
    }

    return ret;
}

pbool PObject::unpack(const PXmlElement* xmlElement)
{
    puint32 numProperties = m_properties.size();
    for (puint32 i = 0; i < numProperties; ++i)
    {
        m_properties[i]->reset();

        const char *name = m_properties[i]->name();
        const pchar *value = xmlElement->attribute(name);
        if (value != P_NULL && !m_properties[i]->unpack(value))
        {
            return false;
        }
    }

    return true;
}

void PObject::registerProperty(PPropertyNameIndexMap *nameIndexMap, const pchar *name, PAbstractProperty *property)
{
    puint32 index = m_properties.count();

    property->setIndex(index);
    property->setName(name);
    m_properties.append(property);

    if (index >= nameIndexMap->numberOfPropertyNames())
    {
        // Add  name into the record only when it is new to the record.
        nameIndexMap->addPropertyName(name, index);
    }
}

void PObject::unregisterProperty(PPropertyNameIndexMap* nameIndexMap, const pchar* name)
{
    puint32 index = nameIndexMap->propertyIndex(name);
    if (index != 0xffffffff)
    {
        // Reset the index of the propertys which are after the removed one.
        // And delete the removing one from the  database.
        puint32 count = m_properties.count();
        for (puint32 i = index + 1; i < count; ++i)
        {
            m_properties[i - 1] = m_properties[i];
            m_properties[i - 1]->setIndex(i - 1);
        }
        m_properties.pop_back();

        // Remove the name.
        nameIndexMap->removePropertyName(name);
    }
}

void PObject::onEvent(PEvent *event)
{
}
    
