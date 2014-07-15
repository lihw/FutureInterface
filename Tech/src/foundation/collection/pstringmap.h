// pstringmap.h
// A map containing pairs of a const string and an entity
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PSTRINGMAP_H
#define PSTRINGMAP_H

#include <PFoundation/pmap.h>
#include <PFoundation/pcrt.h>

class PStringMapLess 
{
public:
    bool operator()(const pchar *a, const pchar *b) const
    { return pstrcmp(a, b) < 0; }
};

template<typename T>
class PStringMap : public PMap<pchar *, T, PStringMapLess>
{
public:
    PStringMap();
    ~PStringMap();
};

template<typename T>
PStringMap<T>::PStringMap()
{
}

template<typename T>
PStringMap<T>::~PStringMap()
{
}

#endif // !PSTRINGMAP_H
