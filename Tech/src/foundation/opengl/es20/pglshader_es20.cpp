// pglshader_es20.cpp
// OpenGL shader wrapper.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
// 
// Hongwei Li lihw81@gmail.com
// 

#include "../pglshader.h"

#include <PFoundation/plog.h>
#include <PFoundation/passert.h>
#include <PFoundation/pnew.h>
#include <PFoundation/pglerror.h>
#include <PFoundation/pcrt.h>

#if defined P_IOS
#  include <OpenGLES/ES2/gl.h>
#  include <OpenGLES/ES2/glext.h>
#else
#  include <GLES2/gl2.h>
#  include <GLES2/gl2ext.h>
#endif


PGlShader::PGlShader()
{
    m_program   = 0;
#if defined P_DEBUG
    m_enabled   = false;
#endif
}

PGlShader::~PGlShader()
{    
#if defined P_DEBUG
    if (m_enabled && m_program != 0)
    {
        glUseProgram(0);
    }
#endif
    
    // Delete the program. 
    if (m_program != 0)
    {
        glDeleteProgram(m_program);
    }
    
    pGlErrorCheckWarning();
}

pbool PGlShader::compile(const pchar* vertexProgramSource, const pchar* fragmentProgramSource)
{
    GLint compiled;

    // If it is already loaded.
    if (m_program != 0)
    {
        PLOG_ERRORX(P_LOG_CHANNEL_OPENGLEGL, "Can't modify an existing shader program");
        return false;
    }

    // Compile the vertex shader. 
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const char **)&vertexProgramSource, 0);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);

    if (!compiled)
    {
        GLint logLength, charsWritten;

        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

        pchar* log = PNEWARRAY(pchar, logLength + 1);

        glGetShaderInfoLog(vertexShader, logLength, &charsWritten, log);

        PLOG_ERRORX(P_LOG_CHANNEL_OPENGLEGL, "Vertex shader compiling failed.");
        PLOG_INFOX(P_LOG_CHANNEL_OPENGLEGL, log);

        PDELETEARRAY(log);
            
        // Release the resource and return. 
        glDeleteShader(vertexShader);

        return false;
    }
    
    // Compile fragment source. 
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const char **)&fragmentProgramSource, 0);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);

    if (!compiled)
    {
        GLint logLength, charsWritten;

        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);

        pchar* log = PNEWARRAY(pchar, logLength + 1);
        glGetShaderInfoLog(fragmentShader, logLength, &charsWritten, log);

        PLOG_ERRORX(P_LOG_CHANNEL_OPENGLEGL, "Fragment shader compiling failed.");
        PLOG_INFOX(P_LOG_CHANNEL_OPENGLEGL, log);

        PDELETEARRAY(log);
            
        // Release the reSource and return. 
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);

        return false;
    }
    
    // Create the OpenGL program object. 
    m_program = glCreateProgram();

    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);

    // Link the shaders. 
    glLinkProgram(m_program);

    // We should delete shader as soon as possible to have less maintenance.
    // In order to avoid glError, we can even detach the shaders after linking.
    glDetachShader(m_program, vertexShader);
    glDetachShader(m_program, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    GLint linked;
    glGetProgramiv(m_program, GL_LINK_STATUS, &linked);

    if (!linked)
    {
        GLint logLength, charsWritten;

        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);

        pchar * log = PNEWARRAY(pchar, logLength + 1);
        glGetProgramInfoLog(m_program, logLength, &charsWritten, log);

        PLOG_ERRORX(P_LOG_CHANNEL_OPENGLEGL, "shader linking failed");
        PLOG_INFOX(P_LOG_CHANNEL_OPENGLEGL, log);

        PDELETEARRAY(log);

        // Restore the state and return false.
        glDeleteProgram(m_program);
        m_program = 0;
        
        return false;
    }
    
    // Validate the current program. 
    glValidateProgram(m_program);

    GLint status;
    glGetProgramiv(m_program, GL_VALIDATE_STATUS, &status);
    if (!status)
    {
        GLint logLength, charsWritten;

        glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);
    
        pchar* log = PNEWARRAY(pchar, logLength + 1);
        glGetProgramInfoLog(m_program, logLength, &charsWritten, log);

        PLOG_ERRORX(P_LOG_CHANNEL_OPENGLEGL, "Validation of the shader failed");
        PLOG_INFOX(P_LOG_CHANNEL_OPENGLEGL, log);

        PDELETEARRAY(log);
        
        glDeleteProgram(m_program);
        m_program = 0;
        return false;
    }

    // The link will implicitly bind the program to the pipeline so
    // we need to remove it from pipeline explicitly.
    glUseProgram(0);

    pGlErrorCheckAbort();

    return true;
}

#if defined P_OPENGL_BINARY_PROGRAM_ENABLED

