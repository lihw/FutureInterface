// piostime.mm
// Fetch/set the system time.
// 
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "../ptime.h"

#include <time.h>
#include <sys/time.h>
#include <stdio.h>

#if defined __MACH__
#  include <mach/clock.h>
#  include <mach/mach.h>
#endif

PTime P_APIENTRY pTimeGetTime(void)
{
    PTime currentTime;
    tm systemTimeStruct;

    time_t currentSystemTime;
    time(&currentSystemTime);

    localtime_r(&currentSystemTime, &systemTimeStruct);

    currentTime.m_hours = systemTimeStruct.tm_hour;
    currentTime.m_minutes = systemTimeStruct.tm_min;
    currentTime.m_seconds = systemTimeStruct.tm_sec;

    currentTime.m_day = systemTimeStruct.tm_mday;
    currentTime.m_month = systemTimeStruct.tm_mon + 1;
    currentTime.m_year = systemTimeStruct.tm_year + 1900;

    return currentTime;
}

void P_APIENTRY pTimeSetTime(const PTime* time)
{
    struct tm systemTimeStruct;
    time_t systemTime;

    systemTimeStruct.tm_hour = time->m_hours;
    systemTimeStruct.tm_min = time->m_minutes;
    systemTimeStruct.tm_sec = time->m_seconds;

    systemTimeStruct.tm_mday = time->m_day;
    systemTimeStruct.tm_mon = time->m_month - 1;
    systemTimeStruct.tm_year = time->m_year - 1900;

    systemTimeStruct.tm_isdst = 0;

    systemTime = mktime(&systemTimeStruct);

    if(systemTime != -1)
    {
        struct timeval timeValue;
        timeValue.tv_sec = systemTime;
        timeValue.tv_usec = 0;

        settimeofday(&timeValue, NULL);
    }        
}

puint32 P_APIENTRY pTimeGetCurrentTimestamp()
{
    puint32 milliSeconds;
    
    clock_serv_t cclock;
    mach_timespec_t mts;

    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    
    // Combine nanoseconds and seconds into milliseconds. 
    milliSeconds = (mts.tv_nsec / 1000000) + (mts.tv_sec * 1000);

    return milliSeconds;
}

