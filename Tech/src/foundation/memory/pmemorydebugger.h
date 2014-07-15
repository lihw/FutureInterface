// pmemorydebugger.h
// The header file of memory debugger
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//
// 
#ifndef PMEMORYDEBUGGER_H
#define PMEMORYDEBUGGER_H

#include <PFoundation/pglobal.h>

#if P_ENABLE_MEMORY_DEBUGGING == 1

struct PMemoryChunkHeader
{
    puint32             guard;   
    puint32             bytes;      // the size of chunk excluding header and footer.
    const pchar        *file;
    puint32             line;
    pbool               isArray;
    puint32             uid;
    PMemoryChunkHeader *next; // ditto.
    PMemoryChunkHeader *previous; // ditto.
};

struct PMemoryChunkFooter
{
    puint32 bytes;
    puint32 guard;
};

struct PMemoryChunkState
{
    void initialize()
    {
        totalBytes                  = 0; // Used in memory pool.
        usedBytes                   = 0;
        largestChunkBytes           = 0;
        smallestChunkBytes          = puint32(-1);
        averageChunkBytes           = 0;
        numChunks                   = 0;
        numAllocationsPerFrame      = 0;
        numAllocationsLastFrame     = 0;
        numAllocations              = 0;
        numAllocationsTillLastFrame = 0;
        numFrames                   = 0;
    }
    
    void alloc(puint32 bytes)
    {
        usedBytes += bytes;
        if (largestChunkBytes < bytes)
        {
            largestChunkBytes = bytes;
        }
        if (smallestChunkBytes > bytes)
        {
            smallestChunkBytes = bytes;
        }
        averageChunkBytes = usedBytes / (numChunks + 1);
        numChunks++;
        numAllocations++;
    }

    void free(puint32 bytes)
    {
        usedBytes -= bytes;
        numChunks--;
        if (numChunks > 0)
        {
            averageChunkBytes = usedBytes / numChunks;
        }
        else
        {
            averageChunkBytes = 0;
        }
    }

    void update(pfloat32 deltaTime)
    {
        numFrames++;

        numAllocationsLastFrame = numAllocations - numAllocationsTillLastFrame;
        numAllocationsTillLastFrame = numAllocations;

        numAllocationsPerFrame = numAllocations / numFrames;

        // Reset every 60 frames to reflect the recent memory use states and
        // avoid integer overflow.
        if (numFrames == 60)
        {
            numAllocations = 0;
            numAllocationsTillLastFrame = 0;
            numFrames = 0;
        }
    }

    puint32 totalBytes;
    puint32 usedBytes;
    puint32 largestChunkBytes;
    puint32 smallestChunkBytes;
    puint32 averageChunkBytes;
    puint32 numChunks;                  // The number of memory chunks right now.
    puint32 numAllocationsPerFrame;     // The averaged malloc() times per frame
    puint32 numAllocationsLastFrame;    // The times of malloc() in last frame.

    puint32 numAllocations;
    puint32 numAllocationsTillLastFrame;
    puint32 numFrames;
};

class PMemoryDebugger
{
    PMemoryDebugger();
    ~PMemoryDebugger();

    PMemoryDebugger(const PMemoryDebugger &) {}
    void operator=(const PMemoryDebugger &) {}
    
public:
    static PMemoryDebugger *getInstance();
    static void create();
    static void destroy();
    static void update(pfloat32 deltaTime);
    static void report();
    
    void *malloc(puint32 size, const pchar *file, const pint32 line, pbool isArray = false);
    void free(void *ptr, pbool isArray = false);

private:
    void initialize();
    
public:
    P_INLINE puint32 getTotalMemoryBytes() const 
    { return m_state.totalBytes; };
    P_INLINE puint32 getUsedMemoryBytes() const 
    { return m_state.usedBytes; };
    P_INLINE puint32 getLargestMemoryChunkBytes() const 
    { return m_state.largestChunkBytes; };
    P_INLINE puint32 getSmallestMemoryChunkBytes() const 
    { return m_state.smallestChunkBytes; };
    P_INLINE puint32 getAverageMemoryChunkBytes() const 
    { return m_state.averageChunkBytes; };
    P_INLINE puint32 getNumberOfMemoryChunks() const 
    { return m_state.numChunks; };
    P_INLINE puint32 getNumberOfMemoryAllocationsPerFrame() const 
    { return m_state.numAllocationsPerFrame; };
    P_INLINE puint32 getNumberOfMemoryAllocationsLastFrame() const 
    { return m_state.numAllocationsLastFrame; };

private:
    static PMemoryDebugger  *s_debugger;

    PMemoryChunkHeader      *m_head;
    PMemoryChunkHeader      *m_tail;
    PMemoryChunkState        m_state;
    puint32                 uid;

    static const puint32    HEADER_GUARD;
    static const puint32    FOOTER_GUARD;
};

#else // else of P_ENABLE_MEMORY_DEBUGGING

class PMemoryDebugger
{
public:
    static PMemoryDebugger *getInstance() { return P_NULL; };
    static void create() {}
    static void update(pfloat32 deltaTime) {};
    static void report() {};
    static void destroy() {};
};

#endif // P_ENABLE_MEMORY_DEBUGGING == 0

#endif // !PMEMORYDEBUGGER_H
