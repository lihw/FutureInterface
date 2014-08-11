// pglstate_es20.cpp
// The OpenGL render state
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "../pglstate.h"

#include <PFoundation/plog.h>
#include <PFoundation/passert.h>
#include <PFoundation/pglerror.h>

#if defined P_IOS
#  include <OpenGLES/ES2/gl.h>
#  include <OpenGLES/ES2/glext.h>
#else
#  include <GLES2/gl2.h>
#  include <GLES2/gl2ext.h>
#endif


PGlState::PGlState(const puint32 *initialViewport)
{
    m_initialViewport[0] = initialViewport[0];
    m_initialViewport[1] = initialViewport[1];
    m_initialViewport[2] = initialViewport[2];
    m_initialViewport[3] = initialViewport[3];

    reset();
    
    const GLubyte* vendor = glGetString(GL_VENDOR);
    PLOG_INFO("Running on %s display card", (pchar*)(vendor));
}

PGlState::~PGlState()
{
}

void PGlState::reset()
{
    // Reset the OpenGL states.

    // -------------------------------------------------------------- 
    // viewport
    // -------------------------------------------------------------- 
    m_viewport[0] = m_initialViewport[0]; 
    m_viewport[1] = m_initialViewport[1]; 
    m_viewport[2] = m_initialViewport[2]; 
    m_viewport[3] = m_initialViewport[3]; 

    glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
    
    // -------------------------------------------------------------- 
    // Color mask
    // -------------------------------------------------------------- 
    m_colorWriteEnabled[0] = true;
    m_colorWriteEnabled[1] = true;
    m_colorWriteEnabled[2] = true;
    m_colorWriteEnabled[3] = true;

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // -------------------------------------------------------------- 
    // Clear color
    // -------------------------------------------------------------- 
    m_colorClearValue[0] = 0.5f;
    m_colorClearValue[1] = 0.5f;
    m_colorClearValue[2] = 0.5f;
    m_colorClearValue[3] = 0.0f;

    glClearColor(m_colorClearValue[0],
                 m_colorClearValue[1],
                 m_colorClearValue[2],
                 m_colorClearValue[3]);
    
    // -------------------------------------------------------------- 
    // Depth
    // -------------------------------------------------------------- 
    m_depthWriteEnabled = true;
    glDepthMask(GL_TRUE);
     
    m_depthClearValue = 1.0f;
    glClearDepthf(1.0f);
    m_depthTest = P_GLDEPTHTEST_DEFAULT;
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    
    // -------------------------------------------------------------- 
    // Scissor
    // -------------------------------------------------------------- 
    m_scissorTestEnabled = false;
    glDisable(GL_STENCIL_TEST);

    m_scissor[0] = m_initialViewport[0];
    m_scissor[1] = m_initialViewport[1];
    m_scissor[2] = m_initialViewport[2];
    m_scissor[3] = m_initialViewport[3];
    glScissor(m_scissor[0], m_scissor[1], m_scissor[2], m_scissor[3]);

    // -------------------------------------------------------------- 
    // TODO: stencil
    // -------------------------------------------------------------- 

    // -------------------------------------------------------------- 
    // Blend
    // -------------------------------------------------------------- 
    m_blend = P_GLBLEND_DEFAULT;
    glDisable(GL_BLEND);
    
    // -------------------------------------------------------------- 
    // Culling
    // -------------------------------------------------------------- 
    m_cull = P_GLCULL_DEFAULT;
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // -------------------------------------------------------------- 
    // End
    // -------------------------------------------------------------- 
    pGlErrorCheckError();
}

void PGlState::getViewport(puint32* viewport) const
{
    viewport[0] = m_viewport[0];
    viewport[1] = m_viewport[1];
    viewport[2] = m_viewport[2];
    viewport[3] = m_viewport[3];
}

void PGlState::setViewport(puint32 x, puint32 y, puint32 width, puint32 height)
{
    // FIXME: We are still at 4K age.
    PASSERT(x < 4096 &&
            y < 4096 &&
            x + width <= 4096 &&
            y + height <= 4096);
    if (x >= 4096 || y >= 4096 || x + width > 4096 || y + height > 4096)
    {
        PLOG_ERRORX(P_LOG_CHANNEL_OPENGLEGL, "invalid viewport values: (%d, %d, %d, %d)", x, y, width, height);

        // FIXME: it is good to let application crash at OpenGL so we don't return here.
    }

    // Cache the viewport value.
    if (m_viewport[0] == x && m_viewport[1] == y && 
        m_viewport[2] == width && m_viewport[3] == height)  
    {
        return ;
    }

    m_viewport[0] = x;
    m_viewport[1] = y;
    m_viewport[2] = width;
    m_viewport[3] = height;

    glViewport(x, y, width, height);
}

