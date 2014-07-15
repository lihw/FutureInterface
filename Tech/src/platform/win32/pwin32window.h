// p_win32_window.h
// The native window under win32
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#ifndef PWIN32WINDOW_H
#define PWIN32WINDOW_H

#include <PFoundation/pinput.h>

#include <windows.h>


class PWin32Surface;
class PContext;


class P_DLLEXPORT PWin32Window 
{
public:
    PWin32Window(PContext* context);
    virtual ~PWin32Window();

    P_INLINE HWND getHWND() const
    { PASSERT(m_hWnd != P_NULL); return m_hWnd; }
    P_INLINE HDC getHDC() const
    { PASSERT(m_hDC != P_NULL); return m_hDC; }

    virtual pbool create();
    virtual void run();
    virtual void destroy();
    
    P_INLINE PContext* getContext() const
    { return m_context; }

    void mousePressEvent(pint32 x, pint32 y, pbool isCtrlHeld);
    void mouseMoveEvent(pint32 x, pint32 y, pbool isCtrlHeld);
    void mouseReleaseEvent(pint32, pint32 y, pbool isCtrlHeld);
    void keyPressEvent(puint32 nativeScanCode, PInputKey key, puint32 state);
    void keyReleaseEvent(puint32 nativeScanCode, PInputKey key, puint32 state);
    void resizeEvent(pint32 width, pint32 height);
    pbool closeEvent();
    void focusLostEvent();
    void focusGainedEvent();
    void pause();
    void resume();

private:
    pbool createWindow(puint32 windowWidth, puint32 windowHeight);
    void destroyWindow();

private:
    HDC                m_hDC;
    HWND               m_hWnd;   
    DWORD              m_style; 
    PWin32Surface*     m_surface;
    PContext*          m_context;
};

#endif // PWIN32WINDOW_H
