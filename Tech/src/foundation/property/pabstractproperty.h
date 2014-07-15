// pabstractproperty.h
// The property base class 
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#ifndef P_ABSTRACTPROPERTY_H
#define P_ABSTRACTPROPERTY_H

#include <PFoundation/ppropertynameindexmap.h>

#include <PFoundation/parray.h>
#include <PFoundation/pstring.h>

class PVariant;

enum PPropertyTypeEnum
{
    P_PROPERTY_BOOL,
    P_PROPERTY_COLOR,
    P_PROPERTY_ENUM,
    P_PROPERTY_FLOAT,
    P_PROPERTY_INT,
    P_PROPERTY_MATRIX3X3,
    P_PROPERTY_MATRIX4X4,
    P_PROPERTY_TRANSFORM,
    P_PROPERTY_PROJECTION,
    P_PROPERTY_STRING,
    P_PROPERTY_TEXTURE,
    P_PROPERTY_UINT,
    P_PROPERTY_VECTOR2,
    P_PROPERTY_VECTOR3,
    P_PROPERTY_VECTOR4,
    
    P_PROPERTY_UNKNOWN,

    P_PROPERTY_FIRST = P_PROPERTY_BOOL,
    P_PROPERTY_LAST = P_PROPERTY_VECTOR4,
    P_PROPERTY_NUMBER = P_PROPERTY_LAST - P_PROPERTY_FIRST + 1,
};


// PAbstractProperty is base class of PBasicProperty and PComboProperty.
// However, to extend to new properties, users should either inherit
// PBasicProperty or PComplexProperty rather than this one. 
class P_DLLEXPORT PAbstractProperty
{
    friend class PObject;
    friend class PBasicProperty;
    friend class PComboProperty;

private:
    PAbstractProperty(const PAbstractProperty &other) {}
    void operator=(const PAbstractProperty &other) {}

public: 
    PAbstractProperty(PPropertyTypeEnum type);
    virtual ~PAbstractProperty();

    P_INLINE PPropertyTypeEnum type() const { return m_type; }
    P_INLINE const pchar *name() const { return m_name; }

    virtual void setValue(const PAbstractProperty &property) = 0;
    virtual void setValue(const PVariant &value) = 0;
    virtual void setValue(const void *value) = 0;
    virtual void reset() = 0;

    virtual pbool unpack(const pchar *value) = 0;

    virtual void interpolate(pfloat32 t, PAbstractProperty *a, PAbstractProperty *b) = 0;

    virtual puint32 index() const;

private:
    P_INLINE void setIndex(puint32 index) { m_index = index; }
    P_INLINE void setName(const pchar *name) { m_name = const_cast<pchar*>(name); }

protected:
    PPropertyTypeEnum   m_type;
    puint32             m_index;
    pchar              *m_name;
};

// The base class of basic properties.
class P_DLLEXPORT PBasicProperty : public PAbstractProperty
{
private:
    PBasicProperty(const PBasicProperty &other) : PAbstractProperty(P_PROPERTY_BOOL) {}
    void operator=(const PBasicProperty &other) {}

public: 
    PBasicProperty(PPropertyTypeEnum type, PAbstractProperty *parent = P_NULL);
    virtual ~PBasicProperty();
    
    virtual puint32 index() const;
    
protected:
    PAbstractProperty  *m_parent;
};


// Combo properties are composed of basic properties 
class P_DLLEXPORT PComboProperty : public PAbstractProperty
{
    PComboProperty(const PComboProperty &other) : PAbstractProperty(P_PROPERTY_BOOL) {}
    void operator=(const PComboProperty &other) {}

public:
    PComboProperty(PPropertyTypeEnum type);
    virtual ~PComboProperty();
    
    P_INLINE puint32 numberOfSubproperties() const { return m_properties.count(); }
    PBasicProperty *subproperty(puint32 index) const;
    PBasicProperty *subproperty(const pchar *name) const;

protected:
    void addSubproperty(PPropertyNameIndexMap *nameIndexMap, const pchar *name, PBasicProperty *property);
    void removeSubproperty(PPropertyNameIndexMap *nameIndexMap, const pchar *name);
    virtual PPropertyNameIndexMap *propertyNameIndexMap() const;

protected:
    PArray<PBasicProperty*> m_properties; // subproperties.

private:
    static PPropertyNameIndexMap m_propertyNameIndexMap;
};

#endif // !P_ABSTRACTPROPERTY_H

