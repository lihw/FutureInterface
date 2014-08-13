// pxmldocument.cpp
// A xml document 
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pxmldocument.h"

#include <PFoundation/pinputstream.h>
#include <PFoundation/poutputstream.h>

#include <PFoundation/pnew.h>
#include <PFoundation/passert.h>
#include <PFoundation/plog.h>
#include <PFoundation/pcrt.h>

#include <tinyxml2.h>

#include "pxmlstruct_private.h"


PXmlDocument::PXmlDocument()
{
    m_document = PNEW(PXmlDocumentPrivate);
    m_document->m_document = PNEW(tinyxml2::XMLDocument);
}

PXmlDocument::~PXmlDocument()
{
    if (m_document->m_document != P_NULL)
    {
        PDELETE(m_document->m_document);
    }
    PDELETE(m_document);
}

PXmlDocument::PXmlDocument(const PXmlDocument& other)
{
    if (other.m_document != P_NULL)
    {
        m_document = PNEW(PXmlDocumentPrivate);
        m_document->m_document = other.m_document->m_document;
    }
    else
    {
        m_document = P_NULL;
    }
}

const PXmlDocument& PXmlDocument::operator=(const PXmlDocument& other)
{
    if (this != &other)
    {
        m_document->m_document = other.m_document->m_document;
    }

    return *this;
}

PXmlElement PXmlDocument::firstChildElement() const
{
    PXmlElement ret(this);
    if (m_document == P_NULL || m_document->m_document == P_NULL)
    {
        return ret;
    }

    ret.m_node->m_element = m_document->m_document->FirstChildElement();
    return ret;
}

PXmlElement PXmlDocument::lastChildElement() const
{
    PXmlElement ret(this);
    if (m_document == P_NULL || m_document->m_document == P_NULL)
    {
        return ret;
    }

    ret.m_node->m_element = m_document->m_document->LastChildElement();
    return ret;
}
    
void PXmlDocument::addChildElement(const PXmlElement& childElement)
{
    if (m_document != P_NULL && m_document->m_document != P_NULL)
    {
        if (childElement.m_node != P_NULL && 
            childElement.m_node->m_element != P_NULL &&
            childElement.m_document == this)
        {
            m_document->m_document->InsertEndChild(childElement.m_node->m_element);
        }
    }
}

pbool PXmlDocument::save(POutputStream &outputStream)
{
    PASSERT_NOTIMPLEMENTED();
    return false;
}

pbool PXmlDocument::parse(PInputStream &inputStream)
{
    if (m_document != P_NULL && m_document->m_document != P_NULL)
    {
        puint8 *text;
        inputStream.readAllBytes(text);
        pbool ret = parse((const pchar *)text);
        PDELETEARRAY(text);        
        return ret;
    }

    return false;
}

pbool PXmlDocument::parse(const pchar *text)
{
    if (m_document != P_NULL && m_document->m_document != P_NULL)
    {
        puint32 len = pstrlen(text);
        pint32 ret = m_document->m_document->Parse((const pchar *)text, len);
        if (ret != tinyxml2::XML_NO_ERROR)
        {
            PLOG_ERROR("Failed to parse XML file.");
            PLOG_ERROR(m_document->m_document->GetErrorStr1());
            return false;
        }

        return true;
    }

    return false;
}
