// plog.cpp
// The log helper functions.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com

#include "plog.h"

#include <stdarg.h>

#include <PFoundation/pcrt.h>
#include <PFoundation/pdebugutility.h>
#include <PFoundation/pnew.h>
#include <PFoundation/plogoutputconsole.h>
#include <PFoundation/plogoutputdebug.h>

#if P_ENABLE_LOGGING == 1

static PAbstractLogOutput *g_logChannels[P_LOG_MAX_CHANNELS];

// By default, all kinds of log will be printed.
static PLogVerbosityEnum g_logVerbosity = P_LOG_DEBUG;

static void P_APIENTRY pLogOutput(PLogChannelEnum channel, 
    const pchar *text, 
    puint32 length) 
{
    if (channel != P_LOG_CHANNEL_DEFAULT && g_logChannels[channel] != P_NULL)
    {
        g_logChannels[channel]->print(text, length);
    }
    else
    {
        g_logChannels[P_LOG_CHANNEL_DEFAULT]->print(text, length);
    }
}

void P_CCONV pLog(PLogVerbosityEnum verbosity, 
    PLogChannelEnum channel,
    const pchar *file, 
    puint32 line, 
    const pchar *format, 
    ...)
{
    // The global verbosity value controls which kinds of log can be 
    // printed.
    if (verbosity < g_logVerbosity)
    {
        return ;
    }

    static const pchar *verbosityText[] = 
    {
        "(debug)",
        "(info)",
        "(warning)",
        "(error)",
        "(fatal)",
    };

    va_list arguments;

    va_start(arguments, format);

    if (pstrlen(format) > 200)
    {
        pchar message[1024];
        puint32 nchars = psprintf(message, 1024, 
                "(warning)%s:%d,log buffer for formatting data is too small.",
                __FILE__, __LINE__);
        pLogOutput(P_LOG_CHANNEL2, message, nchars);
    }
    else
    {
        pchar msg[3000]; 
        puint32 nchars;
        if (verbosity == P_LOG_ERROR || verbosity == P_LOG_WARNING)
        {
            pchar fmt[256];
            psprintf(fmt, 256, "%s%s:%d,%s", 
                verbosityText[verbosity], file, line, format);
            nchars = pvsprintf(msg, 3000, fmt, arguments);
        }
        else 
        {
            nchars = pvsprintf(msg, 3000, format, arguments);
        }

        pLogOutput(channel, msg, nchars);
    }

    if (verbosity == P_LOG_FATAL)
    {
        // FIXME: make it more polite
        pabort();
    }
}

void P_APIENTRY pInitializeLogSystem()
{
    pchar name[128];
    for (puint32 i = 0; i < P_LOG_MAX_CHANNELS; ++i)
    {
        g_logChannels[i] = PNEW(PLogOutputConsole);
        psprintf(name, 128, "channel%02d", i);
        g_logChannels[i]->setName(name);
    }
}

void P_APIENTRY pUninitializeLogSystem()
{
    for (puint32 i = 0; i < P_LOG_MAX_CHANNELS; ++i)
    {
        PDELETE(g_logChannels[i]);
    }
}

void pLogChannelSetOutput(PLogChannelEnum channel, PAbstractLogOutput *output)
{
    if (g_logChannels[channel] != P_NULL)
    {
        PDELETE(g_logChannels[channel]);
    }
    g_logChannels[channel] = output;
}

void pLogChannelSetName(PLogChannelEnum channel, const pchar *name)
{
    g_logChannels[channel]->setName(name);
}

void P_APIENTRY pLogSetVerbosity(PLogVerbosityEnum verbosity)
{
    g_logVerbosity = verbosity;
}

#endif // P_ENABLE_LOGGING
