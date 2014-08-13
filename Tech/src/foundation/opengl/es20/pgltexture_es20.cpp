// pgltexture_es20.cpp
// The OpenGL texture
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "../pgltexture.h"

#include <PFoundation/pglerror.h>
#include <PFoundation/plog.h>
#include <PFoundation/passert.h>

#if defined P_IOS
#  include <OpenGLES/ES2/gl.h>
#  include <OpenGLES/ES2/glext.h>
#else
#  include <GLES2/gl2.h>
#  include <GLES2/gl2ext.h>
#endif


PGlTexture::PGlTexture()
{
    m_texture           = 0;
    m_target            = 0;
    m_compressed        = false;
    m_mipmap            = false;
    m_minFiltering      = P_GLTEXTURE_FILTERING_NEAREST;
    m_magFiltering      = P_GLTEXTURE_FILTERING_NEAREST;
    m_wrapModeS         = P_GLTEXTURE_WRAPMODE_CLAMP_TO_EDGE;
    m_wrapModeT         = P_GLTEXTURE_WRAPMODE_CLAMP_TO_EDGE;
    m_enabled           = false;
    m_bytes             = 0;
    m_textureUnit       = 0xffffffff;
}

PGlTexture::~PGlTexture()
{
    if (m_texture != 0)
    {
        glDeleteTextures(1, &m_texture);

        pGlErrorCheckWarning();
    }
}

pbool PGlTexture::create(puint8* data, puint32 size, PGlTextureFormatEnum format, pbool mipmap)
{
    m_target = GL_TEXTURE_2D;
    m_compressed = true;

    // TODO: 
    PASSERT_NOTIMPLEMENTED();

    return false;
}

pbool PGlTexture::create(puint8* data, puint32 width, puint32 height, 
    PGlTextureFormatEnum format, pbool mipmap)
{
    // We can't overwrite an existing texture.
    if (m_texture != 0)
    {
        PLOG_WARNINGX(P_LOG_CHANNEL_OPENGLEGL, "Texture can't be overwritten");
        return false;
    }

    glGenTextures(1, &m_texture);

    m_target = GL_TEXTURE_2D;
    m_compressed = false;
    m_mipmap = mipmap;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(m_target, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Use the same texture format for internalFormat and format in OpenGL ES 2.0.
    puint32 internalFormat;
    puint32 dataFormat;
    interpretFormat(format, internalFormat, dataFormat);

    // Compute the size of the texture
    switch (format)
    {
        case P_GLTEXTURE_FORMAT_R8: m_bytes = width * height; break;
        case P_GLTEXTURE_FORMAT_RA88: m_bytes = width * height * 2; break;
        case P_GLTEXTURE_FORMAT_RGB888: m_bytes = width * height * 3; break;
        case P_GLTEXTURE_FORMAT_RGBA8888: m_bytes = width * height * 4; break;
        default:
            PASSERT(!"Unsupported OpenGL texture format");
            PLOG_ERRORX(P_LOG_CHANNEL_OPENGLEGL, "Unsupported OpenGL texture format");
            glDeleteTextures(1, &m_texture);
            return false;
    }

    // Create the OpenGL texture object.
    glTexImage2D(GL_TEXTURE_2D, 
                 0, 
                 internalFormat, 
                 width, 
                 height, 
                 0, 
                 dataFormat, 
                 GL_UNSIGNED_BYTE, 
                 data);
    
    setMipmapInternal(m_mipmap);
    
    setFilteringInternal(m_minFiltering, m_magFiltering);

    setWrapModeInternal(m_wrapModeS, m_wrapModeT);

    glBindTexture(m_target, 0);

    pGlErrorCheckAbort();

    return true;
}
    
pbool PGlTexture::create(puint8 **data, puint32 *width, puint32 *height, PGlTextureFormatEnum format, 
        pbool mipmap)
{
    // We can't overwrite an existing texture.
    if (m_texture != 0)
    {
        PLOG_WARNINGX(P_LOG_CHANNEL_OPENGLEGL, "Texture can't be overwritten");
        return false;
    }

    glGenTextures(1, &m_texture);

    m_target = GL_TEXTURE_CUBE_MAP;
    m_compressed = false;
    m_mipmap = mipmap;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(m_target, m_texture);

    // Use the same texture format for internalFormat and format in OpenGL ES 2.0.
    puint32 internalFormat;
    puint32 dataFormat;
    interpretFormat(format, internalFormat, dataFormat);

    // Compute the size of the texture
    m_bytes = 0;
    for (pint32 i = 0; i < 6; i++)
    {
        switch (format)
        {
            case P_GLTEXTURE_FORMAT_R8: m_bytes += width[i] * height[i]; break;
            case P_GLTEXTURE_FORMAT_RA88: m_bytes += width[i] * height[i] * 2; break;
            case P_GLTEXTURE_FORMAT_RGB888: m_bytes += width[i] * height[i] * 3; break;
            case P_GLTEXTURE_FORMAT_RGBA8888: m_bytes += width[i] * height[i] * 4; break;
            default:
                PASSERT(!"Unsupported OpenGL texture format");
                PLOG_ERRORX(P_LOG_CHANNEL_OPENGLEGL, "Unsupported OpenGL texture format");
                glDeleteTextures(1, &m_texture);
                return false;
        }
    }

    // Create the OpenGL texture object.
    for (pint32 i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                     0, 
                     internalFormat, 
                     width[i], 
                     height[i], 
                     0, 
                     dataFormat, 
                     GL_UNSIGNED_BYTE, 
                     data[i]);
    }
    
    setMipmapInternal(m_mipmap);
    
    setFilteringInternal(m_minFiltering, m_magFiltering);

    if (m_wrapModeS != P_GLTEXTURE_WRAPMODE_CLAMP_TO_EDGE ||
        m_wrapModeT != P_GLTEXTURE_WRAPMODE_CLAMP_TO_EDGE)
    {
        PLOG_WARNINGX(P_LOG_CHANNEL_OPENGLEGL, "Cubemap only supports wrapping of clamping to edge");
        m_wrapModeS = P_GLTEXTURE_WRAPMODE_CLAMP_TO_EDGE;
        m_wrapModeT = P_GLTEXTURE_WRAPMODE_CLAMP_TO_EDGE;
    }
    setWrapModeInternal(m_wrapModeS, m_wrapModeT);

    glBindTexture(m_target, 0);

    // FIXME: how to ensure the completeness of the cubemap.

    pGlErrorCheckAbort();

    return true;
}

