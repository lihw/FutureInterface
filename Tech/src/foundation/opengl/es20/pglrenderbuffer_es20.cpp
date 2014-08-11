// pglrenderbuffer_es20.cpp
// The OpenGL render buffer
//
// Copyright 2012 - 2014 Future Interface . 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "../pglrenderbuffer.h"

#include <PFoundation/pglerror.h>
#include <PFoundation/plog.h>
#include <PFoundation/passert.h>
#include <PFoundation/pnew.h>

#if defined P_IOS
#  include <OpenGLES/ES2/gl.h>
#  include <OpenGLES/ES2/glext.h>
#else
#  include <GLES2/gl2.h>
#  include <GLES2/gl2ext.h>
#endif


PGlRenderbuffer::PGlRenderbuffer()
{
    m_renderbuffer = 0;
}

PGlRenderbuffer::~PGlRenderbuffer()
{
    if (m_renderbuffer != 0)
    {
        glDeleteRenderbuffers(1, &m_renderbuffer);
    }
}

pbool PGlRenderbuffer::create(puint32 width, puint32 height, PGlTextureFormatEnum format)
{
    PASSERT(format >= P_GLTEXTURE_FORMAT_DEPTH_COMPONENT16 &&
            format <= P_GLTEXTURE_FORMAT_STENCIL_INDEX8);

    PASSERT(m_renderbuffer == 0);
    if (m_renderbuffer != 0)
    {
        PLOG_WARNINGX(P_LOG_CHANNEL_OPENGLEGL, "Renderbuffer is already created");
        return false;
    }

    GLenum internalFormat;

    switch (format)
    {
       case P_GLTEXTURE_FORMAT_DEPTH_COMPONENT16:
           internalFormat = GL_DEPTH_COMPONENT16;
           break;
       case P_GLTEXTURE_FORMAT_STENCIL_INDEX8:
           internalFormat = GL_STENCIL_INDEX8;
           break;
        default:
            PASSERT(!"Unsupported render buffer format");
            PLOG_ERRORX(P_LOG_CHANNEL_OPENGLEGL, "Unsupported render buffer format");
            break;
    }
    
    glGenRenderbuffers(1, &m_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    pGlErrorCheckAbort();

    return true;
}



