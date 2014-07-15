// popertaor.h
// The operator functional object
//
// Copyright 2012 -2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef POPERATOR_H
#define POPERATOR_H

#include <PFoundation/pglobal.h>

template<typename T>
class PLess
{
public:
    bool operator()(const T &_1, const T &_2) const
    {
        return _1 < _2; 
    }
};



#endif // !POPERATOR_H
