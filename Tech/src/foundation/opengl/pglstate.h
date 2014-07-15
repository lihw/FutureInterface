// pglstate.h
// The OpenGL render state
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#ifndef PGLSTATE_H 
#define PGLSTATE_H 

#include <PFoundation/pglobal.h>

enum PGlStateEnum
{
    // Stencil
    // FIXME: As we do not have to implement stencil for the first version of this engine,
    // leave the codes here and will fix them later.
    P_GLSTENCILTEST_DISABLED,      
    P_GLSTENCILTEST_BACK,    // Back stencil status set
    P_GLSTENCILTEST_FRONT,   // Front stencil status set  

    // Depth
    P_GLDEPTHTEST_LESS,
    P_GLDEPTHTEST_LEQUAL,
    P_GLDEPTHTEST_EQUAL,
    P_GLDEPTHTEST_GEQUAL,
    P_GLDEPTHTEST_GREATER,
    P_GLDEPTHTEST_NEVER,
    P_GLDEPTHTEST_ALWAYS,
    P_GLDEPTHTEST_DISABLED,
    
    // Cull 
    P_GLCULL_NONE,    ///< No culling. 
    P_GLCULL_BACK,    ///< Backface culling. 
    P_GLCULL_FRONT,   ///< Frontface culling. 

    // Blending
    P_GLBLEND_OPAQUE,   ///< No blending. 
    P_GLBLEND_ALPHA,    ///< Blend source and destination color using source alpha. 
    P_GLBLEND_DSTALPHA, ///< Blend source and destination color using destination alpha.
    P_GLBLEND_ADDITIVE, ///< Add source to destination color
    
    P_GLSTENCILTEST_DEFAULT = P_GLSTENCILTEST_DISABLED,
    P_GLSTENCILTEST_FIRST   = P_GLSTENCILTEST_DISABLED,
    P_GLSTENCILTEST_LAST    = P_GLSTENCILTEST_BACK,
    P_GLSTENCILTEST_NUMBER  = P_GLSTENCILTEST_LAST - P_GLSTENCILTEST_FIRST + 1,
    
    P_GLDEPTHTEST_DEFAULT = P_GLDEPTHTEST_LEQUAL,
    P_GLDEPTHTEST_FIRST   = P_GLDEPTHTEST_LESS,
    P_GLDEPTHTEST_LAST    = P_GLDEPTHTEST_DISABLED,
    P_GLDEPTHTEST_NUMBER  = P_GLDEPTHTEST_LAST - P_GLDEPTHTEST_FIRST + 1,
    
    P_GLCULL_DEFAULT = P_GLCULL_BACK,
    P_GLCULL_FIRST   = P_GLCULL_NONE,
    P_GLCULL_LAST    = P_GLCULL_FRONT,
    P_GLCULL_NUMBER  = P_GLCULL_LAST - P_GLCULL_FIRST + 1,
    
    P_GLBLEND_DEFAULT = P_GLBLEND_OPAQUE,
    P_GLBLEND_FIRST   = P_GLBLEND_OPAQUE,
    P_GLBLEND_LAST    = P_GLBLEND_ADDITIVE,
    P_GLBLEND_NUMBER  = P_GLBLEND_LAST - P_GLBLEND_FIRST + 1,
};

enum PGlStateQueryEnum
{
    P_GLTEXTURE_UNIT_NUMBER, ///< The maximum texture unit number.
};

class P_DLLEXPORT PGlState
{
public:
    PGlState(const puint32 *initialViewport);
    ~PGlState();

    // Initialize or reset the state to default.
    void reset();

    void setViewport(puint32 x, puint32 y, puint32 width, puint32 height);

    void setColorWriteEnabled(pbool red, pbool green, pbool blue, pbool alpha);
    void setColorClearValue(pfloat32 red, pfloat32 green, pfloat32 blue, pfloat32 alpha);

    void setDepthWriteEnabled(pbool enabled);
    void setDepthClearValue(pfloat32 value);
    void setDepthTest(PGlStateEnum mode);

    void setScissorTestEnabled(pbool enabled);
    void setScissorRegion(puint32 x, puint32 y, puint32 width, puint32 height);

    void setBlend(PGlStateEnum mode);

    P_INLINE PGlStateEnum cull() const { return m_cull; }
    void setCull(PGlStateEnum mode);

    void setStencilTest(PGlStateEnum mode);
    // TODO: more about stencil testing.

    void getViewport(puint32* viewport) const;

    puint32 query(PGlStateQueryEnum flag);

protected:
    puint32            m_initialViewport[4]; // The initial width and height
    puint32            m_viewport[4]; // The viewport
    pbool              m_colorWriteEnabled[4]; // Color mask 
    pfloat32           m_colorClearValue[4]; // Clear color
    pbool              m_depthWriteEnabled; // Depth 
    pfloat32           m_depthClearValue;
    PGlStateEnum       m_depthTest;
    pbool              m_scissorTestEnabled; /// Scissor 
    pint32             m_scissor[4];
    PGlStateEnum       m_blend; /// Blend 
    PGlStateEnum       m_cull; /// Cull
    PGlStateEnum       m_stencilTest;  // Stencil
    PGlStateEnum       m_fill;  // Stencil
};



#endif // !PGLSTATE_H