pbool PGlShader::programBinary(pEnum binaryFormat, const void* binary, pint32 length)
{
    // If this shader program has been already compiled and linked. 
    if (m_program != 0)
    {
        PLOG_ERRORX(P_LOG_CHANNEL_OPENGLEGL, "Can't modify an existing shader program");
        return false;
    }

    // Create the OpenGL program object. 
    program = glCreateProgram();

    // Load the binary program. 
    glProgramBinaryOES(m_program, binaryFormat, binary, length);

    // Validate the current program. 
    glValidateProgram(m_program);

    glGetProgramiv(m_program, GL_VALIDATE_STATUS, &status);
    if (!status)
    {
        PLOG_ERRORX(P_LOG_CHANNEL_OPENGLEGL, "validation of the shader failed");
        glDeleteProgram(m_program);
        m_program = 0;
        return false;
    }

    // The glProgramBinaryOES will implicitly bind the program to the pipeline.
#if defined P_DEBUG
    m_enabled = true;
#endif

    pGlErrorCheckErrorAbort();

    return true;
}

pbool PGlShader::getProgramBinary(pEnum* binaryFormat, const void** binary, pint32* length)
{
    GLint bufSize;
    puint8* binaryData;
    GLint status;

    if (m_program != 0)
    {
        PLOG_ERRORX(P_LOG_CHANNEL_OPENGLEGL, "Not a valid shader");
        return false;
    }

    // Get the length of the binary program.
    glGetProgramiv(m_program, PROGRAM_BINARY_LENGTH_OES, &bufSize);

    *length = bufSize;

    // Dump the binary program into the buffer when needed.
    if (binary == P_NULL)
    {
        puint8* binaryData = PNEW(puint8[bufSize]);

        glGetProgramBinaryOES(m_program, bufSize, binaryFormat, binaryData, length);
        
        *binary = binaryData;
    }
    
    pGlErrorCheckErrorAbort();

    return true;
}

#endif // P_OPENGL_BINARY_PROGRAM_ENABLED

void PGlShader::enable()
{
    // As PRenderEngine will ensure the same shader won't be 
    // enabled twice consecutively.
    PASSERT(!m_enabled);
#if defined P_DEBUG
    if (m_enabled)
    {
        return ;
    }
    m_enabled = true;
#endif

    // Switch the current shader when necessary. 
    glUseProgram(m_program);


    pGlErrorCheckError();
}

void PGlShader::disable()
{
    // As render engine will ensure the same shader won't be 
    // disabled twice consecutively.
    PASSERT(m_enabled);

#if defined P_DEBUG
    m_enabled = false;
#endif

    glUseProgram(0);

    pGlErrorCheckError();
}

void PGlShader::useDefault()
{
    PASSERT(!m_enabled);
    glUseProgram(0);
    pGlErrorCheckError();
}

pint32 PGlShader::uniformLocation(const pchar* name)
{
    return glGetUniformLocation(m_program, name);
}

void PGlShader::uniform(pint32 location, pfloat32 x)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif 
        glUniform1f(location, x);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniform(pint32 location, pfloat32 x, pfloat32 y)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniform2f(location, x, y);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniform(pint32 location, pfloat32 x, pfloat32 y, pfloat32 z)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniform3f(location, x, y, z);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniform(pint32 location, pfloat32 x, pfloat32 y, pfloat32 z, pfloat32 w)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniform4f(location, x, y, z, w);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniform(pint32 location, pint32 x)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniform1i(location, x);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniform(pint32 location, pint32 x, pint32 y)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniform2i(location, x, y);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniform(pint32 location, pint32 x, pint32 y, pint32 z)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniform3i(location, x, y, z);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniform(pint32 location, pint32 x, pint32 y, pint32 z, pint32 w)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniform4i(location, x, y, z, w);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniform1iv(pint32 location, const pint32 *value, pint32 count)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniform1iv(location, count, value);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniform2iv(pint32 location, const pint32 *value, pint32 count)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniform2iv(location, count, value);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniform3iv(pint32 location, const pint32 *value, pint32 count)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniform3iv(location, count, value);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniform4iv(pint32 location, const pint32 *value, pint32 count)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniform4iv(location, count, value);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniform1fv(pint32 location, const pfloat32 *value, pint32 count)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniform1fv(location, count, value);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniform2fv(pint32 location, const pfloat32 *value, pint32 count)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniform2fv(location, count, value);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniform3fv(pint32 location, const pfloat32 *value, pint32 count)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniform3fv(location, count, value);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniform4fv(pint32 location, const pfloat32 *value, pint32 count)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniform4fv(location, count, value);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniformMatrix2fv(pint32 location, const pfloat32 *value, pint32 count)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniformMatrix2fv(location, count, GL_FALSE, value);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniformMatrix3fv(pint32 location, const pfloat32 *value, pint32 count)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniformMatrix3fv(location, count, GL_FALSE, value);
#if defined P_DEBUG
    }
#endif
}

void PGlShader::uniformMatrix4fv(pint32 location, const pfloat32 *value, pint32 count)
{
#if defined P_DEBUG
    if (m_enabled)
    {
#endif
        glUniformMatrix4fv(location, count, GL_FALSE, value);
#if defined P_DEBUG
    }
#endif
}

pbool PGlShader::isDefault() const
{
    return m_program == 0;
}
    
