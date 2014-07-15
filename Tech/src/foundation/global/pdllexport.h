// pdllexport.h
// To manage the DLL export or import
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PDLLEXPORT_H
#define PDLLEXPORT_H

# include "pplatform.h"

/// Extern, dll, and calling convention definitions
#if defined P_SHARED_LIBRARY 

# if defined P_WIN32

#    define P_DLLEXPORT __declspec(dllexport) ///< Function is exported from DLL
#    //define P_DLLEXPORT __declspec(dllimport) ///< Function is imported from DLL
#    define P_DLLHIDE   

# else // other OSes

#    define P_DLLEXPORT __attribute__ ((visibility("default")))
#    define P_DLLHIDE   __attribute__ ((visibility("hidden")))

# endif // P_WIN32

#else

# define P_DLLEXPORT 
# define P_DLLHIDE   

#endif // P_SHARED_LIBRARY


#endif // !P_DLLEXPORT_H