pbool PGlTexture::create(puint8 **data, puint32 size, PGlTextureFormatEnum format, pbool mipmap)
{
    m_target = GL_TEXTURE_CUBE_MAP;
    m_compressed = true;

    // TODO:

    return false;
}
    
pbool PGlTexture::isCubeMap() const
{
    return m_target == GL_TEXTURE_CUBE_MAP;
}

void PGlTexture::setWrapMode(PGLTextureWrapModeEnum wrapModeS,
    PGLTextureWrapModeEnum wrapModeT)
{
    if (m_texture != 0)
    {
        if (m_wrapModeS != wrapModeS ||
            m_wrapModeT != wrapModeT)
        {
            if (isCubeMap())
            {
                if (wrapModeS != P_GLTEXTURE_WRAPMODE_CLAMP_TO_EDGE ||
                    wrapModeT != P_GLTEXTURE_WRAPMODE_CLAMP_TO_EDGE)
                {
                    PASSERTINFO(0, "Wrap mode must be clamp to edge when the texture is a cubemap.");
                    return;
                }
            }
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(m_target, m_texture);
            setWrapModeInternal(wrapModeS, wrapModeT);
            glBindTexture(m_target, 0);
        }
    }
        
    m_wrapModeS = wrapModeS;
    m_wrapModeT = wrapModeT;
}

void PGlTexture::setFiltering(PGlTextureFilteringEnum minFiltering, 
    PGlTextureFilteringEnum magFiltering)
{
    if (m_texture != 0)
    {
        if (m_minFiltering != minFiltering ||
            m_magFiltering != magFiltering)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(m_target, m_texture);
            setFilteringInternal(minFiltering, magFiltering);
            glBindTexture(m_target, 0);
        }
    }

    m_minFiltering = minFiltering;
    m_magFiltering = magFiltering;
}