puint32 PGlShader::numberOfUniforms()
{
    GLint nuniforms;
    glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &nuniforms);

    pGlErrorCheckError();

    return puint32(nuniforms);
}

PGlShaderUniform PGlShader::uniformInformation(puint32 index)
{
    PGlShaderUniform ret;
    ret.m_index = index;

    GLenum  type   = 0;
    GLint   size   = -1;
    GLsizei length = 0;
    // TODO: query the maximum length of the active uniform.
    glGetActiveUniform(m_program, index, 1024, &length, &size, &type, ret.m_name);

    pGlErrorCheckError();

    switch (type)
    {
        case GL_FLOAT:        ret.m_type = P_GLSHADERUNIFORM_FLOAT; break;
        case GL_FLOAT_VEC2:   ret.m_type = P_GLSHADERUNIFORM_FLOAT2; break;
        case GL_FLOAT_VEC3:   ret.m_type = P_GLSHADERUNIFORM_FLOAT3; break;
        case GL_FLOAT_VEC4:   ret.m_type = P_GLSHADERUNIFORM_FLOAT4; break;
        case GL_INT:          ret.m_type = P_GLSHADERUNIFORM_INT; break;
        case GL_INT_VEC2:     ret.m_type = P_GLSHADERUNIFORM_INT2; break;
        case GL_INT_VEC3:     ret.m_type = P_GLSHADERUNIFORM_INT3; break;
        case GL_INT_VEC4:     ret.m_type = P_GLSHADERUNIFORM_INT4; break;
        case GL_BOOL:         ret.m_type = P_GLSHADERUNIFORM_BOOL; break;
        case GL_BOOL_VEC2:    ret.m_type = P_GLSHADERUNIFORM_BOOL2; break;
        case GL_BOOL_VEC3:    ret.m_type = P_GLSHADERUNIFORM_BOOL3; break;
        case GL_BOOL_VEC4:    ret.m_type = P_GLSHADERUNIFORM_BOOL4; break;
        case GL_FLOAT_MAT2:   ret.m_type = P_GLSHADERUNIFORM_FLOATMATRIX2X2; break;
        case GL_FLOAT_MAT3:   ret.m_type = P_GLSHADERUNIFORM_FLOATMATRIX3X3; break;
        case GL_FLOAT_MAT4:   ret.m_type = P_GLSHADERUNIFORM_FLOATMATRIX4X4; break;
        case GL_SAMPLER_2D:   ret.m_type = P_GLSHADERUNIFORM_SAMPLER2D; break;
        case GL_SAMPLER_CUBE: ret.m_type = P_GLSHADERUNIFORM_SAMPLERCUBE; break;
        default:
            ret.m_type = P_GLSHADERUNIFORM_UNKNOWN;
            PASSERT(!"Unrecognized OpenGL uniform variable type");
            PLOG_WARNINGX(P_LOG_CHANNEL_OPENGLEGL, "Unrecognized OpenGL uniform variable type");
            break;
    }

    // Save size
    ret.m_size = size;

    // When the uniform is an array of primitive data types, like vec3, the uniform
    // name returned by glGetActiveUniform will be platform dependent. In NV cards,
    // the name will be the variable name, while in AMD cards, the first element in
    // array will be returned. We unify the returned name to be the first element always.
    if (ret.m_size > 1)
    {
        if (pstrstr(ret.m_name, "[0]") == P_NULL)
        {
            pstrcat(ret.m_name, "[0]");
        }
    }

    // Get the location of this uniform
    GLint loc = glGetUniformLocation(m_program, ret.m_name);
    PASSERT(loc >= 0);
    if (loc < 0)
    {
        PLOG_WARNINGX(P_LOG_CHANNEL_OPENGLEGL, "Unable to find the location of %s", ret.m_name);
    }
    ret.m_location = loc;

    return ret;
}
    
PGlVertexFormat PGlShader::vertexFormat() const
{
    PGlVertexFormat ret;
    
    GLint numVertexAttribs;
    GLint maxVertexNameLength;
    glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &numVertexAttribs);
    glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxVertexNameLength);

    pchar *name = PNEWARRAY(pchar, maxVertexNameLength + 1);
    for (GLint i = 0; i < numVertexAttribs; ++i)
    {
        GLsizei length;
        GLint size;
        GLenum type;

        glGetActiveAttrib(m_program, i, maxVertexNameLength, &length, &size, &type, name); 
    
        switch (type)
        {
            case GL_FLOAT:      ret.addAttrib(name, 1, P_FLOAT32); break;
            case GL_FLOAT_VEC2: ret.addAttrib(name, 2, P_FLOAT32); break;
            case GL_FLOAT_VEC3: ret.addAttrib(name, 3, P_FLOAT32); break;
            case GL_FLOAT_VEC4: ret.addAttrib(name, 4, P_FLOAT32); break;
            case GL_FLOAT_MAT2: 
            case GL_FLOAT_MAT3: 
            case GL_FLOAT_MAT4: 
                PASSERT_NOTIMPLEMENTED(); 
                break;
        }
    }

    PDELETEARRAY(name);

    return ret;
}
