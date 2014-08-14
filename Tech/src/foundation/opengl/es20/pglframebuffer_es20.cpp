// pglframebuffer.cpp
// The framebuffer object.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
// 
// Hongwei Li lihw81@gmail.com and Hongwei Li lihw81@gmail.com
// 


#include "../pglframebuffer.h"

#include <PFoundation/pgltexture.h>
#include <PFoundation/pglrenderbuffer.h>
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

PGlFramebuffer::PGlFramebuffer()
{
    m_colorBuffer   = P_NULL;
    m_depthBuffer   = P_NULL;
    m_stencilBuffer = P_NULL;
    m_framebuffer   = 0;
}

PGlFramebuffer::~PGlFramebuffer()
{
    PDELETE(m_colorBuffer);
    PDELETE(m_depthBuffer);
    PDELETE(m_stencilBuffer);

    if (m_framebuffer != 0)
    {
        glDeleteFramebuffers(1, &m_framebuffer);
    }
}

pbool PGlFramebuffer::create(puint32 width, puint32 height, PGlTextureFormatEnum colorFormat, 
            PGlTextureFormatEnum depthFormat, PGlTextureFormatEnum stencilFormat)
{
    PASSERT(m_colorBuffer == P_NULL);
    if (m_colorBuffer != P_NULL)
    {
        PLOG_WARNINGX(P_LOG_CHANNEL_OPENGLEGL, "Framebuffer has a color buffer already");
        return false;
    }
    
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    
    // -------------------------------------------------------------- 
    // Color attachment
    // -------------------------------------------------------------- 
    glActiveTexture(GL_TEXTURE0);

    PASSERT(colorFormat < P_GLTEXTURE_FORMAT_UNKNOWN);
    m_colorBuffer = PNEW(PGlTexture);
    if (!m_colorBuffer->create((puint8 *)P_NULL, width, height, colorFormat, false))
    {
        PLOG_WARNINGX(P_LOG_CHANNEL_OPENGLEGL, "Failed to create color buffer texture");
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, m_colorBuffer->textureHandle());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        m_colorBuffer->textureHandle(), 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // -------------------------------------------------------------- 
    // Depth attachment
    // -------------------------------------------------------------- 
    if (depthFormat != P_GLTEXTURE_FORMAT_UNKNOWN)
    {
        m_depthBuffer = PNEW(PGlRenderbuffer);
        if (!m_depthBuffer->create(width, height, depthFormat))
        {
            PDELETE(m_colorBuffer);
            PDELETE(m_depthBuffer);
            PLOG_WARNINGX(P_LOG_CHANNEL_OPENGLEGL, "Failed to create depth render buffer");
            return false;
        }
        // Bind depth buffer.
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer->getRenderbufferHandle());
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                m_depthBuffer->getRenderbufferHandle());
    }
    
    // -------------------------------------------------------------- 
    // Stencil attachment
    // -------------------------------------------------------------- 
    if (stencilFormat != P_GLTEXTURE_FORMAT_UNKNOWN)
    {
        m_stencilBuffer = PNEW(PGlRenderbuffer);
        if (!m_stencilBuffer->create(width, height, stencilFormat))
        {
            PDELETE(m_colorBuffer);
            PDELETE(m_depthBuffer);
            PDELETE(m_stencilBuffer);
            PLOG_WARNINGX(P_LOG_CHANNEL_OPENGLEGL, "Failed to create stencil render buffer");
            return false;
        }

        // Bind stencil buffer.
        glBindRenderbuffer(GL_RENDERBUFFER, m_stencilBuffer->getRenderbufferHandle());
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                m_stencilBuffer->getRenderbufferHandle());
    }
    
    if (!checkFramebufferStatus())
    {
        PDELETE(m_colorBuffer);
        PDELETE(m_depthBuffer);
        PDELETE(m_stencilBuffer);
    
        return false;
    }

    if (!pGlErrorCheckError())
    {
        PDELETE(m_colorBuffer);
        PDELETE(m_depthBuffer);
        PDELETE(m_stencilBuffer);

        return false;
    }
    
    return true;
}

pbool PGlFramebuffer::checkFramebufferStatus()
{
    puint32 result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    pbool ret = true;

    if (result != GL_FRAMEBUFFER_COMPLETE)
    {
        glDeleteFramebuffers(1, &m_framebuffer);

        if (result == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
        {
            PLOG_ERRORX(P_LOG_CHANNEL_OPENGLEGL, "Frame buffer couldn't be constructed: incomplete attachment");
        }
        if (result == GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS)
        {
            PLOG_ERRORX(P_LOG_CHANNEL_OPENGLEGL, "Frame buffer couldn't be constructed: incomplete dimensions");
        }    
        if (result == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
        {
            PLOG_ERRORX(P_LOG_CHANNEL_OPENGLEGL, "Frame buffer couldn't be constructed: missing attachment");
        }
        
        ret = false;
    }

    return ret;
}

void PGlFramebuffer::enable()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    pGlErrorCheckError();
}

void PGlFramebuffer::clearFramebuffer(pbool color, pbool depth, pbool stencil)
{
    GLbitfield bits = 0;
    if (color)
    {
        bits |= GL_COLOR_BUFFER_BIT;
    }
    if (depth)
    {
        bits |= GL_DEPTH_BUFFER_BIT;
    }
    if (stencil)
    {
        bits |= GL_STENCIL_BUFFER_BIT;
    }

    glClear(bits);
}
    
PGlFramebuffer *PGlFramebuffer::createDefaultFramebuffer()
{
    GLint fbo;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
    
    PGlFramebuffer *framebuffer = PNEW(PGlFramebuffer);
    framebuffer->m_framebuffer = fbo;
    // FIXME: we don't fill the color, depth and stencil buffer although they may exist.
    pGlErrorCheckError();
    return framebuffer;
}
