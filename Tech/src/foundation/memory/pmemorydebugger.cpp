// pmemorydebugger.cpp
// The memory debugger.
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//
// 
#include "pmemorydebugger.h"

#include <PFoundation/passert.h>
#include <PFoundation/pcrt.h>
#include <PFoundation/pdebugutility.h>

#if P_ENABLE_MEMORY_DEBUGGING == 1

PMemoryDebugger *PMemoryDebugger::s_debugger = P_NULL;

const puint32 PMemoryDebugger::HEADER_GUARD = 0xDEADBEEF;
const puint32 PMemoryDebugger::FOOTER_GUARD = 0xBADDCAFE;

PMemoryDebugger *PMemoryDebugger::getInstance()
{
    return PMemoryDebugger::s_debugger;
}

void PMemoryDebugger::create()
{
    if (!PMemoryDebugger::s_debugger)
    {
        // Notice: Can not use pnew since we will call getInstance() in pDebugMalloc().
        PMemoryDebugger::s_debugger = (PMemoryDebugger *)pmalloc(sizeof(PMemoryDebugger));
        PMemoryDebugger::s_debugger->initialize();
    }    
}

void PMemoryDebugger::destroy()
{
    PMemoryDebugger::report();
    
    if (PMemoryDebugger::s_debugger)
    {
        pfree(PMemoryDebugger::s_debugger->m_head);
        pfree(PMemoryDebugger::s_debugger->m_tail);

        pfree(PMemoryDebugger::s_debugger);
        PMemoryDebugger::s_debugger = P_NULL;
    }
}

void PMemoryDebugger::update(pfloat32 deltaTime)
{
    if (PMemoryDebugger::s_debugger)
    {
        PMemoryDebugger::s_debugger->m_state.update(deltaTime);
    }
}

void PMemoryDebugger::report()
{
    if (PMemoryDebugger::s_debugger)
    {
        if (PMemoryDebugger::s_debugger->m_head->next != PMemoryDebugger::s_debugger->m_tail)
        {
            pchar msg[3000]; 
            pDebugOutputString("Memory debugger detected memory leaks!\n"
                               "Dumping chunks ->");
            
            PMemoryChunkHeader *node = PMemoryDebugger::s_debugger->m_head->next;
            while (node != PMemoryDebugger::s_debugger->m_tail)
            {
                puint8 *data = (puint8 *)(node + 1);
                if (node->isArray)
                {
                    psprintf(msg, 3000, "{%d} array at 0x08%X, %d bytes long.", 
                        node->uid, data, node->bytes);
                }
                else
                {
                    psprintf(msg, 3000, "{%d} block at 0x08%X, %d bytes long.", 
                        node->uid, data, node->bytes);
                }
                pDebugOutputString(msg);
                psprintf(msg, 3000, "allocated in %s, %d (uid=%d).", node->file, node->line, node->uid);
                pDebugOutputString(msg);
                psprintf(msg, 3000, "content (first 16 bytes): %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", 
                    data[0], data[1], data[2], data[3], 
                    data[4], data[5], data[6], data[7], 
                    data[8], data[9], data[10], data[11], 
                    data[12], data[13], data[14], data[15]);
                pDebugOutputString(msg);
                node = node->next;
            }
            
            pDebugOutputString("Chunks dump complete.");
        }
    }
}

PMemoryDebugger::PMemoryDebugger()
{
}

PMemoryDebugger::~PMemoryDebugger()
{
    PASSERT(PMemoryDebugger::s_debugger == P_NULL);
}

void PMemoryDebugger::initialize()
{
    // Two phony allocation nodes for efficient list manipulation.
    m_head = (PMemoryChunkHeader *)pmalloc(sizeof(PMemoryChunkHeader));
    m_tail = (PMemoryChunkHeader *)pmalloc(sizeof(PMemoryChunkHeader));

    m_head->previous = P_NULL;
    m_head->next     = m_tail;
    m_tail->previous = m_head;
    m_tail->next     = P_NULL;

    uid = 0;

    m_state.initialize();
}

void *PMemoryDebugger::malloc(puint32 size, const pchar *file, const pint32 line, pbool isArray)
{
    if (size == 0)
    {
        return P_NULL;
    }

    puint32 realSize = size + sizeof(PMemoryChunkHeader) + sizeof(PMemoryChunkFooter);
    PMemoryChunkHeader *chunk = (PMemoryChunkHeader *)pmalloc(realSize);

    if (chunk != P_NULL)
    {
        // Fill chunk header.
        chunk->bytes   = size;
        chunk->file    = file;
        chunk->line    = line;
        chunk->guard   = HEADER_GUARD;
        chunk->isArray = isArray;
        chunk->uid     = uid++;
        chunk->next    = P_NULL;

        // Fill chunk footer.
        PMemoryChunkFooter *footer;
        footer = (PMemoryChunkFooter *)(((pchar *)chunk) + sizeof(PMemoryChunkHeader) + size);
        footer->bytes = size;
        footer->guard = FOOTER_GUARD;

        // Add to chunk list.
        chunk->previous        = m_head;
        chunk->next            = m_head->next;
        m_head->next->previous = chunk;
        m_head->next           = chunk;
        
        // Sync state.
        m_state.alloc(size);
    }

    return (void *)(chunk + 1);
}

void PMemoryDebugger::free(void *ptr, pbool isArray)
{
    if (!ptr)
    {
        return;
    }

    PMemoryChunkHeader *chunk = (PMemoryChunkHeader *)ptr;
    chunk = chunk - 1;

    // Check the chunk.
    PASSERT(chunk->guard == HEADER_GUARD);
    PASSERT(chunk->isArray == isArray);

    if (chunk->guard == HEADER_GUARD)
    {
        PMemoryChunkFooter *footer;
        footer = (PMemoryChunkFooter *)(((pchar *)chunk) + sizeof(PMemoryChunkHeader) + chunk->bytes);
        PASSERT(footer->guard == FOOTER_GUARD);

        // Remove from chunk list.
        chunk->previous->next = chunk->next;
        chunk->next->previous = chunk->previous;

        // Sync state.
        m_state.free(chunk->bytes);
        
        // Free memory.    
        pfree(chunk);
    }
    else
    {
        // Not a chunk, just free it.
        pfree(ptr);
    }
}

#endif // P_ENABLE_MEMORY_DEBUGGING == 1
