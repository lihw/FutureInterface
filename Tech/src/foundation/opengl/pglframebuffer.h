// pglframebuffer.h
// The framebuffer object.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
// 
// Hongwei Li lihw81@gmail.com
// 


#ifndef PGLFRAMEBUFFER_H
#define PGLFRAMEBUFFER_H

#include <PFoundation/pgltexture.h>

class PGlRenderbuffer;

class P_DLLEXPORT PGlFramebuffer
{
public:
    PGlFramebuffer();
    ~PGlFramebuffer();

    // Create the framebuffer and the texture with the given size.
    // Create depthbuffer and stencilbuffer for depth and stencil storage.
    // Check framebuffer status if error.
    pbool create(puint32 width, puint32 height, PGlTextureFormatEnum colorFormat, 
             PGlTextureFormatEnum depthFormat, PGlTextureFormatEnum stencilFormat);

    // Enable the framebuffer when render.
    void enable();
    
    P_INLINE const PGlTexture *colorBuffer() const { return m_colorBuffer; }
    P_INLINE const PGlRenderbuffer *depthBuffer() const { return m_depthBuffer; }
    P_INLINE const PGlRenderbuffer *stencilBuffer() const { return m_stencilBuffer; }

    static void clearFramebuffer(pbool color, pbool depth, pbool stencil);
    static PGlFramebuffer *createDefaultFramebuffer();

private:
    pbool checkFramebufferStatus();

private:
    PGlTexture      *m_colorBuffer;
    PGlRenderbuffer *m_depthBuffer;
    PGlRenderbuffer *m_stencilBuffer;
    puint32          m_framebuffer;
};

#endif // !PGLFRAMEBUFFER_H
