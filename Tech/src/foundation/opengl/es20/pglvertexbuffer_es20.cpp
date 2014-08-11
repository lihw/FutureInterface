// PGlVertexBuffer_es20.cpp
// The wrapper of OpenGL vertex array.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
// 
// Hongwei Li lihw81@gmail.com
// 


#include "../pglvertexbuffer.h"

#include <PFoundation/passert.h>
#include <PFoundation/plog.h>
#include <PFoundation/pnew.h>
#include <PFoundation/pcrt.h>
#include <PFoundation/pglerror.h>

#if defined P_IOS
#  include <OpenGLES/ES2/gl.h>
#  include <OpenGLES/ES2/glext.h>
#else
#  include <GLES2/gl2.h>
#  include <GLES2/gl2ext.h>
#endif


// -------------------------------------------------------------- 
// PGlVertexFormat
// -------------------------------------------------------------- 

PGlVertexFormat::PGlVertexFormat()
{
    m_numAttribs = 0;
    m_size       = 0;

    for (puint32 i = 0; i < 8; ++i)
    {
        m_attribs[i].index  = -1;
        m_attribs[i].name   = P_NULL;
        m_attribs[i].size   = 0;
        m_attribs[i].type   = 0;
        m_attribs[i].offset = 0;
    }
}

PGlVertexFormat::PGlVertexFormat(const PGlVertexFormat &other)
{
    m_numAttribs = other.m_numAttribs;
    m_size       = other.m_size;
    for (puint32 i = 0; i < m_numAttribs; ++i)
    {
        m_attribs[i].index  = other.m_attribs[i].index;
        m_attribs[i].name   = pstrdup(other.m_attribs[i].name);
        m_attribs[i].size   = other.m_attribs[i].size;
        m_attribs[i].type   = other.m_attribs[i].type;
        m_attribs[i].offset = other.m_attribs[i].offset;
    }
}

PGlVertexFormat::~PGlVertexFormat()
{
    for (puint32 i = 0; i < m_numAttribs; ++i)
    {
        PDELETEARRAY(m_attribs[i].name);
    }
}

void PGlVertexFormat::operator=(const PGlVertexFormat &other)
{
    if (this != &other)
    {
        for (puint32 i = 0; i < m_numAttribs; ++i)
        {
            PDELETEARRAY(m_attribs[i].name);
        }
    
        m_numAttribs = other.m_numAttribs;
        m_size       = other.m_size;

        for (puint32 i = 0; i < m_numAttribs; ++i)
        {
            m_attribs[i].index  = other.m_attribs[i].index;
            m_attribs[i].name   = pstrdup(other.m_attribs[i].name);
            m_attribs[i].size   = other.m_attribs[i].size;
            m_attribs[i].type   = other.m_attribs[i].type;
            m_attribs[i].offset = other.m_attribs[i].offset;
        }
    }
}

void PGlVertexFormat::addAttrib(const pchar *name, pint32 size, puint32 type)
{
    if (m_numAttribs < 7)
    {
        m_attribs[m_numAttribs].name = pstrdup(name);
        m_attribs[m_numAttribs].size = size;
        m_attribs[m_numAttribs].type = type;

        const puint32 dataTypeSizes[] =
        {
            1,
            1,
            2,
            2,
            4,
            4,
            4
        };

        m_attribs[m_numAttribs].offset = m_size;
        m_size += dataTypeSizes[m_attribs[m_numAttribs].type] * 
                m_attribs[m_numAttribs].size;
        
        m_numAttribs++;
    }
}

pbool PGlVertexFormat::operator==(const PGlVertexFormat &other) const
{
    if (other.m_numAttribs != m_numAttribs || m_size != other.m_size)
    {
        return false;
    }

    // FIXME: can replace size and type comparsion with a single offset comparison?
#define ATTRIB_EQUAL(i) \
    (m_attribs[i].index == other.m_attribs[i].index && \
    m_attribs[i].size == other.m_attribs[i].size && \
    m_attribs[i].type == other.m_attribs[i].type)

    return ATTRIB_EQUAL(0) && ATTRIB_EQUAL(1) && 
           ATTRIB_EQUAL(2) && ATTRIB_EQUAL(3) && 
           ATTRIB_EQUAL(4) && ATTRIB_EQUAL(5) && 
           ATTRIB_EQUAL(6) && ATTRIB_EQUAL(7);

#undef ATTRIB_EQUAL
}

pbool PGlVertexFormat::calibrate(PGlVertexFormat *source, PGlVertexFormat *ref, PGlVertexFormat *out) 
{
    if (source->m_numAttribs < ref->m_numAttribs)
    {
        return false;
    }
        
    out->m_numAttribs = source->m_numAttribs;

    out->m_attribs[0].index = -1;
    out->m_attribs[1].index = -1;
    out->m_attribs[2].index = -1;
    out->m_attribs[3].index = -1;
    out->m_attribs[4].index = -1;
    out->m_attribs[5].index = -1;
    out->m_attribs[6].index = -1;
    out->m_attribs[7].index = -1;
    
    for (puint32 i = 0; i < ref->m_numAttribs; ++i)
    {
        puint32 j;
        for (j = 0; j < source->m_numAttribs; ++j)
        {
            if (pstrcmp(source->m_attribs[j].name, ref->m_attribs[i].name) == 0 &&
                source->m_attribs[j].size <= ref->m_attribs[i].size &&
                source->m_attribs[j].type == ref->m_attribs[i].type)
            {
                out->m_attribs[j].index  = i;
                out->m_attribs[j].size   = source->m_attribs[j].size; 
                out->m_attribs[j].type   = source->m_attribs[j].type; 
                out->m_attribs[j].offset = source->m_attribs[j].offset; 
                break;
            }
        }

        if (j == source->m_numAttribs)
        {
            return false;
        }
    }

    out->m_size = source->m_size;
    
    return true;
}

