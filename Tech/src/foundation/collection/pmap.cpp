// pmap.cpp
// A set of key value pairs
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pmap.h"

#if defined P_DEBUG

static void pMapTestCompiling()
{
	PMap<pint32, pint32> map;
	map.insert(1, 1);
	pint32 ret = map[1];

	PMap<pint32, pint32>::iterator it, ib = map.begin(), ie = map.end();
	for (it = ib; it != ie; ++it)
	{
		pint32 value = it.value();
	}
}

#endif // !P_DEBUG
