// ptime.h
// Fetch and set the system time.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//


#ifndef PTIME_H
#define PTIME_H

#include <PFoundation/pglobal.h>


struct PTime
{
    puint32 m_hours;   ///< Hours 0-23 
    puint32 m_minutes; ///< Minutes 0-59. 
    puint32 m_seconds; ///< Seconds 0-59. 
    puint32 m_day;     ///< Day of month. 1 is the first day. 
    puint32 m_month;   ///< Month of year. 1 is January. 
    puint32 m_year;    ///< Year. 
};

// Returns the current time and date in a structure. 
// \return the current time in PTime structure.
P_EXTERN PTime P_APIENTRY pTimeGetTime(void);

// Sets the current time and date. 
// \param time the target current time. 
P_EXTERN void P_APIENTRY pTimeSetTime(const PTime* time);

// Returns current time from the system in milliseconds.
// Values should be only used relative to each other because starting time is not specified.
// \return the current timestamp.
P_EXTERN puint32 P_APIENTRY pTimeGetCurrentTimestamp(void);

#if defined P_MSC
# pragma comment (lib, "winmm.lib") // for timeGetTime();
#endif

#endif
