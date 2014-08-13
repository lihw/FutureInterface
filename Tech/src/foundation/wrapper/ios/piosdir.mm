// piosdir.mm
// List the content of the directory on iOS
// 
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "../pdir.h"

#include <PFoundation/pmemory_module.h>
#include <PFoundation/plog.h>
#include <PFoundation/ppath.h>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

PDir::PDir(const pchar *dir)
{
    // Check if the directory exists.
    struct stat s;
    if (stat(dir, &s) != 0 || !S_ISDIR(s.st_mode))
    {
        PLOG_ERROR("%s is not a valid directory.", dir);
        m_exists =  false;
    }
    else
    {
        m_exists = true;
    
        // Fetch the item names in that directory.
        DIR* dirObject;
        dirent* dirEntry;

        m_items = P_NULL;
        m_numberOfItems = 0;

        puint32 capacity = 32;
        puint8* itemData = PNEWARRAY(puint8, capacity * sizeof(PDirItem));
        pmemset(itemData, 0, sizeof(capacity * sizeof(PDirItem)));

        if ((dirObject = opendir(dir)) == NULL)
        {
            PLOG_ERROR("Failed to read directory %s.", dir);
            PDELETEARRAY(itemData);
        }
        else
        {
            char path[4096];
            while ((dirEntry = readdir(dirObject)) != NULL)
            {
                // Find first file will always return "." and ".." 
                // as the first two directories.
                if (pstrcmp(dirEntry->d_name, ".") != 0 && 
                    pstrcmp(dirEntry->d_name, "..") != 0)
                {
                    //Build up our file path using the passed in
                    //  [sDir] and the file/foldername we just found:
                    psprintf(path, 4096, "%s\\%s", dir, dirEntry->d_name);

                    //Is the entity a File or Folder?
                    if (dirEntry->d_type == DT_DIR)
                    {
                        // TODO: use the absolute path.
                        new (itemData + m_numberOfItems * sizeof(PDirItem)) PDirItem(P_DIR_ITEM_DIRECTORY, path);
                        m_numberOfItems++;
                    }
                    else if (dirEntry->d_type == DT_REG)
                    {
                        // TODO: use the absolute path.
                        new (itemData + m_numberOfItems * sizeof(PDirItem)) PDirItem(P_DIR_ITEM_FILE, path);
                        m_numberOfItems++;
                    }
                    else
                    {
                        PLOG_WARNING("%s is not supported.", path);
                    }

                    if (m_numberOfItems > capacity)
                    {
                        resizeItemArray(itemData, capacity, 2 * capacity);
                        capacity *= 2;
                    }
                }
            }

            closedir(dirObject); //Always, Always, clean things up!
        
            m_items = reinterpret_cast<PDirItem*>(itemData);
        }
    }
}

PDir::~PDir()
{
    for (puint32 i = 0; i < m_numberOfItems; ++i)
    {
        m_items[i].~PDirItem();
    }

    puint8 *itemData = reinterpret_cast<puint8*>(m_items);
    PDELETEARRAY(itemData);
}

void PDir::resizeItemArray(puint8 *&itemData, puint32 oldSize, puint32 newSize)
{
    puint8 *newItemData = PNEWARRAY(puint8, newSize * sizeof(PDirItem));
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
    if (!mkdir(path, S_IRWXU | S_IRGRP | S_IXGRP))
    {
        if (errno == EEXIST)
        {
            PLOG_ERROR("directory (%s) already exists.", path);
        }
        else if (errno == ENOENT)
        {
            PLOG_ERROR("some intermediate directories in the path (%s) are missing.", path);
        }
        
        return false;
    }

    return true;
}

