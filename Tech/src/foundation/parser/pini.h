// pini.h 
// The ini parser wrapper
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PINI_H
#define PINI_H

#include <PFoundation/pstring.h>
#include <PFoundation/pmap.h>


class P_DLLEXPORT PIni
{
public:
    PIni(const pchar *filename);
    ~PIni();

    const PString &entry(const PString &name) const;
    const PString &entry(const pchar *name) const;
    void addEntry(const pchar *name, const pchar *value);
    puint32 numberOfEntries() const { return m_entries.size(); }
    
private:
    pbool load(const pchar *filename);
    // TODO: replace with PDictionary
    PMap<PString, PString> m_entries;
    // An null string
    PString m_nullString;
};



#endif // !PINI_H
