// pglrenderbuffer.h
// The OpenGL render buffer
//
// Copyright 2012 - 2014 Future Interface . 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#ifndef PGLRENDERBUFFER_H
#define PGLRENDERBUFFER_H

#include <PFoundation/pgltexture.h>

class P_DLLEXPORT PGlRenderbuffer
{
    friend class PGlFramebuffer;

private:
    PGlRenderbuffer();
    ~PGlRenderbuffer();

    pbool create(puint32 width, puint32 height, PGlTextureFormatEnum format);

public:
    P_INLINE puint32 getRenderbufferHandle() const { return m_renderbuffer; }

private:
    puint32 m_renderbuffer;  
};


#endif // !PGLRENDERBUFFER_H
