// parchivefile.h
// Read and save the archive file
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PARCHIVEFILE_H
#define PARCHIVEFILE_H

#include <PFoundation/parray.h>
#include <PFoundation/pstring.h>
#include <PFoundation/pinputstream.h>

struct PAsset;

// Future Interface Archive File Format
//
//
// <magic>                ; 4 bytes, always 0xca, 0xfe, 0xbe, 0xbe
// <version>              ; 4 bytes, 0x0, 0x0, major, minor
// <endianess>            ; 4 bytes, the endianess of the platform that issues 
//                        ; this archive. We write 0x01020304 to the field when 
//                        ; creating the archive. It will ends up in the archive 
//                        ; 0x01, 0x02, 0x03, 0x4 if the platform is big endian 
//                        ; or little endian otherwise.
// <fileCount>            ; A 32bit uint. The number of files in this archive
// <fileEntryOffset>      ; A 32bit uint. The starting offset to the beginning 
//                        ; of archive of the entry section
// <fileNameOffset>       ; A 32bit uint. The starting offset to the beginning 
//                        ; of the archive of the file name section
// <fileEntrySize>        ; A 32bit uint. The data size of all file entries
// <fileNameSize>         ; A 32bit uint. The data size of all file names
// <exntension>           ; 128 bytes, for future extension.
// <file names>
// <file entries>

struct PArchiveHeader
{
    puint8  magic[4];
    puint8  version[4];
    puint32 endianess;
    puint32 fileCount;
    puint32 fileEntryOffset;
    puint32 fileNamesOffset;
    puint32 allFileEntrySize;
    puint32 allFileNameSize;
    puint8  extension[128];
};

struct PArchiveEntry
{
    puint32 byteOffset;
    puint32 fileSize;
    PString filePath;
};

class P_DLLEXPORT PArchiveFile
{
public:
    PArchiveFile();
    ~PArchiveFile();

    // Load an Future Interface's archive (.par) into memory.
    pbool load(const pchar *filePath);

    // The number of files in archive.
    puint32 getNumberOfFiles() const;
    // List all file names in this archive.
    void getFilenames(PArray<PString> &out_filenames) const;
    // Create an input stream from the archive. If failed, try to create the input stream
    // from the external storage using the relative path.
    pbool createInputStream(const pchar *filePath, PInputStream *out_inputStream);
    // Check if the file is in archive.
    pbool isPathInArchive(const pchar *filePath) const;

private:
    const PArchiveEntry* findEntry(const pchar *filePath) const;
    pbool readArchiveHeader(PAsset *asset);
    puint32 toUint32(puint8 *quadruple) const;

private:
    PString                m_archivePath; // to store the name of the archive.
    PArchiveHeader         m_header;
    // TODO: use sorted array.
    PArray<PArchiveEntry>  m_entries;
};

#endif // !PARCHIVEFILE_H
