// plog.h
// The log helper functions.
// 
// Copyright 2012 - 2014 Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PLOG_H
#define PLOG_H

#include <PFoundation/pabstractlogoutput.h>

//
// The levels of the log
// DEBUG - used for debug and only effective in the debug version. These debug log will be removed in the release version by conditional compiling.
// INFO - any thing informative and worthy printing out.
// WARNING - the program can still run but a glitch shows up
// ERROR - the function should return immediately at the point of which an error happens. whether to continue the program or not depends on the consideration/logic of application.
// FATAL - an unrecoverable error is seen. The program should stop immediately. 
enum PLogVerbosityEnum
{
    P_LOG_DEBUG,
    P_LOG_INFO,
    P_LOG_WARNING,
    P_LOG_ERROR,
    P_LOG_FATAL,

    P_LOG_NO_LOG = P_LOG_FATAL,
    P_LOG_VERBOSITY_FIRST = P_LOG_DEBUG,
    P_LOG_VERBOSITY_LAST = P_LOG_FATAL,
    P_LOG_VERBOSITY_NUMBER = P_LOG_VERBOSITY_LAST - P_LOG_VERBOSITY_FIRST + 1
};

// We have up to P_LOG_MAX_CHANNELS.
enum PLogChannelEnum
{
    P_LOG_CHANNEL0   = 0x1,      // All logs will be copied to this channel.
    P_LOG_CHANNEL1   = 0x2,      // OpenGL.
    P_LOG_CHANNEL2   = 0x4,      // Renderer
    P_LOG_CHANNEL3   = 0x8,      // Animation
    P_LOG_CHANNEL4   = 0x10,
    P_LOG_CHANNEL5   = 0x20,
    P_LOG_CHANNEL6   = 0x40,
    P_LOG_CHANNEL7   = 0x80,
    P_LOG_CHANNEL8   = 0x100,
    P_LOG_CHANNEL9   = 0x200,
    P_LOG_CHANNEL10  = 0x400,
    P_LOG_CHANNEL11  = 0x800,
    P_LOG_CHANNEL12  = 0x1000,
    P_LOG_CHANNEL13  = 0x2000,
    P_LOG_CHANNEL14  = 0x4000,
    P_LOG_CHANNEL15  = 0x8000, 
    P_LOG_CHANNEL16  = 0x10000, // Studio 
    P_LOG_CHANNEL17  = 0x20000,
    P_LOG_CHANNEL18  = 0x30000,
    P_LOG_CHANNEL19  = 0x40000,
    P_LOG_CHANNEL20  = 0x100000,
    P_LOG_CHANNEL21  = 0x200000,
    P_LOG_CHANNEL22  = 0x300000,
    P_LOG_CHANNEL23  = 0x400000, 
    P_LOG_CHANNEL24  = 0x1000000, // User application
    P_LOG_CHANNEL25  = 0x2000000,
    P_LOG_CHANNEL26  = 0x4000000,
    P_LOG_CHANNEL27  = 0x8000000,
    P_LOG_CHANNEL28  = 0x10000000,
    P_LOG_CHANNEL29  = 0x20000000,
    P_LOG_CHANNEL30  = 0x40000000,
    P_LOG_CHANNEL31  = 0x80000000,

    P_LOG_CHANNEL_DEFAULT = P_LOG_CHANNEL0,

    P_LOG_CHANNEL_OPENGLEGL = P_LOG_CHANNEL1,
    P_LOG_CHANNEL_RENDERER = P_LOG_CHANNEL2,
    P_LOG_CHANNEL_ANIMATION = P_LOG_CHANNEL3,

    P_LOG_MAX_CHANNELS = 32,
};

