// pgltexture.h
// The OpenGL texture
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#ifndef PGLTEXTURE_H
#define PGLTEXTURE_H


#include <PFoundation/pglobal.h>

enum PGlTextureFilteringEnum
{
    P_GLTEXTURE_FILTERING_NEAREST,
    P_GLTEXTURE_FILTERING_LINEAR,
};

enum PGLTextureWrapModeEnum
{
    P_GLTEXTURE_WRAPMODE_REPEAT,
    P_GLTEXTURE_WRAPMODE_CLAMP_TO_EDGE,
    P_GLTEXTURE_WRAPMODE_MIRRORED_REPEAT,
};

enum PGlTextureFormatEnum 
{
    P_GLTEXTURE_FORMAT_RGBA8888,
    P_GLTEXTURE_FORMAT_RGB888,
    P_GLTEXTURE_FORMAT_RA88,
    P_GLTEXTURE_FORMAT_R8,
    P_GLTEXTURE_FORMAT_DEPTH_COMPONENT16,
    P_GLTEXTURE_FORMAT_STENCIL_INDEX8,
    P_GLTEXTURE_FORMAT_UNKNOWN,

    P_GLTEXTURE_FORMAT_FIRST = P_GLTEXTURE_FORMAT_RGBA8888,
    P_GLTEXTURE_FORMAT_LAST = P_GLTEXTURE_FORMAT_UNKNOWN,
    P_GLTEXTURE_FORMAT_NUMBER = P_GLTEXTURE_FORMAT_LAST - P_GLTEXTURE_FORMAT_FIRST,
};

class P_DLLEXPORT PGlTexture
{
public:
    PGlTexture();
    ~PGlTexture();

    pbool create(puint8 *data, puint32 width, puint32 height, PGlTextureFormatEnum format, pbool mipmap);
    pbool create(puint8 *data, puint32 size, PGlTextureFormatEnum foramt, pbool mipmap);
    pbool create(puint8 **data, puint32 *width, puint32 *height, PGlTextureFormatEnum format, pbool mipmap);
    pbool create(puint8 **data, puint32 size, PGlTextureFormatEnum foramt, pbool mipmap);
    void setWrapMode(PGLTextureWrapModeEnum wrapModeS, PGLTextureWrapModeEnum wrapModeT);
    void setFiltering(PGlTextureFilteringEnum minFiltering, PGlTextureFilteringEnum magFiltering);

    P_INLINE pbool isCompressed() const { return m_compressed; }
    P_INLINE pbool isMipmap() const { return m_mipmap; }
    P_INLINE puint32 bytes() const { return m_bytes; }
    P_INLINE puint32 textureHandle() const { return m_texture; }
    P_INLINE puint32 textureUnit() const { return m_textureUnit; }
    P_INLINE pbool isEnabled() const { return m_enabled; }

    pbool isCubeMap() const; 

    void enable(puint32 textureUnit);
    void disable();


    void copyTexture(PGlTextureFormatEnum format, puint32 x, puint32 y, 
        puint32 width, puint32 height, puint32 border);

private:
    void setWrapModeInternal(PGLTextureWrapModeEnum wrapModeS, PGLTextureWrapModeEnum wrapModeT);
    void setFilteringInternal(PGlTextureFilteringEnum minFiltering, PGlTextureFilteringEnum magFiltering);
    void setMipmapInternal(pbool mipmap);
    void interpretFormat(PGlTextureFormatEnum format, puint32 &internalFormat, puint32 &dataFormat);

private:
    puint32                 m_texture;  
    puint32                 m_target; // E.g., GL_TEXTURE_2D
    pbool                    m_compressed; // Is it a compressed texture?
    puint32                 m_bytes; // The size of texture data.
    PGlTextureFilteringEnum m_minFiltering;
    PGlTextureFilteringEnum m_magFiltering;
    PGLTextureWrapModeEnum  m_wrapModeS;
    PGLTextureWrapModeEnum  m_wrapModeT;
    pbool                   m_mipmap; // Is it a mipmap texture?
    puint32                 m_textureUnit; // The texture unit it is bound to currently
    pbool                   m_enabled; // If this texture is used by some shader right now.
};


#endif // !PGLTEXTURE_H
