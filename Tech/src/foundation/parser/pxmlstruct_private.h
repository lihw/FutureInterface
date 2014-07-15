// pxmlstruct_private.h
// private data structure for xml reading/writing
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PXMLSTRUCT_PRIVATE_H
#define PXMLSTRUCT_PRIVATE_H

struct PXmlElementPrivate
{
    tinyxml2::XMLElement* m_element;
};

struct PXmlDocumentPrivate
{
    tinyxml2::XMLDocument* m_document;
};

#endif // !PXMLSTRUCT_PRIVATE_H