// Fatal message can't be turned off.
#if defined P_GCC
#  define PLOGFATAL(fmt, ...) \
    do { pLog(P_LOG_FATAL, P_LOG_CHANNEL_DEFAULT, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } while (0)
#  define pLogFatalX(channel, fmt, ...) \
    do { pLog(P_LOG_FATAL, channel, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } while (0)
#elif defined P_MSC
#  define PLOGFATAL(fmt, ...) \
    do { pLog(P_LOG_FATAL, P_LOG_CHANNEL_DEFAULT, __FILE__, __LINE__, fmt, __VA_ARGS__); } while (0)
#  define pLogFatalX(channel, fmt, ...) \
    do { pLog(P_LOG_FATAL, channel, __FILE__, __LINE__, fmt, __VA_ARGS__); } while (0)
#endif

#if P_ENABLE_LOGGING == 1
# if defined P_GCC || defined P_LLVM
#  define PLOG_ERROR(fmt, ...) \
    do { pLog(P_LOG_ERROR, P_LOG_CHANNEL_DEFAULT, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } while (0)
#  define PLOG_ERRORX(channel, fmt, ...) \
    do { pLog(P_LOG_ERROR, channel, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } while (0)
#  define PLOG_WARNING(fmt, ...) \
    do { pLog(P_LOG_WARNING, P_LOG_CHANNEL_DEFAULT, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } while (0)
#  define PLOG_WARNINGX(channel, fmt, ...) \
    do { pLog(P_LOG_WARNING, channel, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } while (0)
#  define PLOG_INFO(fmt, ...) \
    do { pLog(P_LOG_INFO, P_LOG_CHANNEL_DEFAULT, P_NULL, 0, fmt, ##__VA_ARGS__); } while (0)
#  define PLOG_INFOX(channel, fmt, ...) \
    do { pLog(P_LOG_INFO, channel, P_NULL, 0, fmt, ##__VA_ARGS__); } while (0)
# elif defined P_MSC
#  define PLOG_ERROR(fmt, ...) \
    do { pLog(P_LOG_ERROR, P_LOG_CHANNEL_DEFAULT, __FILE__, __LINE__, fmt, __VA_ARGS__); } while (0)
#  define PLOG_ERRORX(channel, fmt, ...) \
    do { pLog(P_LOG_ERROR, channel, __FILE__, __LINE__, fmt, __VA_ARGS__); } while (0)
#  define PLOG_WARNING(fmt, ...) \
    do { pLog(P_LOG_WARNING, P_LOG_CHANNEL_DEFAULT, __FILE__, __LINE__, fmt, __VA_ARGS__); } while (0)
#  define PLOG_WARNINGX(channel, fmt, ...) \
    do { pLog(P_LOG_WARNING, channel, __FILE__, __LINE__, fmt, __VA_ARGS__); } while (0)
#  define PLOG_INFO(fmt, ...) \
    do { pLog(P_LOG_INFO, P_LOG_CHANNEL_DEFAULT, P_NULL, 0, fmt, __VA_ARGS__); } while (0)
#  define PLOG_INFOX(channel, fmt, ...) \
    do { pLog(P_LOG_INFO, channel, P_NULL, 0, fmt, __VA_ARGS__); } while (0)
# else
#  error "unsupported platform"
# endif
# if defined P_DEBUG
#  if defined P_GCC || defined P_LLVM
#   define PLOG_DEBUG(fmt, ...) \
     do{ pLog(P_LOG_DEBUG, P_LOG_CHANNEL_DEFAULT, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } while (0)
#   define PLOG_DEBUGX(channel, fmt, ...) \
     do{ pLog(P_LOG_DEBUG, channel, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } while (0)
#   define PLOG_DEBUGIf(cond, fmt, ...) \
     { if ((cond)) { pLog(P_LOG_DEBUG, P_LOG_CHANNEL_DEFAULT, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } }
#   define PLOG_DEBUGIfX(cond, channel, fmt, ...) \
     { if ((cond)) { pLog(P_LOG_DEBUG, channel, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } }
#  elif defined P_MSC
#   define PLOG_DEBUG(fmt, ...) \
     do { pLog(P_LOG_DEBUG, P_LOG_CHANNEL_DEFAULT, __FILE__, __LINE__, fmt, __VA_ARGS__); } while (0)
#   define PLOG_DEBUGX(channel, fmt, ...) \
     do { pLog(P_LOG_DEBUG, channel, __FILE__, __LINE__, fmt, __VA_ARGS__); } while (0)
#   define PLOG_DEBUGIf(cond, fmt, ...) \
     { if ((cond)) { pLog(P_LOG_DEBUG, P_LOG_CHANNEL_DEFAULT, __FILE__, __LINE__, fmt, __VA_ARGS__); } }
#   define PLOG_DEBUGIfX(cond, channel, fmt, ...) \
     { if ((cond)) { pLog(P_LOG_DEBUG, channel, __FILE__, __LINE__, fmt, __VA_ARGS__); } }
#  else
#   error "unsupported platform"
#  endif
# else
#   define PLOG_DEBUG(fmt, ...) 
#   define PLOG_DEBUGIf(cond, fmt, ...) 
#   define PLOG_DEBUGX(fmt, channel, ...) 
#   define PLOG_DEBUGIfX(cond, channel, fmt, ...) 
# endif
#else
# define PLOG_ERROR(fmt, ...) 
# define PLOG_ERRORX(fmt, channel, ...) 
# define PLOG_WARNING(fmt, ...) 
# define PLOG_WARNINGX(fmt, channel, ...) 
# define PLOG_INFO(fmt, ...)
# define PLOG_INFOX(fmt, channel, ...)
# define PLOG_DEBUG(fmt, ...)
# define PLOG_DEBUGX(fmt, channel, ...)
# define PLOG_DEBUGIf(cond, fmt, ...)
# define PLOG_DEBUGIfX(cond, channel, fmt, ...)

#endif // P_ENABLE_LOGGING


#if P_ENABLE_LOGGING == 1

// Output the log in the formatted way
P_EXTERN void P_CCONV pLog(PLogVerbosityEnum verbosity, PLogChannelEnum channel, const pchar *file, puint32 line, const pchar *format, ...);

// Initialize the log subsystem. Only need to be called once at the initialization of the engine.
P_EXTERN void P_APIENTRY pInitializeLogSystem();

// Uninitialized the log subsystem. Should be called at the uninitialization of the engine.
P_EXTERN void P_APIENTRY pUninitializeLogSystem();

// Set the output target of each channel. The output should be being valid
// thereafter. A local variable of output object shouldn't be used to be the
// parameter of this function.
P_EXTERN void pLogChannelSetOutput(PLogChannelEnum channel, PAbstractLogOutput *output);

// Set the name of the channel. The default is "channel[n]" where n is the channel id.
P_EXTERN void pLogChannelSetName(PLogChannelEnum channel, const pchar *name);

// Set the global verbosity
P_EXTERN void P_APIENTRY pLogSetVerbosity(PLogVerbosityEnum verbosity);

#else

#define pLog(...)
#define pInitializeLogSystem() 
#define pUninitializeLogSystem()
#define pLogChannelSetOutput(channel, output)
#define pLogChannelSetName(channel, name)
#define pLogSetVerbosity(x)

#endif // P_ENABLE_LOGGING == 0

#endif // !PLOG_H
