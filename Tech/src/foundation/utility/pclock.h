// pclock.h 
// The wall clock that controls the global ticking.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PCLOCK_H
#define PCLOCK_H

#include <PFoundation/pglobal.h>


class P_DLLEXPORT PClock 
{
public:
    PClock();
    ~PClock();

    P_INLINE pfloat32 currentTime() const { return m_currentTime; }
    P_INLINE pfloat32 deltaTime() const { return m_deltaTime; }
    P_INLINE pfloat32 systemCurrentTime() const { return m_systemDeltaTime; }
    P_INLINE pfloat32 systemDeltaTime() const { return m_systemDeltaTime; }
    P_INLINE pbool isFpsControlEnabled() const { return m_maxFps > 0; }
    P_INLINE puint32 maximumFps() const { return m_maxFps; }
    P_INLINE pfloat32 scaling() const { return m_scaling; }
    P_INLINE pbool isPaused() const { return m_paused; }
    P_INLINE pfloat32 fps() const { return 1000.0f / m_systemDeltaTime; }
    P_INLINE puint32 timestamp() const { return m_timestamp; }

    // Tick will be called once every frame.
    void tick();

    void pause();
    void resume();
    void setScaling(pfloat32 scaling);
    void setMaxFps(puint32 fps);

private:
    pfloat32    m_currentTime;   // The current clock time.
    pfloat32    m_deltaTime;     // The elapsed time since last frame.
    pfloat32    m_systemCurrentTime; // The elapsed time in milliseconds without scaling.
    pfloat32    m_systemDeltaTime; // The delta time between consecutive frames without scaling.
    puint32     m_elapsedFrames; // The elapsed frames since the application's start.
    puint32     m_playedFrames; // The elapsed frames since last pause.
    puint32     m_maxFps; // Maximum FPS.
    pfloat32    m_minMSPerFrame; // Minimum millisecond per frame
    pfloat32    m_scaling;
    pbool        m_paused;
    puint32     m_timestamp; // The timestamp of this frame since system boots
    pfloat32    m_lastSleepingTime; // How many milliseconds we slept over in last frame.
};


#endif // !PCLOCK_H

