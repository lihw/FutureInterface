// parchivefile.h
// Read and save the archive file
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// lihw81@gmail.com
//

#include "parchivefile.h"

#include <PFoundation/passet.h>

PArchiveFile::PArchiveFile()
{
}

PArchiveFile::~PArchiveFile()
{
}

pbool PArchiveFile::load(const pchar *filePath)
{
    if (filePath == P_NULL)
    {
        PLOG_ERROR("Invalid archive file path");
        return false;
    }
    m_archivePath = PString(filePath);

    // FIXME: refactor these system dependent code to independent ones.
    PInputStream inputStream;

    PAsset asset = pAssetOpen(filePath);
    if (!pAssetIsValid(&asset))
    {
        PLOG_ERROR("Not a valid asset archive file: %s", filePath);
        return false;
    }

    // Read header
    if (!readArchiveHeader(&asset))
    {
       PLOG_ERROR("Failed to parse archive header %s", filePath);
       return false;
    }

    // Read file names
    puint8 *allFileNames = PNEWARRAY(puint8, m_header.allFileNameSize);
    pAssetSeek(&asset, m_header.fileNamesOffset, P_FILE_SEEK_FROM_BEGINNING);
    if (pAssetRead(&asset, allFileNames, 
        m_header.allFileNameSize) != m_header.allFileNameSize)
    {
       PLOG_ERROR("Failed to read file names in archive %s", filePath);
       PDELETEARRAY(allFileNames);
       return false;
    }

    puint8 *entryBuffer = PNEWARRAY(puint8, m_header.allFileEntrySize);
    pAssetSeek(&asset, m_header.fileEntryOffset, P_FILE_SEEK_FROM_BEGINNING);
    if (pAssetRead(&asset, entryBuffer, 
        m_header.allFileEntrySize) != m_header.allFileEntrySize)
    {
       PLOG_ERROR("Failed to read file entries in archive %s", filePath);
       PDELETEARRAY(allFileNames);
       PDELETEARRAY(entryBuffer);
       return false;
    }

    // Parse file entries
    puint32 entrySize = sizeof(PArchiveEntry) - sizeof(PString);
    pchar *p = (pchar *)allFileNames;
    for (puint32 i = 0; i < m_header.fileCount; ++i)
    {
        PArchiveEntry entry;

        entry.byteOffset = toUint32(&entryBuffer[entrySize * i]);
        entry.fileSize   = toUint32(&entryBuffer[entrySize * i + 4]);
        
        puint32 length = pstrlen(p);
        entry.filePath = PString(p, length);
        p += length + 1;
        
        m_entries.append(entry);
    }

    PDELETEARRAY(allFileNames);
    PDELETEARRAY(entryBuffer);

    pAssetClose(&asset);
    
    return true;
}

puint32 PArchiveFile::getNumberOfFiles() const
{
    return m_entries.count();
}

void PArchiveFile::getFilenames(PArray<PString> &out_filenames) const
{
    for (puint32 i = 0; i < m_entries.size(); ++i)
    {
        out_filenames.append(m_entries[i].filePath);
    }
}

pbool PArchiveFile::createInputStream(const pchar *filePath, 
    PInputStream *out_inputStream)
{
    // No archive loaded or the archive is empty
    if (m_entries.isEmpty())
    {
        PLOG_WARNING("No archive is loaded or the archive is empty.");
        return out_inputStream->createFromFile(filePath, P_STREAM_ENDIANNESS_PLATFORM);
    }

    const PArchiveEntry* entry = findEntry(filePath);
    if (entry == P_NULL)
    {
        PLOG_WARNING("Failed to locate entry (%s) in archive.", filePath);
        return out_inputStream->createFromFile(filePath, P_STREAM_ENDIANNESS_PLATFORM);
    }

    puint8 *buffer = PNEWARRAY(puint8, entry->fileSize);
    PAsset asset = pAssetOpen(m_archivePath.c_str());
    if (!pAssetIsValid(&asset))
    {
        PLOG_ERROR("Not a valid asset archive file: %s.", filePath);
        return out_inputStream->createFromFile(filePath, P_STREAM_ENDIANNESS_PLATFORM);
    }
    pAssetSeek(&asset, entry->byteOffset, P_FILE_SEEK_FROM_BEGINNING);
    pbool success = (pAssetRead(&asset, buffer, entry->fileSize) == entry->fileSize);
    pAssetClose(&asset);
    
    if (success)
    {
        return out_inputStream->createFromMemory(buffer, entry->fileSize, 
            (PStreamEndianessEnum)m_header.endianess);
    }
    else
    {
        PLOG_ERROR("Failed to create stream for %s.", filePath);
        PDELETEARRAY(buffer);
        return false;
    }

    return false;
}

pbool PArchiveFile::isPathInArchive(const pchar *filePath) const
{
    return findEntry(filePath) != P_NULL;
}

const PArchiveEntry* PArchiveFile::findEntry(const pchar *filePath) const
{
    // TODO: use sorted array
    for (puint32 i = 0; i < m_entries.count(); ++i)
    {
        if (m_entries[i].filePath == filePath)
        {
            return &m_entries[i];
        }
    }

    return P_NULL;
}

pbool PArchiveFile::readArchiveHeader(PAsset *asset)
{
    if (pAssetRead(asset, &m_header, sizeof(PArchiveHeader)) != sizeof(PArchiveHeader))
    {
        PLOG_ERROR("Failed to read asset file %s", m_archivePath.c_str());
        return false;
    }
    
    if (m_header.magic[0] != 0xca || 
        m_header.magic[1] != 0xfe || 
        m_header.magic[2] != 0xbe || 
        m_header.magic[3] != 0xbe)
    {
        PLOG_ERROR("Archive header is corrupted");
        return false;
    }

    puint8 *e = (puint8 *)&m_header.endianess;
    if (e[0] == 0x01 && e[1] == 0x02 && e[2] == 0x03 && e[3] == 0x04)
    {
        m_header.endianess = P_STREAM_ENDIANNESS_BIG;
    }
    else if (e[0] == 0x04 && e[1] == 0x03 && e[2] == 0x02 && e[3] == 0x01)
    {
        m_header.endianess = P_STREAM_ENDIANNESS_LITTLE;
    }
    else
    {
        PLOG_ERROR("Uknown endianness of the archvie. The archive may be courrpted");
        return false;
    }

    m_header.fileCount        = toUint32((puint8*)&m_header.fileCount);
    m_header.fileEntryOffset  = toUint32((puint8*)&m_header.fileEntryOffset);
    m_header.fileNamesOffset  = toUint32((puint8*)&m_header.fileNamesOffset);
    m_header.allFileEntrySize = toUint32((puint8*)&m_header.allFileEntrySize);
    m_header.allFileNameSize  = toUint32((puint8*)&m_header.allFileNameSize);

    return true;
}

puint32 PArchiveFile::toUint32(puint8 *quadruple) const
{
    if (m_header.endianess == P_STREAM_ENDIANNESS_LITTLE)
    {
        return (puint32)quadruple[0] | ((puint32)quadruple[1] << 8) |
            ((puint32)quadruple[2] << 16) | ((puint32)quadruple[3] << 24);
    }
        
    return (puint32)quadruple[3] | ((puint32)quadruple[2] << 8) |
            ((puint32)quadruple[1] << 16) | ((puint32)quadruple[0] << 24);
}
