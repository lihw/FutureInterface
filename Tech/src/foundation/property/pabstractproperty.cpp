// pabstractproperty.cpp
// The property base class 
//
// Copyright 2012 - 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#include "pabstractproperty.h"

#include <PFoundation/plog.h>
#include <PFoundation/passert.h>


// -------------------------------------------------------------- 
// class PAbstractProperty
// -------------------------------------------------------------- 
PAbstractProperty::PAbstractProperty(PPropertyTypeEnum type)
{
    m_type            = type;
    m_index           = 255;
}
    
PAbstractProperty::~PAbstractProperty()
{
}

puint32 PAbstractProperty::index() const
{
    return m_index;
}
    
// -------------------------------------------------------------- 
// class PBasicProperty
// -------------------------------------------------------------- 
PBasicProperty::PBasicProperty(PPropertyTypeEnum type, PAbstractProperty *parent)
    : PAbstractProperty(type)
{
    m_parent = parent;
}

PBasicProperty::~PBasicProperty()
{
}

puint32 PBasicProperty::index() const
{
    if (m_parent != P_NULL)
    {
        return (((m_index + 1) << 8) | m_parent->index());
    }

    return m_index;
}


// -------------------------------------------------------------- 
// class PComboProperty
// -------------------------------------------------------------- 
PPropertyNameIndexMap PComboProperty::m_propertyNameIndexMap;

PComboProperty::PComboProperty(PPropertyTypeEnum type)
    : PAbstractProperty(type)
{
    m_propertyNameIndexMap.initialize(P_NULL);
}
    

PComboProperty::~PComboProperty()
{
    m_propertyNameIndexMap.release();
}
    
PBasicProperty *PComboProperty::subproperty(puint32 index) const
{
    if (index < m_properties.count())
    {
        return m_properties[index];
    }
    return P_NULL;
}

PBasicProperty *PComboProperty::subproperty(const pchar *name) const
{
    puint32 index = propertyNameIndexMap()->propertyIndex(name);
    if (index != 0xffffffff)
    {
        return m_properties[index];
    }
    return P_NULL;
}

void PComboProperty::addSubproperty(PPropertyNameIndexMap *nameIndexMap, const pchar *name, 
        PBasicProperty *property)
{
    puint32 index = m_properties.count();

    property->setIndex(index);
    property->setName(name);
    m_properties.append(property);

    if (index >= nameIndexMap->numberOfPropertyNames())
    {
        // Add property name into the record only when it is new to the record.
        nameIndexMap->addPropertyName(name, index);
    }
}

void PComboProperty::removeSubproperty(PPropertyNameIndexMap *nameIndexMap, const pchar *name)
{
    puint32 index = nameIndexMap->propertyIndex(name);
    if (index != 0xffffffff)
    {
        // Reset the index of the properties which are after the removed one.
        // And delete the removing one from the property database.
        puint32 propertyCount = m_properties.count();
        for (puint32 i = index + 1; i < propertyCount; i++)
        {
            m_properties[i - 1] = m_properties[i];
            m_properties[i - 1]->setIndex(i - 1);
        }
        m_properties.pop_back();

        // Remove the name.
        nameIndexMap->removePropertyName(name);
    }
}

PPropertyNameIndexMap *PComboProperty::propertyNameIndexMap() const
{
    return &m_propertyNameIndexMap;
}



