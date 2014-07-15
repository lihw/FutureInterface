// ptimeline.h 
// The time line 
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PTIMELINE_H
#define PTIMELINE_H

#include <PFoundation/pglobal.h>


class P_DLLEXPORT PTimeLine
{
public:
    PTimeLine();
    ~PTimeLine();

    void setScaling(pfloat32 scaling);
    void pause();
    void resume();
    void reset();
    void seek(pfloat32 time);

    // Update the timeline with delta time from
    // global clock.
    void update(pfloat32 deltaTime);

    // return the elapsed time in milliseconds
    P_INLINE pfloat32 getElapsedTime() const
    { return m_elapsedTime; }
    // return the delta time in milliseconds
    P_INLINE pfloat32 getDeltaTime() const
    { return m_deltaTime; }
    P_INLINE pbool isPaused() const
    { return m_paused; }
    P_INLINE pfloat32 getScaling() const 
    { return m_scaling; }


protected:
    pbool     m_paused; 
    pfloat32 m_scaling;
    pfloat32 m_elapsedTime;
    pfloat32 m_deltaTime;
};


#endif // !PTIMELINE_H
