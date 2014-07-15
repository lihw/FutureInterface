// pcontextproperties.h
// The properties of a context.
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PCONTEXTPROPERTIES_H
#define PCONTEXTPROPERTIES_H

#include <PFoundation/pstring.h>

enum PWindowLayoutModeEnum
{
    P_WINDOWLAYOUT_LEFT_BOTTOM,
    P_WINDOWLAYOUT_MIDDLE_CENTER,
    P_WINDOWLAYOUT_RIGHT_TOP,

    P_WINDOWLAYOUT_DEFAULT = P_WINDOWLAYOUT_MIDDLE_CENTER,

    P_WINDOWLAYOUT_FIRST = P_WINDOWLAYOUT_LEFT_BOTTOM,
    P_WINDOWLAYOUT_LAST = P_WINDOWLAYOUT_RIGHT_TOP,
    P_WINDOWLAYOUT_MODE_NUMBER = P_WINDOWLAYOUT_LAST - P_WINDOWLAYOUT_FIRST + 1,
};

struct PContextProperties 
{
    PContextProperties();

    PString                m_archiveName;  // The archive name.
    PString                m_contextName; 
    puint32                m_windowWidth;  // The default width value is 960.
    puint32                m_windowHeight; // The default height value is 540.
    PWindowLayoutModeEnum  m_windowLayout;
    puint32                m_maxFps;       // The maximum FPS. It is 30 fps by default. To disable FPS control, set it to 0.
#if defined P_WIN32
    puint32                m_rgba[4];      // Window color buffer bits
    puint32                m_stencil;      // Window framebuffer bits
    puint32                m_depth;        // Window depth buffer bits
    puint32                m_multisamples; 
#endif // P_WIN32
};


#endif // !PCONTEXTPROPERTIES_H