void PGlTexture::enable(puint32 textureUnit)
{
    PASSERT(!m_enabled);

    if (m_enabled)
    {
        return ;
    }

    m_enabled = true;
    
    pGlErrorCheckError();
    
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(m_target, m_texture);

    m_textureUnit = textureUnit;

    pGlErrorCheckError();
}

void PGlTexture::disable()
{
    PASSERT(m_enabled);
    PASSERT(m_textureUnit != 0xffffffff);
    
        pGlErrorCheckError();
    
    if (!m_enabled)
    {
        return ;
    }
    m_enabled = false;
    
    glActiveTexture(GL_TEXTURE0 + m_textureUnit);
    glBindTexture(m_target, 0);

    m_textureUnit = 0xffffffff;
    
    pGlErrorCheckError();
}
    
void PGlTexture::copyTexture(PGlTextureFormatEnum format, 
        puint32 x, puint32 y, puint32 width, puint32 height, puint32 border)
{
    PASSERT(m_target == GL_TEXTURE_2D);
    PASSERT(m_enabled);

    puint32 internalFormat;
    puint32 dataFormat;
    interpretFormat(format, internalFormat, dataFormat);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, width, height, border);

    pGlErrorCheckError();
}

void PGlTexture::setWrapModeInternal(PGLTextureWrapModeEnum wrapModeS,
    PGLTextureWrapModeEnum wrapModeT) 
{
    if (wrapModeS == P_GLTEXTURE_WRAPMODE_REPEAT)
    {
        glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    }
    else if (wrapModeS == P_GLTEXTURE_WRAPMODE_CLAMP_TO_EDGE)
    {
        glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    }
    else if (wrapModeS == P_GLTEXTURE_WRAPMODE_MIRRORED_REPEAT)
    {
        glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    }

    if (wrapModeT == P_GLTEXTURE_WRAPMODE_REPEAT)
    {
        glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else if (wrapModeT == P_GLTEXTURE_WRAPMODE_CLAMP_TO_EDGE)
    {
        glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    else if (wrapModeT == P_GLTEXTURE_WRAPMODE_MIRRORED_REPEAT)
    {
        glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    }
            
    pGlErrorCheckError();
}

void PGlTexture::setFilteringInternal(PGlTextureFilteringEnum minFiltering, 
    PGlTextureFilteringEnum magFiltering)
{
    if (m_mipmap)
    {
        if (minFiltering == P_GLTEXTURE_FILTERING_NEAREST)
        {
            glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        }
        else
        {
            glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }

        if (magFiltering == P_GLTEXTURE_FILTERING_NEAREST)
        {
            glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        else
        {
            glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
    }
    else
    {
        if (minFiltering == P_GLTEXTURE_FILTERING_NEAREST)
        {
            glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
        else
        {
            glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }

        if (magFiltering == P_GLTEXTURE_FILTERING_NEAREST)
        {
            glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        else
        {
            glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
    }
            
    pGlErrorCheckError();
}

void PGlTexture::setMipmapInternal(pbool mipmap)
{
    if (mipmap)
    {
        m_bytes += m_bytes / 3;
        glGenerateMipmap(m_target);
        pGlErrorCheckError();
    }
}
    
void PGlTexture::interpretFormat(PGlTextureFormatEnum format, 
    puint32 &internalFormat,
    puint32 &dataFormat)
{
    switch (format)
    {
        case P_GLTEXTURE_FORMAT_RGBA8888: 
            internalFormat = GL_RGBA;
            dataFormat = GL_RGBA;
            break;
        case P_GLTEXTURE_FORMAT_RGB888: 
            internalFormat = GL_RGB;
            dataFormat = GL_RGB;
            break;
        case P_GLTEXTURE_FORMAT_R8:
            internalFormat = GL_LUMINANCE;
            dataFormat = GL_LUMINANCE;
            break;
        case P_GLTEXTURE_FORMAT_RA88:
            internalFormat = GL_LUMINANCE_ALPHA;
            dataFormat = GL_LUMINANCE_ALPHA;
            break;
        default:
            PASSERT(!"Unsupported texture format");
            PLOG_ERRORX(P_LOG_CHANNEL_OPENGLEGL, "Unsupported texture format");
            break;
    }
}
