// pxmldocument.h
// A xml document 
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PXMLDOCUMENT_H
#define PXMLDOCUMENT_H

#include <PFoundation/pxmlelement.h>


struct PXmlDocumentPrivate;
class PInputStream;
class POutputStream;


class PXmlDocument
{
    friend class PXmlElement;

public:
    PXmlDocument();
    PXmlDocument(const PXmlDocument& other);
    ~PXmlDocument();
    
    const PXmlDocument& operator=(const PXmlDocument& other);

    PXmlElement firstChildElement() const;
    PXmlElement lastChildElement() const;

    void addChildElement(const PXmlElement& childElement);
    
    pbool save(POutputStream &outputStream);
    pbool parse(PInputStream &inputStream);
    pbool parse(const pchar *text);

private:
    PXmlDocumentPrivate* m_document;
};


#endif // !PXMLDOCUMENT_H