void PGlVertexFormat::enable(puint8 *offset)
{
    const GLenum dataTypes[] =
    {
        GL_BYTE,
        GL_UNSIGNED_BYTE,
        GL_SHORT,
        GL_UNSIGNED_SHORT,
        GL_INT,
        GL_UNSIGNED_INT,
        GL_FLOAT,
    };

    // Enable each vertex attribute. 
    for (puint32 i = 0; i < m_numAttribs; ++i)
    {
        if (m_attribs[i].index >= 0)
        {
            glVertexAttribPointer(m_attribs[i].index,
                                  m_attribs[i].size, 
                                  dataTypes[m_attribs[i].type], 
                                  GL_FALSE, 
                                  m_size,
                                  m_attribs[i].offset + offset);
            glEnableVertexAttribArray(m_attribs[i].index);
        }
    }
}

void PGlVertexFormat::disable()
{
    for (puint32 i = 0; i < m_numAttribs; ++i)
    {
        glDisableVertexAttribArray(i);
    }
}

// -------------------------------------------------------------- 
// PGlVertexBuffer
// -------------------------------------------------------------- 
PGlVertexBuffer::PGlVertexBuffer()
{
    m_primitive     = GL_TRIANGLES;
    m_vbo           = 0;
    m_ibo           = 0;
    m_vertices      = P_NULL;
    m_numVertices   = 0;
    m_indices       = P_NULL;
    m_numIndices    = 0;
    m_format        = P_NULL;
    m_enabled       = false;
}

PGlVertexBuffer::~PGlVertexBuffer()
{
    if (m_vbo != 0)
    {   
        glDeleteBuffers(1, &m_vbo);       
        m_vbo = 0;
    }
    if (m_ibo != 0)
    {
        glDeleteBuffers(1, &m_ibo);
        m_ibo = 0;
    }

    PDELETEARRAY(m_vertices);
    PDELETEARRAY(m_indices);

    PDELETE(m_format);

    pGlErrorCheckWarning();
}

pbool PGlVertexBuffer::create(PGlPritmiveEnum primitive,
                             const PGlVertexFormat &vertexFormat, 
                             void *vertices, 
                             puint32 numVertices, 
                             puint16 *indices, 
                             puint32 numIndices,
                             pbool isGPUResident)
{
    const GLenum primitives[] =
    {
        GL_TRIANGLES,
        GL_LINES,
        GL_POINTS,
    };

    PASSERT(m_vbo == 0 && m_ibo == 0);
    PASSERT(m_vertices == P_NULL && m_indices == P_NULL);

    // Sanity check
    if (vertices == P_NULL || indices == P_NULL)
    {
        PLOG_ERRORX(P_LOG_CHANNEL_OPENGLEGL, "Some parameters are NULL.");
        return false;
    }
    
    m_primitive = primitives[primitive];

    m_format = PNEW(PGlVertexFormat(vertexFormat));

    m_numVertices = numVertices;
    m_numIndices  = numIndices;

    // Compute the buffer size to hold all vertex data. 
    GLuint vertexBufferSize = m_numVertices * m_format->vertexSize();
    GLuint indexBufferSize  = m_numIndices * sizeof(puint16); 

    // Copy the vertex and index data to GPU and CPU if needed.
    if (isGPUResident)
    {
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ibo);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else
    {
        m_vertices = PNEWARRAY(puint8, vertexBufferSize);
        pmemcpy(m_vertices, vertices, vertexBufferSize);

        m_indices = PNEWARRAY(puint16, m_numIndices);
        pmemcpy(m_indices, indices, indexBufferSize);
    }

    pGlErrorCheckAbort();

    return true;
}
    
puint32 PGlVertexBuffer::numberOfPrimitives() const
{
    if (m_primitive == GL_TRIANGLES)
    {
        return m_numIndices / 3;
    }
    else if (m_primitive == GL_LINES)
    {
        return m_numIndices / 2;
    }
    else if (m_primitive == GL_POINTS)
    {
        return m_numIndices;
    }
    else
    {
        PLOG_WARNING("Can't find the number of complex primitives");
    }
    
    return 0;
}

void PGlVertexBuffer::render()
{
    PASSERT(m_enabled);
    PASSERT(m_vbo > 0 || m_vertices != P_NULL);

    if (m_enabled)
    {
        glDrawElements(m_primitive, m_numIndices, GL_UNSIGNED_SHORT, m_indices);
        pGlErrorCheckError();
    }
}

void PGlVertexBuffer::enable()
{
    // TODO: to use VAO OES 2.0 extension.
    
    // PRenderEngine will ensure the same VBO won't be enabled twice consecutively.
    PASSERT(!m_enabled);
    if (m_enabled)
    {
        return ;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

    m_enabled = true;

    pGlErrorCheckError();
}

void PGlVertexBuffer::disable()
{
    PASSERT(m_enabled);
    m_enabled = false;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PGlVertexBuffer::useDefault()
{
    PASSERT(!m_enabled);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

    
