// pdevice.h
// Each PContext has one device instance which collects the current display
// screen's status and manges the input events.
//
// Copyright 2012 - 2014 Future Interface Li. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com


#ifndef PDEVICE_H
#define PDEVICE_H

#include <PFoundation/pinput.h>

enum PDeviceStateEnum
{
    P_DEVICE_STATE_UNINITIALIZED,
    P_DEVICE_STATE_ACTIVE,
    P_DEVICE_STATE_INACTIVE,
    P_DEVICE_STATE_CLOSED,

    P_DEVICE_STATE_FIRST = P_DEVICE_STATE_UNINITIALIZED,
    P_DEVICE_STATE_LAST = P_DEVICE_STATE_CLOSED,
};

enum PDeviceOrientationEnum
{
    P_DEVICE_ORIENTATION_PORTRAIT,
    P_DEVICE_ORIENTATION_LANDSCAPE,

    P_DEVICE_ORIENTATION_FIRST = P_DEVICE_ORIENTATION_PORTRAIT,
    P_DEVICE_ORIENTATION_LAST = P_DEVICE_ORIENTATION_LANDSCAPE
};

enum PDeviceOrientationAngleEnum
{
    P_DEVICE_ORIENTATION_ANGLE_0,
    P_DEVICE_ORIENTATION_ANGLE_180,
};


class PContext;


class PDevice
{
public:
    PDevice(PContext* context);
    virtual ~PDevice();

    pbool onClose();
    void onFocusGained();
    void onFocusLost();
    void onResized(puint32 width, puint32 height);
    void onOrientationChanged(PDeviceOrientationEnum orientation);
    void onOrientationAngleChanged(PDeviceOrientationAngleEnum angle);

    P_INLINE PDeviceStateEnum getState() const { return m_state; }
    P_INLINE PContext* getContext() const { return m_context; }
    P_INLINE PDeviceOrientationAngleEnum getOrientationAngle() const { return m_orientationAngle; }
    P_INLINE PDeviceOrientationEnum getOrientation() const { return m_orientation; }
    P_INLINE PInputEventQueue* getInputEventQueue() const { return m_inputEventQueue; }
    P_INLINE PInputKeyDeviceStateEnum getKeyState(PInputKey key) const 
    { PASSERTINFO(key < P_KEY_COUNT, "out of key range"); return m_keyStates[key]; }
    P_INLINE puint32 getScreenWidth() const { return m_screenWidth; }
    P_INLINE puint32 getScreenHeight() const { return m_screenHeight; }

    void setState(PDeviceStateEnum state);
    void setKeyState(PInputKey key, PInputKeyDeviceStateEnum state);

    // To port this engine to other platform, developers should override the following four
    // functions for that specific platform.
    virtual void update();

private:
    PContext*                    m_context;
    PDeviceStateEnum             m_state;
    PDeviceOrientationEnum       m_orientation;
    PDeviceOrientationAngleEnum  m_orientationAngle;
    PInputEventQueue*            m_inputEventQueue;
    PInputKeyDeviceStateEnum     m_keyStates[P_KEY_COUNT];
    puint32                      m_screenWidth;
    puint32                      m_screenHeight;
};

#endif // !PDEVICE_H
