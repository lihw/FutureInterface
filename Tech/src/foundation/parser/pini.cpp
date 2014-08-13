// pini.cpp
// The ini parser wrapper
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "pini.h"

#include <PFoundation/ppath.h>

#include "ini_private.h"

static int iniHandler_internal(void *user, const char *section, const char *name,
    const char *value)
{
    PIni *ini = (PIni *)user;

    ini->addEntry(name, value);

    return 1;
}

PIni::PIni(const pchar *filename)
{
    pbool ret = load(filename);
    PASSERTINFO(ret, "Failed to load ini file.");
}

PIni::~PIni()
{
}

const PString &PIni::entry(const PString &name) const
{
    PMap<PString, PString>::const_iterator it = m_entries.find(name);
    if (it != m_entries.end())
    {
        return it.value();
    }

    PLOG_WARNING("Failed to find entry called %s.", name.c_str());
    return m_nullString;
}

const PString &PIni::entry(const pchar *name) const
{
    return entry(PString(name));
}
    
void PIni::addEntry(const pchar *name, const pchar *value)
{
    m_entries.insert(PString(name), PString(value));
}
    
pbool PIni::load(const pchar *filename)
{
    //
    // Look for the file in the specified path
    //
    const pchar* pathList[] =
    {
        ".",
        pPathGetApplicationDirectory(),
        pPathGetExternalStoragePath(),
    };

    for (size_t i = 0; i < sizeof(pathList) / sizeof(pathList[0]); ++i)
    {
        if (pathList[i] != P_NULL)
        {
            PString cfgPath = PString(pathList[i]) + PString(pPathGetDelimiter()) + PString(filename);
            PFile *fp = pfopen(cfgPath.c_str(), "rb");
            if (fp != P_NULL)
            {
                if (ini_parse_file((FILE *)(fp), iniHandler_internal, this) < 0)
                {
                    PLOG_ERROR("Failed to parse %s", cfgPath.c_str());
                    pfclose(fp);
                    return false;
                }
                pfclose(fp);

                return true;
            }
        }
    }

    //
    // Look for the ini file in asset
    //
    PAsset asset = pAssetOpen(filename);
    if (pAssetIsValid(&asset))
    {
        PIniBufferObject iniBufferObject;
        iniBufferObject.m_buffer = (const pchar*)pAssetGetBuffer(&asset);
        iniBufferObject.m_bufferSize = pAssetGetSize(&asset);
        iniBufferObject.m_position = 0;
        
        if (ini_parse_buffer(&iniBufferObject, iniHandler_internal, this) < 0)
        {
            PLOG_ERROR("Failed to parse %s", filename);
            pAssetClose(&asset);
            return false;
        }

        pAssetClose(&asset);
        
        return true;
    }

    PLOG_ERROR("Failed to find %s", filename);
    
    return false;
}