void PGlState::setColorWriteEnabled(pbool red, pbool green, pbool blue, pbool alpha)
{
    if (m_colorWriteEnabled[0] == red &&
        m_colorWriteEnabled[1] == green &&
        m_colorWriteEnabled[2] == blue &&
        m_colorWriteEnabled[3] == alpha)
    {
        return ;
    }

    m_colorWriteEnabled[0] = red;
    m_colorWriteEnabled[1] = green;
    m_colorWriteEnabled[2] = blue;
    m_colorWriteEnabled[3] = alpha;

    GLboolean r = red? GL_TRUE : GL_FALSE;
    GLboolean g = green? GL_TRUE : GL_FALSE;
    GLboolean b = blue? GL_TRUE : GL_FALSE;
    GLboolean a = alpha? GL_TRUE : GL_FALSE;

    glColorMask(r, g, b, a);
}

void PGlState::setColorClearValue(pfloat32 red, pfloat32 green, pfloat32 blue, pfloat32 alpha)
{
    if (m_colorClearValue[0] == red &&
        m_colorClearValue[1] == green &&
        m_colorClearValue[2] == blue &&
        m_colorClearValue[3] == alpha)
    {
        return ;
    }

    m_colorClearValue[0] = red;
    m_colorClearValue[1] = green;
    m_colorClearValue[2] = blue;
    m_colorClearValue[3] = alpha;

    glClearColor(m_colorClearValue[0],
                 m_colorClearValue[1],
                 m_colorClearValue[2],
                 m_colorClearValue[3]);
}
    
void PGlState::setDepthClearValue(pfloat32 value)
{
    if (value != m_depthClearValue)
    {
        m_depthClearValue = value;
        glClearDepthf(value);
    }
}

void PGlState::setDepthTest(PGlStateEnum mode)
{
    if (mode == m_depthTest)
    {
        return ;
    }
    m_depthTest = mode;

    if (mode == P_GLDEPTHTEST_DISABLED)
    {
        glDisable(GL_DEPTH_TEST);
    }
    else
    {
        glEnable(GL_DEPTH_TEST);

        GLenum modes[] =
        {
            GL_LESS,
            GL_LEQUAL,
            GL_EQUAL,
            GL_GEQUAL,
            GL_GREATER,
            GL_NEVER,
            GL_ALWAYS,
        };

        glDepthFunc(modes[mode - P_GLDEPTHTEST_FIRST]);
    }
}

void PGlState::setDepthWriteEnabled(pbool enabled)
{
    if (enabled == m_depthWriteEnabled)
    {
        return ;
    }

    m_depthWriteEnabled = enabled;
    if (m_depthWriteEnabled)
    {
        glDepthMask(GL_TRUE);
    }
    else
    {
        glDepthMask(GL_FALSE);
    }
}

void PGlState::setCull(PGlStateEnum mode)
{
    if (m_cull == mode)
    {
        return;
    }

    m_cull = mode;

    switch (puint32(mode))
    {
        case P_GLCULL_NONE:
            glDisable(GL_CULL_FACE);
            break;
        case P_GLCULL_BACK:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            break;
        case P_GLCULL_FRONT:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            break;
        default:
            PASSERT_NOTREACHABLE("Unknown cull mode");
            break;
    }
}

void PGlState::setScissorTestEnabled(pbool enabled)
{
    if (enabled == m_scissorTestEnabled)
    {
        return ;
    } 

    m_scissorTestEnabled = enabled;

    if (m_scissorTestEnabled)
    {
        glEnable(GL_SCISSOR_TEST);
    }
    else
    {
        glDisable(GL_SCISSOR_TEST);
    }
}

void PGlState::setScissorRegion(puint32 x, puint32 y, puint32 width, puint32 height)
{
    if (m_scissor[0] == x &&
        m_scissor[1] == y &&
        m_scissor[2] == width &&
        m_scissor[3] == height)
    {
        return ;
    }

    // TODO: compute the intersection with screen.

    m_scissor[0] = x;
    m_scissor[1] = y;
    m_scissor[2] = width;
    m_scissor[3] = height;

    glScissor(m_scissor[0], m_scissor[1], m_scissor[2], m_scissor[3]);
}

void PGlState::setStencilTest(PGlStateEnum mode)
{
    if (mode == m_stencilTest)
    {
        return ;
    }

    m_stencilTest = mode;

    // TODO: Implement this function.
    PASSERT_NOTIMPLEMENTED();
}


void PGlState::setBlend(PGlStateEnum mode)
{
    if (m_blend == mode)
    {
        return ;
    }

    m_blend = mode;

    switch (mode)
    {
        case P_GLBLEND_OPAQUE:
            glDisable(GL_BLEND);
            break;
        case P_GLBLEND_ALPHA: 
            glEnable(GL_BLEND);
            glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case P_GLBLEND_DSTALPHA: 
            glEnable(GL_BLEND);
            glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
            glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
            break;
        case P_GLBLEND_ADDITIVE:
            glEnable(GL_BLEND);
            glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        default:
            PLOG_WARNINGX(P_LOG_CHANNEL_OPENGLEGL, "unknown blend mode");
            PASSERT(0);
            break;
    }
}

puint32 PGlState::query(PGlStateQueryEnum flag)
{
    GLint ret;
    switch (flag)
    {
        case P_GLTEXTURE_UNIT_NUMBER:
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &ret);
            break;
        default:
            PLOG_WARNINGX(P_LOG_CHANNEL_OPENGLEGL, "unsupported query flag");
            break;
    }

    pGlErrorCheckError();

    return ret; 
}
    


