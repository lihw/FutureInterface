// pnew.h
// When memory debugging is enabled, we override global new and delete
// operator. Note that this memory debuggin is only for engine development,
// we should not release it to application dev.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PNEW_H
#define PNEW_H

#include <PFoundation/pglobal.h>

#if defined P_ANDROID || defined P_IOS
# include <stddef.h> // for size_t
#endif

#if P_ENABLE_MEMORY_DEBUGGING == 1

#define PNEW(a)          (new(__FILE__, __LINE__) a)
#define PNEWARRAY(a, n)  (new(__FILE__, __LINE__) a [(n)])
#define PDELETE(p)       do { delete p; p = P_NULL; } while (0);
#define PDELETETHIS(p)   do { delete p; } while(0);
#define PDELETEARRAY(p)  do { delete [] p; p = P_NULL; } while (0);

// TODO: add exception handling.
void * operator new(size_t size, const pchar *file, const pint32 line);
void operator delete(void *ptr);
void operator delete(void *ptr, const pchar *file, const pint32 line);
void * operator new[](size_t size, const pchar *file, const pint32 line);
void operator delete[](void *ptr);
void operator delete[](void *ptr, const pchar *file, const pint32 line);

#else /// P_ENABLE_MEMORY_DEBUGGING == 0

#define PNEW(a)          (new a)
#define PNEWARRAY(a, n)  (new a [(n)])
#define PDELETE(p)       do { delete p; p = P_NULL; } while (0);
#define PDELETETHIS(p)   do { delete p; } while(0);
#define PDELETEARRAY(p)  do { delete [] p; p = P_NULL; } while (0);

#endif // P_ENABLE_MEMORY_DEBUGGING == 0

// FIXME: Introduce the STL <new> for placement new and delete. Should
// not expose this system-dependent header in engine's header file. 
#if defined P_USE_STL
# include <new>
#else 
P_INLINE void* P_CCONV operator new (size_t size, void* ptr) throw() 
{
    return ptr;
}

P_INLINE void* P_CCONV operator new[] (size_t size, void* ptr) throw()
{
    return ptr;
}

P_INLINE void P_CCONV operator delete(void* ptr, void* voidptr2) throw()
{
}

P_INLINE void P_CCONV operator delete[] (void* ptr, void* voidptr2) throw()
{
}
#endif // P_USE_STL


#endif // !PNEW_H
