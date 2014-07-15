// peventtype.h
// The event type
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//
// 


#ifndef PEVENTTYPE_H
#define PEVENTTYPE_H

enum PEventTypeEnum
{
    P_EVENT__KEYDOWN,
    P_EVENT__KEYUP,
    P_EVENT__KEYREPEAT,
    
    P_EVENT__TOUCH_DOWN,
    P_EVENT__TOUCH_STATIONARY,
    P_EVENT__TOUCH_MOVE,
    P_EVENT__TOUCH_UP,

    P_EVENT__ANIMATION_PAUSED,
    P_EVENT__ANIMATION_PLAYING,
    P_EVENT__ANIMATION_STOPPED,
    P_EVENT__ANIMATION_REWOUND,

    P_EVENT__TAP,
    P_EVENT__PAN_BEGIN,
    P_EVENT__PAN,
    P_EVENT__PAN_END,
    P_EVENT__PINCH_BEGIN,
    P_EVENT__PINCH,
    P_EVENT__PINCH_END,
    P_EVENT__FLING,
    P_EVENT__LONG_PRESS,

    P_EVENT__TIMER_EXPIRED,
};


#endif // !PEVENTTYPE_H
