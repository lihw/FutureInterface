// pglvertexbuffer.h
// The wrapper of OpenGL vertex buffer object.
// 
// Copyright 2012 - 2014 Future Interface Li. 
// This software is licensed under the terms of the MIT license.
// 
// Hongwei Li lihw81@gmail.com
//

#ifndef PGLVERTEXBUFFER_H
#define PGLVERTEXBUFFER_H

#include <PFoundation/passert.h>

enum PGlPritmiveEnum
{
    P_GLTRIANGLES = 0,
    P_GLLINES     = 1,
    P_GLPOINTS    = 2,

    P_GLPRIMITIVE_DEFAULT = P_GLTRIANGLES,

    P_GLPRIMITIVE_FIRST = P_GLTRIANGLES,
    P_GLPRIMITIVE_LAST = P_GLPOINTS,
    P_GLPRIMITIVE_NUMBER = P_GLPRIMITIVE_LAST - P_GLPRIMITIVE_FIRST + 1,
};

struct P_DLLEXPORT PGlVertexFormat
{
    struct Attrib
    {
        pint32   index;    // The index
        pchar   *name;     // 
        pint32   size;     // The number of the elements in this vertex attribute. 
        puint32  type;     // The type of the element in this vertex attribute. 
        puint32  offset;   // The offset of this attribute from beginning.
    };

    PGlVertexFormat();
    PGlVertexFormat(const PGlVertexFormat &other);
    ~PGlVertexFormat();
    void operator=(const PGlVertexFormat &other);

    void clear();
    void enable(puint8 *offset);
    void disable();
    void addAttrib(const pchar *name, pint32 size, puint32 type);
    pbool operator==(const PGlVertexFormat &other) const;
    
    // Readjust the order of attribute in source to match those in ref and
    // generate the returned vertex format. Return false when there are some
    // attributes that can't be found in source.
    static pbool calibrate(PGlVertexFormat *source, PGlVertexFormat *ref, PGlVertexFormat *out); 

    P_INLINE pbool operator!=(const PGlVertexFormat &other) const { return !(operator==(other)); }
    P_INLINE const Attrib& attrib(puint32 index) { PASSERT(index < m_numAttribs); return m_attribs[index]; }
    P_INLINE puint32 vertexSize() const { return m_size; }
    
    puint32 m_size; 
    puint32 m_numAttribs;
    Attrib  m_attribs[8]; // The OpenGL ES 2.0 supports up to 8 vertex attributes

private:
};

class P_DLLEXPORT PGlVertexBuffer
{
public:
    // Constructor.
    PGlVertexBuffer();
    
    // Destructor
    ~PGlVertexBuffer();

    // Create a vertex array object. 
    pbool create(PGlPritmiveEnum primitive,
                const PGlVertexFormat &vertexFormat, 
                void *vertices, 
                puint32 numVertices, 
                puint16 *indices, 
                puint32 numIndices,
                pbool isGPUResident);

    // Render the vertex array. 
    void render();

    // The VBO will be set to the current rendering VBO.
    void enable();

    // The VBO will be disabled.
    void disable();

    // Use default VBO 0.
    void useDefault();

    P_INLINE pbool isEnabled() const { return m_enabled; }

    P_INLINE puint32 numberOfVertices() const { return m_numVertices; }
    P_INLINE puint32 numberOfIndices() const { return m_numIndices; }
    P_INLINE puint8 *vertices() const { return m_vertices; }
    P_INLINE puint16 *indices() const { return m_indices; }
    P_INLINE PGlVertexFormat *vertexFormat() const { return m_format; }

    puint32 numberOfPrimitives() const;

private:
    PGlVertexFormat   *m_format; // The description of each vertex attribute. 
    puint32            m_primitive; // The rendering pritmive of this vbo.
    puint32            m_vbo; // The vertex buffer object. It is zero when CPU copy is only needed.
    puint32            m_ibo; // The index buffer in GPU. It is zero when buffer is stored in GPU. 
    puint8*            m_vertices; // The vertex buffer in CPU. It is NULL when VBO is only needed. 
    puint32            m_numVertices; // The number of vertices.
    puint16           *m_indices; // The index buffer in CPU. It is NULL when buffer is in GPU. 
    puint32            m_numIndices; // The number of indices. 
    pbool               m_enabled; // If it is the current VBO.
};


#endif // !PGLVERTEXBUFFER_H
