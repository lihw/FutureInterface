// pglshader.h
// OpenGL Shader wrapper.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
// 
// Hongwei Li lihw81@gmail.com
// 

#ifndef PGLSHADER_H
#define PGLSHADER_H

#include <PFoundation/pglvertexbuffer.h>


enum PGlShaderUniformTypeEnum
{
    P_GLSHADERUNIFORM_FLOAT,
    P_GLSHADERUNIFORM_FLOAT2,
    P_GLSHADERUNIFORM_FLOAT3,
    P_GLSHADERUNIFORM_FLOAT4,
    P_GLSHADERUNIFORM_INT,
    P_GLSHADERUNIFORM_INT2,
    P_GLSHADERUNIFORM_INT3,
    P_GLSHADERUNIFORM_INT4,
    P_GLSHADERUNIFORM_BOOL,
    P_GLSHADERUNIFORM_BOOL2,
    P_GLSHADERUNIFORM_BOOL3,
    P_GLSHADERUNIFORM_BOOL4,
    P_GLSHADERUNIFORM_FLOATMATRIX2X2,
    P_GLSHADERUNIFORM_FLOATMATRIX3X3,
    P_GLSHADERUNIFORM_FLOATMATRIX4X4,
    P_GLSHADERUNIFORM_SAMPLER2D,
    P_GLSHADERUNIFORM_SAMPLERCUBE,

    P_GLSHADERUNIFORM_UNKNOWN,

    P_GLSHADERUNIFORM_TYPE_FIRST = P_GLSHADERUNIFORM_FLOAT,
    P_GLSHADERUNIFORM_TYPE_LAST = P_GLSHADERUNIFORM_SAMPLERCUBE,
    P_GLSHADERUNIFORM_TYPE_NUMBER = P_GLSHADERUNIFORM_TYPE_LAST - P_GLSHADERUNIFORM_TYPE_FIRST + 1,
};

struct P_DLLEXPORT PGlShaderUniform
{
    puint32                  m_index;
    puint32                  m_location;
    pchar                    m_name[1024];
    PGlShaderUniformTypeEnum m_type;
    pint32                   m_size;

    P_INLINE pbool isArray() const { return m_size > 1; }
};


class P_DLLEXPORT PGlShader
{
public:
    // Constructor
    PGlShader();

    // Destructor
    ~PGlShader();

    // Compile and link shader code. The programming of shader source should
    // conform to Future Interface OpenGL Shader Guideline. 
    pbool compile(const pchar *vertexProgramSource, const pchar *fragmentProgramSource);

#if defined P_OPENGL_BINARY_PROGRAM_ENABLED
    // Load program binary into a program object. It doesn't affect the current
    // shader in the pipeline.
    pbool programBinary(puint32 binaryFormat, const void* binary, pint32 length);

    // Dump program binary to a string. 
    pbool getProgramBinary(puint32* binaryFormat, void** binary, pint32* length);
#endif

    // Bind this shader object to the pipeline. 
    // \return true when succeed
    void enable();

    // Disable this shader.
    void disable();

    // Use default shader 0 on the pipeline.
    void useDefault();

    // Check if this shader is phony
    pbool isDefault() const;

    /// Update the value of the uniform when it is different from the current
    /// one in this shader object. If the shader is not current one in the pipeline,
    /// the value is uploaded to GPU when the shader is bound to pipeline. 
    void uniform(pint32 location, pfloat32 x);
    void uniform(pint32 location, pfloat32 x, pfloat32 y);
    void uniform(pint32 location, pfloat32 x, pfloat32 y, pfloat32 z);
    void uniform(pint32 location, pfloat32 x, pfloat32 y, pfloat32 z, pfloat32 w);
    void uniform(pint32 location, pint32 x);
    void uniform(pint32 location, pint32 x, pint32 y);
    void uniform(pint32 location, pint32 x, pint32 y, pint32 z);
    void uniform(pint32 location, pint32 x, pint32 y, pint32 z, pint32 w);    
    void uniform1iv(pint32 location, const pint32 *value, pint32 count);
    void uniform2iv(pint32 location, const pint32 *value, pint32 count);
    void uniform3iv(pint32 location, const pint32 *value, pint32 count);
    void uniform4iv(pint32 location, const pint32 *value, pint32 count);
    void uniform1fv(pint32 location, const pfloat32 *value, pint32 count);
    void uniform2fv(pint32 location, const pfloat32 *value, pint32 count);
    void uniform3fv(pint32 location, const pfloat32 *value, pint32 count);
    void uniform4fv(pint32 location, const pfloat32 *value, pint32 count);
    void uniformMatrix2fv(pint32 location, const pfloat32 *value, pint32 count);
    void uniformMatrix3fv(pint32 location, const pfloat32 *value, pint32 count);
    void uniformMatrix4fv(pint32 location, const pfloat32 *value, pint32 count);

    // Get the number of active uniforms in this shader object.
    puint32 numberOfUniforms();

    // Get the information of the uniform at certain index.
    PGlShaderUniform uniformInformation(puint32 index);

    // Get the location of certain uniforms.
    pint32 uniformLocation(const pchar *name);

    // Get vertex attributes
    PGlVertexFormat vertexFormat() const;

private:
    puint32 m_program;   // OpenGL program object. When this value is 0, it is a null shader. 
#if defined P_DEBUG
    pbool   m_enabled; // If it is currently bound to the pipeline.
#endif
};

#endif // !PGLSHADER_H
