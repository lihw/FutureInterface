// pcontextproperties.cpp
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pcontextproperties.h"

PContextProperties::PContextProperties()
    : m_contextName("default")
    , m_archiveName("")
{
    m_windowWidth  = 450;
    m_windowHeight = 800;
    m_windowLayout = P_WINDOWLAYOUT_MIDDLE_CENTER;
    m_maxFps       = 30;

#if defined P_WIN32
    m_rgba[0] = 
    m_rgba[1] = 
    m_rgba[2] = 
    m_rgba[3] = 8;

    m_depth = 24;
    m_stencil = 8;

    m_multisamples = 1;
#endif
}

