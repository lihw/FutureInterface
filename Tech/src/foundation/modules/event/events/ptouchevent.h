// ptouchevent.h
// The touch pad event.
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Xujihao xujihao@gmail.com
//
// 


P_EVENTPARAMETER__TOUCH_CURSOR_ID     = 0,  // puint32
P_EVENTPARAMETER__TOUCH_X                ,  // pint32
P_EVENTPARAMETER__TOUCH_Y                ,  // pint32
P_EVENTPARAMETER__FLING_VELOCITY_X       ,  // pfloat32
P_EVENTPARAMETER__FLING_VELOCITY_Y       ,  // pfloat32
P_EVENTPARAMETER__PINCH_1ST_POINTER      ,  // pfloat32[2]  1st pointer position(x,y) 
P_EVENTPARAMETER__PINCH_2ND_POINTER      ,  // pfloat32[2]  2nd pointer position (x,y)
P_EVENTPARAMETER__PINCH_SCALING           ,  // pfloat32 
P_EVENTPARAMETER__PINCH_ROTATION_RADIANS   ,  // pfloat32
P_EVENTPARAMETER__PAN_DELTA_X            ,  // pfloat32
P_EVENTPARAMETER__PAN_DELTA_Y            ,  // pfloat32
P_EVENTPARAMETER__TAPCOUNT               ,  // pint32
P_EVENTPARAMETER__TOUCH_TIMESTAMP        ,  // puint32

P_EVENTPARAMETER__TOUCH_PARAMETER_FIRST = P_EVENTPARAMETER__TOUCH_CURSOR_ID,
P_EVENTPARAMETER__TOUCH_PARAMETER_LAST = P_EVENTPARAMETER__TOUCH_TIMESTAMP,

P_EVENTPARAMETER__TOUCH_PARAMETER_NUMBER = P_EVENTPARAMETER__TOUCH_PARAMETER_LAST - P_EVENTPARAMETER__TOUCH_PARAMETER_FIRST + 1,
