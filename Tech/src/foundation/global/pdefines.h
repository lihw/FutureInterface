// pdefines.h
// The defines and common macros.
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PDEFINES_H
#define PDEFINES_H

#if defined _DEBUG || defined DEBUG 
# define P_DEBUG 1
#endif

#ifndef P_GLOBAL
# define P_GLOBAL // an marker to indicate the variable is globally visible.
#endif


//
// Helper marcos
//

// Usage:
// #define SOME_MARCO() P_MULTILINE_MARCO_BEGIN \
// ... \
// ... \
// P_MULTILINE_MARCO_END
#define P_MULTILINE_MACRO_BEGIN { 
#define P_MULTILINE_MACRO_END }

// Function are mangled as a c interface
#define P_EXTERNC_BEGIN extern "C"  {
#define P_EXTERNC_END }          

// Unused arguments.
#define P_UNUSED_ARGUMENT(p)  ((void)p)

// Count the number of arguments of a macro.
#define P_NUM_ARGS(...) P_NUM_ARGS_IMPL(__VA_ARGS__, 5, 4, 3, 2, 1)
#define P_NUM_ARGS_IMPL(_1, _2, _3, _4, _5, N, ...) N

// FIXME: overloading macro which will select either one argument or two arguments
// implementation based on the number of arguments. It is not working in msvc2012 because
// msvc2012 is not conforming to C99.
//#define FOO1(a) a()
//#define FOO2(a,b) a(b)
//#define P_GET_MACRO(_1, _2, NAME, ...) NAME
//#define P_ONE_OR_TWO_ARGUMENTS(...) P_GET_MACRO(__VA_ARGS__, FOO2, FOO1)(__VA_ARGS__)

//
// Global magic
//


// -------------------------------------------------------------- 
// Platform layer
// -------------------------------------------------------------- 


// --------------------------------------------------------------
// Foundation layer
// -------------------------------------------------------------- 


#endif // !PDEFINES_H

