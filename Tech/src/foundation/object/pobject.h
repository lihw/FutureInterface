// pobject.h
// The base class of all objects which needs properties and event handling
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//
// 

#ifndef POBJECT_H
#define POBJECT_H

#include <PFoundation/pentity.h>
#include <PFoundation/pabstractproperty.h>
#include <PFoundation/ppropertynameindexmap.h>

class PEvent;
class PXmlElement;


#define P_OBJECT \
    protected: \
        virtual PPropertyNameIndexMap* propertyNameIndexMap() const \
        { return &m_propertyNameIndexMap; } \
    private: \
        static PPropertyNameIndexMap m_propertyNameIndexMap;

#define P_OBJECT_DEFINE(c) \
    PPropertyNameIndexMap c::m_propertyNameIndexMap;

#define P_OBJECT_INITIALIZE_PROPERTY(parent) \
    m_propertyNameIndexMap.initialize(parent::propertyNameIndexMap());

#define P_OBJECT_UNINITIALIZE_PROPERTY() \
    m_propertyNameIndexMap.release();

#define P_OBJECT_REGISTER_PROPERTY(name, property) \
    registerProperty(&m_propertyNameIndexMap, name, property)

class PEventManager;

class P_DLLEXPORT PObject : public PEntity
{
    P_OBJECT

    PObject(const PObject &other) {}
    void operator=(const PObject &other) {}

public:
    PObject(); 
    virtual ~PObject();

    //
    // Properties
    //
    PAbstractProperty *property(const pchar *name) const;
    PAbstractProperty *property(puint32 index) const;
    PArray<pchar*> propertyNames() const; 

    P_INLINE puint32 numberOfProperties() const
    { return m_properties.size(); }
    P_INLINE pbool hasProperty(const pchar *name) const
    { return property(name) != P_NULL; }
    
    // Events
    // TODO:
    // void setEventHandler(PEventHandler *handler);
    virtual void onEvent(PEvent *event);
    
    //
    // Serialize (from/to string)
    //
    //virtual void pack(PXmlElement* textElement) const = 0;
    virtual pbool unpack(const PXmlElement* xmlElement) = 0;

protected:
    void registerProperty(PPropertyNameIndexMap* nameIndexMap, const pchar *name, PAbstractProperty *property);
    void unregisterProperty(PPropertyNameIndexMap *nameIndexMap, const pchar *name);

protected:
    PArray<PAbstractProperty *> m_properties;
};

        


#endif // !POBJECT_H
