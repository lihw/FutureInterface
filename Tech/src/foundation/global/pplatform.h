// pplatform.h
// All definitions which is platform dependent.
// 
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
// 
// Hongwei Li lihw81@gmail.com
// 

#ifndef PPLATFORM_H
#define PPLATFORM_H

//
// Platform recognition.
//
// FIXME: move this macro to building config (CMake)
#if defined _MSC_VER || defined _WIN32
# define P_WIN32
# define P_MSC
#elif defined __APPLE__
# define P_IOS
# define P_LLVM
#elif defined 
# define __ANDROID__
# define P_GCC
#else
# error "Unsupported platform"
#endif

//
// Compiling paragma
//
#if defined P_WIN32

# define P_EXTERN    extern                ///< Function is Externally defined
# define P_INLINE    __forceinline         ///< Function is inlined (compiler decision)
# define P_NAKED     __declspec(naked)     ///< Function is naked
# define P_APIENTRY  __stdcall             ///< Calling convention for function exposed in the API
# define P_CCONV     __cdecl               ///< Calling convention for C functions

#elif defined P_ANDROID || defined P_IOS

# define P_EXTERN    extern                ///< Function is Externally defined
# define P_INLINE    inline                ///< Function is inlined (compiler decision)
# define P_NAKED                           ///< Function is naked
# define P_APIENTRY                        ///< Calling convention for function exposed in the API
# define P_CCONV                           ///< Calling convention for C functions

#else // Other platforms

#error "Unsupported platform"

#endif // P_WIN32

#endif  // !PPLATFORM_H
