// pxmlelement.cpp
// A xml node
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pxmlelement.h"

#include <PFoundation/pxmldocument.h>

#include <PFoundation/pnew.h>
#include <PFoundation/passert.h>
#include <PFoundation/plog.h>
#include <PFoundation/pcrt.h>

#include <tinyxml2.h>

#include "pxmlstruct_private.h"

//
// class PXmlElement
//
PXmlElement::PXmlElement(const PXmlDocument* document)
{
    m_node = PNEW(PXmlElementPrivate);
    PASSERT(document != P_NULL &&
             document->m_document != P_NULL &&
             document->m_document->m_document != P_NULL);
    m_document = const_cast<PXmlDocument*>(document);
    m_node->m_element = P_NULL;
}

PXmlElement::~PXmlElement()
{
    PDELETE(m_node);
    // The xml node memory is managed by tinyxml so no need
    // to delete it here.
}
    
PXmlElement::PXmlElement(const PXmlElement& other)
{
    m_node = PNEW(PXmlElementPrivate);
    m_node->m_element = other.m_node->m_element;
    m_document = other.m_document;
}

const PXmlElement& PXmlElement::operator=(const PXmlElement& other)
{
    if (&other != this)
    {
        m_node->m_element = other.m_node->m_element;
        m_document = other.m_document;
    }

    return *this;
}
    
pbool PXmlElement::isValid() const
{
    return m_node->m_element != P_NULL;
}
    
void PXmlElement::addChild(const PXmlElement& child)
{
    if (m_node->m_element == P_NULL)
    {
        PLOG_WARNING("The parent of this doc node is invalid.");
        return ;
    }

    PASSERT(child.m_node != P_NULL && child.m_node->m_element != P_NULL);
    m_node->m_element->InsertEndChild(child.m_node->m_element);
}

PXmlElement PXmlElement::firstChild() const
{
    PXmlElement ret(m_document);
    
    if (m_node->m_element == P_NULL)
    {
        return ret;
    }

    ret.m_node->m_element = m_node->m_element->FirstChildElement();
    return ret;
}

PXmlElement PXmlElement::lastChild() const
{
    PXmlElement ret(m_document);
    if (m_node->m_element == P_NULL)
    {
        return ret;
    }

    ret.m_node->m_element = m_node->m_element->LastChildElement();
    return ret;
}

PXmlElement PXmlElement::nextSibling() const
{
    PXmlElement ret(m_document);

    if (m_node->m_element == P_NULL)
    {
        return ret;
    }

    ret.m_node->m_element = m_node->m_element->NextSiblingElement();

    return ret;
}

PXmlElement PXmlElement::previousSibling() const
{
    PXmlElement ret(m_document);
    if (m_node->m_element == P_NULL)
    {
        return ret;
    }

    ret.m_node->m_element = m_node->m_element->PreviousSiblingElement();

    return ret;
}
    
PXmlElement PXmlElement::child(const pchar *childName) const
{
    PXmlElement ret(m_document);
    if (m_node->m_element == P_NULL)
    {
        return ret;
    }

    ret.m_node->m_element = m_node->m_element->FirstChildElement(childName);

    return ret;
}

pbool PXmlElement::noChildren() const
{
    if (m_node->m_element == P_NULL)
    {
        return true;
    }

    return m_node->m_element->NoChildren();
}

const pchar *PXmlElement::name() const
{
    if (m_node->m_element == P_NULL)
    {
        return P_NULL;
    }
    
    return m_node->m_element->Value();
}

void PXmlElement::setName(const pchar *name)
{
    PASSERT(m_node != P_NULL);
    if (m_node->m_element == P_NULL)
    {
        m_node->m_element = m_document->m_document->m_document->NewElement(name);
    }
    else
    {
        m_node->m_element->SetName(name);
    }
}

const pchar *PXmlElement::attribute(const pchar *name) const
{
    PASSERT(name != P_NULL);

    if (m_node == P_NULL || m_node->m_element == P_NULL)
    {
        return P_NULL;
    }

    return m_node->m_element->Attribute(name);
}

pbool PXmlElement::queryIntAttribute(const pchar *name, pint32 &value) const
{
    PASSERT(name != P_NULL);

    const pchar *attr = attribute(name);
    if (attr == P_NULL)
    {
        return false;
    }

    value = patoi(attr);
    return true;
}

pbool PXmlElement::queryUintAttribute(const pchar *name, puint32 &value) const
{
    PASSERT(name != P_NULL);

    const pchar *attr = attribute(name);
    if (attr == P_NULL)
    {
        return false;
    }

    value = patoi(attr);
    return true;
}

pbool PXmlElement::queryBooleanttribute(const pchar *name, pbool& value) const
{
    PASSERT(name != P_NULL);

    const pchar *attr = attribute(name);
    if (attr == P_NULL)
    {
        return false;
    }

    if (attr[0] == 't' &&
        attr[1] == 'r' &&
        attr[2] == 'u' &&
        attr[3] == 'e' &&
        attr[4] == '\0')
    {
        value = true;
    }
    else
    {
        value = false;
    }

    return true;
}

pbool PXmlElement::queryFloatAttribute(const pchar *name, pfloat32 &value) const
{
    PASSERT(name != P_NULL);

    const pchar *attr = attribute(name);
    if (attr == P_NULL)
    {
        return false;
    }

    value = patof(attr);
    return true;
}

void PXmlElement::addStringAttribute(const pchar *name, const pchar *value)
{
    PASSERT(m_node->m_element != P_NULL);
    if (m_node->m_element == P_NULL)
    {
        return ;
    }

    m_node->m_element->SetAttribute(name, value);
}

void PXmlElement::addIntAttribute(const pchar *name, pint32 value)
{
    PASSERT(m_node->m_element != P_NULL);
    if (m_node->m_element == P_NULL)
    {
        return ;
    }

    m_node->m_element->SetAttribute(name, value);
}

void PXmlElement::addUintAttribute(const pchar *name, puint32 value)
{
    PASSERT(m_node->m_element != P_NULL);
    if (m_node->m_element == P_NULL)
    {
        return ;
    }

    m_node->m_element->SetAttribute(name, value);
}

void PXmlElement::addBooleanttribute(const pchar *name, pbool value)
{
    PASSERT(m_node->m_element != P_NULL);
    if (m_node->m_element == P_NULL)
    {
        return ;
    }

    m_node->m_element->SetAttribute(name, value);
}

void PXmlElement::addFloatAttribute(const pchar *name, pfloat32 value)
{
    PASSERT(m_node->m_element != P_NULL);
    if (m_node->m_element == P_NULL)
    {
        return ;
    }

    m_node->m_element->SetAttribute(name, value);
}
    
void PXmlElement::setText(const pchar *text)
{
    PASSERT(m_node->m_element != P_NULL);

    if (m_node->m_element != P_NULL)
    {
        tinyxml2::XMLText* t = m_node->m_element->GetDocument()->NewText(text);
        m_node->m_element->InsertEndChild(t);
    }
}

const pchar *PXmlElement::text()
{
    PASSERT(m_node->m_element != P_NULL);

    if (m_node->m_element != P_NULL)
    {
        return m_node->m_element->GetText();
    }

    return P_NULL;
}
