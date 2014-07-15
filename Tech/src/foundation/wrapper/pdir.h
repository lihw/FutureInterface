// pdir.h
// List the content of the directory
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PDIR_H
#define PDIR_H

#include <PFoundation/pcrt.h>
#include <PFoundation/passert.h>
#include <PFoundation/pnew.h>

enum PDirItemTypeEnum
{
    P_DIR_ITEM_DIRECTORY,
    P_DIR_ITEM_FILE,
    P_DIR_ITEM_UNKNOWN,
};

class P_DLLEXPORT PDirItem
{
public:
    PDirItem(PDirItemTypeEnum type, const pchar *path)
        : m_type(type)
    {
        PASSERT(m_type != P_DIR_ITEM_UNKNOWN);
        m_path = pstrdup(path);
    }

    ~PDirItem()
    {
        PDELETEARRAY(m_path);
    };

    P_INLINE pbool isFile() const      { return m_type == P_DIR_ITEM_FILE; }
    P_INLINE pbool isDirectory() const { return m_type == P_DIR_ITEM_DIRECTORY; }
    P_INLINE const pchar *path() const { return m_path; }

private:
    PDirItemTypeEnum m_type;
    pchar*           m_path;
};

class P_DLLEXPORT PDir
{
public:
    PDir(const pchar *dir);
    ~PDir();

    P_INLINE puint32 numberOfItems() const { return m_numberOfItems; }
    P_INLINE PDirItem* item(puint32 i) const { PASSERT(i < m_numberOfItems); return &m_items[i]; }
    P_INLINE pbool exists() const { return m_exists; }

    // Only create the final directory. It returns false when either the 
    // directory exists or the intermediate directory in the path is missing.
    static pbool makeDirectory(const pchar *path);

private:
    void resizeItemArray(puint8 *&itemData, puint32 oldSize, puint32 newSize);

private:
    pbool m_exists;
    PDirItem *m_items;
    puint32 m_numberOfItems;
};


#endif // !P_DIR_H
