// pwin32time.cpp
// Fetch/set the system time.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#include "../ptime.h"

#include <windows.h>

#include <time.h>
#include <stdio.h>


PTime P_APIENTRY pTimeGetTime(void)
{
    PTime currentTime;
    tm systemTimeStruct;

    time_t currentSystemTime;
    time(&currentSystemTime);

    localtime_s(&systemTimeStruct, &currentSystemTime);

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
    SYSTEMTIME systemTime;

    systemTime.wDay = (WORD)(time->m_day); 
    systemTime.wDayOfWeek = 0;  // ignored 
    systemTime.wMonth = (WORD)(time->m_month + 1);
    systemTime.wYear = (WORD)(time->m_year);
    
    // TODO: time zone needs to be taken into account! 

    systemTime.wHour = (WORD)time->m_hours;
    systemTime.wMinute = (WORD)time->m_minutes;
    systemTime.wSecond = (WORD)time->m_seconds;
    systemTime.wMilliseconds = 0;

    SetSystemTime(&systemTime);
}

puint32 P_APIENTRY pTimeGetCurrentTimestamp()
{
    return (puint32)timeGetTime();
}
