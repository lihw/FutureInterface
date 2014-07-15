// ppropertynameindexmap.h
// The property name to index mapping. Each PData derived class should contain one
// PPropertyNameIndexMap to manage its property names.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PPROPERTYNAMEINDEXMAP_H
#define PPROPERTYNAMEINDEXMAP_H

#include <PFoundation/pmap.h>

class P_DLLEXPORT PPropertyNameIndexMap
{
private:
    // Disable the copy constructor and assignment operator.
    PPropertyNameIndexMap(const PPropertyNameIndexMap& other) {}
    void operator=(const PPropertyNameIndexMap& other) {}

public:
    PPropertyNameIndexMap();
    ~PPropertyNameIndexMap();

    void addPropertyName(const pchar *name, puint32 index);
    void removePropertyName(const pchar *name);

    puint32 propertyIndex(const pchar *name) const;
    
    puint32 numberOfPropertyNames() const;

    void initialize(const PPropertyNameIndexMap *base);
    void release();


private:
    class StringLess 
    {
    public:
        pbool operator()(const pchar *a, const pchar *b) const
        { return pstrcmp(a, b) < 0; }
    };
    typedef PMap<pchar*, puint32, StringLess> Map;
    typedef PMap<pchar*, puint32, StringLess>::iterator Iterator; 
    
    Map     *m_nameIndexMap;
    puint32  m_ref;   
};


#endif // !PPROPERTYNAMEINDEXMAP_H
