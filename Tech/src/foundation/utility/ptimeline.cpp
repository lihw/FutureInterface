// ptimeline.cpp
// The time line 
//
// Hongwei Li lihw81@gmail.com
//

#include "ptimeline.h" 

PTimeLine::PTimeLine()
{
    m_paused      = false;
    m_scaling     = 1.0f;
    m_elapsedTime = 0.0f;
    m_deltaTime   = 0.0f;
}

PTimeLine::~PTimeLine()
{
}

void PTimeLine::setScaling(pfloat32 scaling)
{
    m_scaling = scaling;
}

void PTimeLine::pause()
{
    m_paused = true;
}

void PTimeLine::resume()
{
    m_paused = false;
}

void PTimeLine::reset()
{
    m_elapsedTime = 0.0f;
    m_deltaTime = 0.0f;
    m_paused = false;
}

void PTimeLine::seek(pfloat32 time)
{
    m_elapsedTime = time * m_scaling;
}

void PTimeLine::update(pfloat32 deltaTime)
{
    if (!m_paused)
    {
        m_deltaTime = deltaTime * m_scaling;
        m_elapsedTime += m_deltaTime;
    }
}
