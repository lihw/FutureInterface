// pxmlelement.h
// A xml document 
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PXMLELEMENT_H
#define PXMLELEMENT_H

#include <PFoundation/pglobal.h>

struct PXmlElementPrivate;
struct PXmlDocumentPrivate;

class PXmlDocument;

class P_DLLEXPORT PXmlElement
{
    friend class PXmlDocument;    

public:
    PXmlElement(const PXmlDocument* document);
    ~PXmlElement();
    PXmlElement(const PXmlElement& other);
    const PXmlElement& operator=(const PXmlElement& other);

    PXmlElement firstChild() const;
    PXmlElement lastChild() const;
    PXmlElement nextSibling() const;
    PXmlElement previousSibling() const;
    PXmlElement child(const pchar *childName) const;

    void addChild(const PXmlElement& child);

    pbool isValid() const;

    pbool noChildren() const;

    const pchar *name() const;
    void setName(const pchar *name); 

    void setText(const pchar *text);
    const pchar *text();

    P_INLINE PXmlDocument *document() const { return m_document; }

    const pchar *attribute(const pchar *name) const;

    pbool queryIntAttribute(const pchar *name, pint32 &value) const;
    pbool queryUintAttribute(const pchar *name, puint32 &value) const;
    pbool queryBooleanttribute(const pchar *name, pbool &value) const;
    pbool queryFloatAttribute(const pchar *name, pfloat32 &value) const;

    void addStringAttribute(const pchar *name, const pchar *value);
    void addIntAttribute(const pchar *name, pint32 value);
    void addUintAttribute(const pchar *name, puint32 value);
    void addBooleanttribute(const pchar *name, pbool value);
    void addFloatAttribute(const pchar *name, pfloat32 value);
    
private:
    PXmlElementPrivate *m_node;
    PXmlDocument       *m_document;
};

#endif // !PXMLELEMENT_H
