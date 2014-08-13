// pgesture_private.h
// The gesture base header file.
//
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#ifndef PGESTURE_PRIVATE_H
#define PGESTURE_PRIVATE_H

#include <PFoundation/pevent.h>

class PContext;


class P_DLLEXPORT PAbstractGesture
{
public:
    PAbstractGesture(PGestureManager *manager, PGestureTypeEnum type);
    virtual ~PAbstractGesture();

    virtual void touchDown(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer) = 0;
    virtual void touchMove(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer) = 0;
    virtual void touchUp(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer) = 0;
    virtual void reset() = 0;

    virtual void update(pfloat32 deltaTime) {};

    P_INLINE PGestureTypeEnum getGestureType() const { return m_type; }

    void setEnabled(pbool enabled);
    P_INLINE pbool isEnabled() const { return m_enabled; }

protected:
    PEvent *createEvent(PEventTypeEnum type);

    enum PGestureStateEnum
    {
        STATE_READY,
        STATE_POSSIBLE,
        STATE_MOVING,
        STATE_FAILURE,

        STATE_FIRST = STATE_READY,
        STATE_LAST = STATE_FAILURE,
        STATE_NUMBER = STATE_LAST - STATE_FIRST + 1,
    };

protected:
    PGestureStateEnum m_state;
    PGestureManager *m_manager;
    PGestureTypeEnum m_type;
    pbool            m_enabled;
};

class P_DLLEXPORT PGesturePan : public PAbstractGesture
{
public:
    PGesturePan(PGestureManager* manager, pfloat32 distanceThreshold = 5.0f);
    virtual ~PGesturePan();
    virtual void touchDown(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);
    virtual void touchMove(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);
    virtual void touchUp(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);
    virtual void reset();

    void setDistanceThreshold(pfloat32 value);
    
    P_INLINE pfloat32 getDistanceThreshold() const
    { return m_distanceThreshold; }
    
private:
    pint32    m_x;
    pint32    m_y;
    pfloat32  m_distanceThreshold;
};

class P_DLLEXPORT PGesturePinch : public PAbstractGesture
{
public:
    PGesturePinch(PGestureManager* manager);
    virtual ~PGesturePinch();

    virtual void touchDown (pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);
    virtual void touchMove (pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);
    virtual void touchUp (pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);
    virtual void reset();

private:
    // TODO: use pVector2iLength()
    pfloat32 getDistance(const pint32 *firstPoint, const pint32 *secondPoint);
    pfloat32 getRadians(const pint32 *firstPoint, const pint32 *secondPoint);

private:
    pbool     m_pressDown[2];
    pint32    m_points[2][2];
    pint32    m_initialPoints[2][2];
    pfloat32  m_initialRadians;
    pfloat32  m_initialDistance;
};

class P_DLLEXPORT PGestureFling : public PAbstractGesture
{
public:
    // velocityThreshold's unit is pixels/second.
    PGestureFling(PGestureManager* manager, pfloat32 distanceThreshold = 20.0f,
        pfloat32 velocityThreshold = 50.0f);
    virtual ~PGestureFling();

    virtual void touchDown(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);
    virtual void touchMove(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);
    virtual void touchUp(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);
    virtual void reset();

    void setDistanceThreshold(pfloat32 value);
    void setVelocityThreshold(pfloat32 value);

    P_INLINE pfloat32 getDistanceThreshold() const
    { return m_distanceThreshold; }
    P_INLINE pfloat32 getVelocityThreshold() const
    { return m_velocityThreshold; }
    
private:
    pint32 m_initialPoint[2];
    puint32 m_initialTimestamp;

    pfloat32 m_velocityThreshold;
    pfloat32 m_distanceThreshold;
};

// PGestureTap constructor and setIntervalThreshold 
// parameter intervalThreshold unit is millisecond
class PGestureTap : public PAbstractGesture
{
public:
    PGestureTap(PGestureManager* manager, pfloat32 distanceThreshold = 15.0f, 
        puint32 intervalThreshold = 350);

    virtual ~PGestureTap();

    virtual void touchDown(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);
    virtual void touchMove(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);
    virtual void touchUp(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);
    virtual void reset();
    
    void setDistanceThreshold(pfloat32 value);
    void setIntervalThreshold(puint32 value);

    P_INLINE pfloat32 getDistanceThreshold() const
    { return m_distanceThreshold; }
    P_INLINE puint32 getIntervalThreshold() const
    { return m_intervalThreshold; }

private:
    puint32     m_firstTapTime;
    puint32     m_firstPoint[2];
    pfloat32    m_distanceThreshold;
    puint32     m_intervalThreshold;
    puint32     m_tapCount;
};

// PGestureLongPress constructor and setIntervalThreshold 
// parameter intervalThreshold unit is millisecond
class PGestureLongPress : public PAbstractGesture
{
public:
    PGestureLongPress(PGestureManager *manager, pfloat32 distanceThreshold = 15.0f,
        puint32 intervalThreshold = 500);
    virtual ~PGestureLongPress();
    virtual void touchDown(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);
    virtual void touchMove(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);
    virtual void touchUp(pint32 x, pint32 y, puint32 timeStamp, pint32 pointer);
    
    // FIXME: replace the update with a timer event.
    void update(pfloat32 deltaTime);

    virtual void reset();
    
    void setDistanceThreshold(pfloat32 value);
    void setIntervalThreshold(puint32 value);

    P_INLINE pfloat32 getDistanceThreshold() const
    { return m_distanceThreshold; }
    P_INLINE puint32 getIntervalThreshold() const
    { return m_intervalThreshold; }

private:
    pfloat32    m_elapsedTime;
    pfloat32    m_distanceThreshold;
    puint32     m_intervalThreshold;
    pint32      m_x;
    pint32      m_y;
};

#endif // !PGESTURE_PRIVATE_H
