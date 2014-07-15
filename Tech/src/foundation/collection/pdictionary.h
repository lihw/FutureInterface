// pdictionary.h
// A sorted array in the alphabetic order 
//
// Copyright 2012 -2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PDICTIONARY_H
#define PDICTIONARY_H

#include <PFoundation/parray.h>

template<typename T>
class PDictionary : public PArray<T>
{
public:
    PDictionary();
    PDictionary(const PDictionary<T>& o);

    void insert(const T &item);

    T& operator[](const pchar *name);
    const T &operator[](const pchar *name) const;

private:
    puint32 find(const pchar *name) const;
};

template<typename T>
PDictionary<T>::PDictionary()
    : PArray<T>::PArray()
{
}

template<typename T>
PDictionary<T>::PDictionary(const PDictionary &other)
    : PArray<T>::PArray(other)
{
}
    
template<typename T>
void PDictionary<T>::insert(const T &item)
{
    PASSERT_NOTIMPLEMENTED();
}

template<typename T>
T& PDictionary<T>::operator[](const pchar *name)
{
    PASSERT_NOTIMPLEMENTED();
}

template<typename T>
const T &PDictionary<T>::operator[](const pchar *name) const
{
    PASSERT_NOTIMPLEMENTED();
}

template<typename T>
puint32 PDictionary<T>::find(const pchar *name) const
{
    PASSERT_NOTIMPLEMENTED();
    return 0;
}

#endif // !PDICTIONARY_H
