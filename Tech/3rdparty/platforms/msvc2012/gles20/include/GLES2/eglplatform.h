/**************************************************************************
 * Name         : eglplatform.h
 *
 * Copyright    : 2008 by Imagination Technologies Limited. All rights reserved.
 *              : No part of this software, either material or conceptual
 *              : may be copied or distributed, transmitted, transcribed,
 *              : stored in a retrieval system or translated into any
 *              : human or computer language in any form by any means,
 *              : electronic, mechanical, manual or other-wise, or
 *              : disclosed to third parties without the express written
 *              : permission of Imagination Technologies Limited, Unit 8, HomePark
 *              : Industrial Estate, King's Langley, Hertfordshire,
 *              : WD4 8LZ, U.K.
 *
 * Platform     : ANSI
 *
 * $Date: 2008/12/02 11:51:08 $ $Revision: 1.12 $
 **************************************************************************/
#ifndef __eglplatform_h_
#define __eglplatform_h_

#   ifndef int32_t
        typedef int int32_t;
#   endif

#if defined (__SYMBIAN32__) /* Symbian GCC */
#   if defined (__GCC32__)
#       define EGLAPI __declspec(dllexport)
#   else
#       define EGLAPI IMPORT_C
#   endif
#elif defined(_WIN32) || defined(__VC32__)             /* Win32 */
#   if defined (_DLL_EXPORTS)
#       define EGLAPI __declspec(dllexport)
#   else
#       define EGLAPI __declspec(dllimport)
#   endif
#elif defined (__ARMCC_VERSION)                      /* ADS */
#   define EGLAPI
#elif defined (__GNUC__)                             /* GCC dependencies (kludge) */
#   define EGLAPI
#elif defined (_UITRON_)
#	define EGLAPI
#endif

#if !defined (EGLAPI)
#   error Unsupported platform!
#endif

#ifndef EGLAPIENTRY
#	if defined(_WIN32) && !defined(UNDER_CE)
#		define EGLAPIENTRY APIENTRY
#	else
#		define EGLAPIENTRY
#	endif
#endif

#if defined __linux__
#	include <sys/types.h>
#	if defined(SUPPORT_X11)
#		include <X11/Xlib.h>
		typedef Display*	NativeDisplayType;
		typedef Window		NativeWindowType;
		typedef Pixmap		NativePixmapType;
#	else
		typedef int		NativeDisplayType;
		typedef void*	NativeWindowType;
		typedef void*	NativePixmapType;
#	endif
#elif defined(__SYMBIAN32__)
#   undef TRUE  /* e32def.h defines this */
#   undef FALSE /* e32def.h defines this */
#	include <e32def.h>
#   ifndef int32_t
        typedef int int32_t;
#   endif

	typedef TInt 	NativeDisplayType;
	/*
		Declare these as void although they points to classes - we can't
		include	a C++ header file as the EGL files are all written in C.
	*/
	typedef void*	NativeWindowType; /* Is really an RWindow* */
	typedef void*	NativePixmapType; /* Is really a CFbsBitmap* */
#elif defined(UNDER_CE) || defined(_WIN32)
	typedef int int32_t;
#	undef UNREFERENCED_PARAMETER
#	include <windows.h>
	typedef HDC		NativeDisplayType;
	typedef HWND	NativeWindowType;
	typedef void*	NativePixmapType;
#elif defined(_UITRON_)
	typedef int int32_t;
	typedef int	NativeDisplayType;
	typedef void*	NativeWindowType;
	typedef void*	NativePixmapType;
#else
	typedef int	NativeDisplayType;
	typedef void*	NativeWindowType;
	typedef void*	NativePixmapType;
#endif

typedef NativeDisplayType EGLNativeDisplayType;
typedef NativePixmapType EGLNativePixmapType;
typedef NativeWindowType EGLNativeWindowType;

#endif /* __eglplatform_h */
