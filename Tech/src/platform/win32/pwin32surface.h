// pwin32surface.h
// Create OpenGL context using EGL under win32
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#ifndef PWIN32SURFACE_H
#define PWIN32SURFACE_H

#include <PFoundation/pglobal.h>

#include <EGL/egl.h>

struct PContextProperties;
class PWin32Window;

class PWin32Surface 
{
public:
    PWin32Surface(HWND hWnd, HDC hDC);
    virtual ~PWin32Surface();

    pbool create(PContextProperties *properties);
    void destroy();
    pbool setActive(pbool enabled);
    void update();

private:
    HDC        m_hDC;               // Device context.
    HWND       m_hWnd;              // The window handle.
    EGLDisplay m_eglDisplay;        // EGL display
    EGLSurface m_eglSurface;        // EGL surface.
    EGLContext m_eglContext;        // EGL context
    EGLConfig  m_eglConfiguration;  // EGL configuration.
};

#endif // !PWIN32SURFACEEGL_H


