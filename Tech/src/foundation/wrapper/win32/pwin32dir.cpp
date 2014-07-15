// pwin32dir.cpp
// List the content of the directory on win32.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "../pdir.h"

#include <PFoundation/plog.h>

#include <windows.h>
#include <direct.h>

#include <stdio.h>

PDir::PDir(const pchar *dir)
{
    // Check if the directory exists.
    DWORD dwAttrib = GetFileAttributesA(dir);
    m_exists = ((dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    
    // Fetch the item names in that directory.
    WIN32_FIND_DATAA fdFile;
    HANDLE hFind = NULL;

    pchar path[MAX_PATH];

    m_items = P_NULL;
    m_numberOfItems = 0;

    puint32 capacity = 32;
    puint8 *itemData = PNEWARRAY(puint8, capacity * sizeof(PDirItem));
    pmemset(itemData, 0, sizeof(capacity * sizeof(PDirItem)));

    // Specify a file mask. *.* = We want everything!
    psprintf(path, MAX_PATH, "%s\\*.*", dir);

    if ((hFind = FindFirstFileA(path, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        PLOG_ERROR("Failed to read directory %s.", dir);
        PDELETEARRAY(itemData);
    }
    else
    {
        do
        {
            // Find first file will always return "." and ".." 
            // as the first two directories.
            if (pstrcmp(fdFile.cFileName, ".") != 0 && 
                pstrcmp(fdFile.cFileName, "..") != 0)
            {
                //Build up our file path using the passed in
                //  [sDir] and the file/foldername we just found:
                psprintf(path, MAX_PATH, "%s\\%s", dir, fdFile.cFileName);

                //Is the entity a File or Folder?
                if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    // TODO: use the absolute path.
                    new (itemData + m_numberOfItems * sizeof(PDirItem)) PDirItem(P_DIR_ITEM_DIRECTORY, path);
                    m_numberOfItems++;
                }
                else if ((fdFile.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) ||
                         (fdFile.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE))
                {
                    // TODO: use the absolute path.
                    new (itemData + m_numberOfItems * sizeof(PDirItem)) PDirItem(P_DIR_ITEM_FILE, path);
                    m_numberOfItems++;
                }
                else
                {
                    PLOG_WARNING("%s is not seen supported.", path);
                }

                if (m_numberOfItems > capacity)
                {
                    resizeItemArray(itemData, capacity, 2 * capacity);
                    capacity *= 2;
                }
            }
        }
        while (FindNextFileA(hFind, &fdFile)); // Find the next file.

        FindClose(hFind); //Always, Always, clean things up!
    }

    m_items = reinterpret_cast<PDirItem*>(itemData);
}

PDir::~PDir()
{
    for (puint32 i = 0; i < m_numberOfItems; ++i)
    {
        m_items[i].~PDirItem();
    }

    puint8* itemData = reinterpret_cast<puint8*>(m_items);
    PDELETEARRAY(itemData);
}

void PDir::resizeItemArray(puint8*& itemData, puint32 oldSize, puint32 newSize)
{
    puint8* newItemData = PNEWARRAY(puint8, newSize * sizeof(PDirItem));
    for (puint32 i = 0; i < oldSize; ++i)
    {
        new (newItemData + i * sizeof(PDirItem)) 
            PDirItem(m_items[i].isFile()? P_DIR_ITEM_FILE : P_DIR_ITEM_DIRECTORY,
                     m_items[i].path());
    }

    PDELETEARRAY(itemData);
    itemData = newItemData;
}

pbool PDir::makeDirectory(const pchar *path)
{
    if (!CreateDirectoryA(path, NULL))
    {
        DWORD err = GetLastError();
        if (err == ERROR_ALREADY_EXISTS)
        {
            PLOG_ERROR("Directory (%s) already exists.", path);
        }
        else if (err == ERROR_PATH_NOT_FOUND)
        {
            PLOG_ERROR("Some intermediate directories in the path (%s) are missing.", path);
        }
        
        return false;
    }

    return true;
}
