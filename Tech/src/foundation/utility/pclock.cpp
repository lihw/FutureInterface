// pclock.cpp
// The wall clock that controls the global ticking.
//
// Hongwei Li lihw81@gmail.com
//

#include "pclock.h"

#include <PFoundation/ptime.h>
#include <PFoundation/passert.h>
#include <PFoundation/plog.h>
#include <PFoundation/pcrt.h>


// TODO: make this define as an member value of global engine configuration.
#define P_MILLISECONDS_PER_FRAME 33

PClock::PClock()
{
    m_timestamp            = 0; // The timestamp from booting of this frame.
    m_deltaTime            = 0;
    m_currentTime          = 0;
    m_systemDeltaTime      = 0;
    m_systemCurrentTime    = 0;
    m_maxFps               = 0; // FPS control is disabled by default
    m_minMSPerFrame        = 1000.0f / 30.0f;
    m_elapsedFrames        = 0;
    m_playedFrames         = 0;
    m_scaling              = 1.0f;
    m_paused               = false;
    m_lastSleepingTime     = 0;
}

PClock::~PClock()
{
}

void PClock::tick()
{
    puint32 thisFrameTime = pTimeGetCurrentTimestamp();
    puint32 deltaTime = thisFrameTime - m_timestamp;
    deltaTime = deltaTime < 1? 1 : deltaTime; // at least 1 ms
    m_timestamp = thisFrameTime;
    
    if (!m_paused)
    {
    #if defined (P_WIN32) && defined (P_DEBUG)
        if (!isFpsControlEnabled() && deltaTime > P_MILLISECONDS_PER_FRAME)
        {
            // The application is in the debug mode and we are stepping through
            // the application, so the interval between two frames might be 
            // abnormally large. In this case, we need to trim the large delta
            // time to normal one for 
            deltaTime = P_MILLISECONDS_PER_FRAME;
        }
    #endif

        // We use the approximate average of last n frames to predicate the delta time
        // for next frame.
        // The larger n is, the faster animation will be after resume in a short while.
        const puint32 n = 4;
        pfloat32 elapsedNumFrames = (pfloat32)pMin(m_playedFrames, n - 1);
        m_systemDeltaTime = (m_systemDeltaTime * elapsedNumFrames + (pfloat32)deltaTime) / 
            (elapsedNumFrames + 1.0f);

        m_systemCurrentTime += m_systemDeltaTime;
        // The awake time is the real time invested in rendering and computation.
        pfloat32 awakeTime = m_systemDeltaTime - m_lastSleepingTime;
        
        if (isFpsControlEnabled() && awakeTime < m_minMSPerFrame)
        {
            // Note that the FPS control is upon the system frame/delta time
            // rather than the scaled one. The former is a measurement of frame
            // rate while the latter is that of the application/game speed.
            pfloat32 sleepingTime = m_minMSPerFrame - awakeTime;
            m_lastSleepingTime = sleepingTime;
            psleep(sleepingTime);
        }

        m_deltaTime = m_systemDeltaTime * m_scaling;

        m_currentTime += m_deltaTime;

        m_elapsedFrames++;
        m_playedFrames++;
    }
}

void PClock::pause()
{
    m_paused = true;
    m_deltaTime = 0;
    m_systemDeltaTime = 0;
}

void PClock::resume()
{
    m_paused = false;
    m_playedFrames = 0;
    m_timestamp = pTimeGetCurrentTimestamp();
    m_lastSleepingTime = 0;
}
    
void PClock::setMaxFps(puint32 fps)
{
    m_maxFps = fps;
    if (m_maxFps > 0)
    {
        m_minMSPerFrame = 1000.0f / (pfloat32)fps;
    }
}
