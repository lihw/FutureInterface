// pconfigs.h
// Building configuration. Turning on/off switch in this file
// will enable certain features and change certain attributes
// of the engine.
//
// Copyright 2012 - 2014 by Future Interface. 
// This software is licensed under the terms of the MIT license.
//
// Hongwei Li lihw81@gmail.com
//

#ifndef PCONFIGS_H
#define PCONFIGS_H

// -------------------------------------------------------------- 
// Platform layer
// -------------------------------------------------------------- 

// -------------------------------------------------------------- 
// Foundation layer
// -------------------------------------------------------------- 
#ifndef P_CONTEXT_MAXIMUM_NUMBER
#  define P_CONTEXT_MAXIMUM_NUMBER 16
#endif

#define P_CONTEXT_INVALID_ID  (0xffffffff)

#ifndef P_EVENT_QUEUE_LENGTH
#  define P_EVENT_QUEUE_LENGTH 64 // The maximum event number in the event queue.
#endif

#ifndef P_INPUT_MAXIMUM_TOUCH
#  define P_INPUT_MAXIMUM_TOUCH 5 // The maximum touch point supported
#endif

#ifndef P_INPUT_EVENT_QUEUE_INITIAL_SIZE
#  define P_INPUT_EVENT_QUEUE_INITIAL_SIZE 32 // The initial input queue length
#endif

// FIMXE: use function arguments rather than macros
#define P_SPHERE_SEGMENT_NUMBER 32
#define P_SPHERE_RING_NUMBER 16

#if !defined P_ENABLE_IMAGE_PNG
#  define P_ENABLE_IMAGE_PNG 1
#endif

#if !defined P_ENABLE_IMAGE_TGA
#  define P_ENABLE_IMAGE_TGA 1
#endif

#if !defined P_ENABLE_LOGGING  // Enable the logging
#  define P_ENABLE_LOGGING 1
#endif 

#if !defined P_NETWORK_MAX_CONNECTIONS
# define P_NETWORK_MAX_CONNECTIONS 8
#endif

#if !defined P_NETWORK_MAX_PACKET_SIZE
# define P_NETWORK_MAX_PACKET_SIZE 4096
#endif

#if !defined P_NETWORK_POLL_TIME
# define P_NETWORK_POLL_TIME 10 // Milliseconds
#endif

#if !defined P_ENABLE_MEMORY_DEBUGGING // Enable the memory debugging 
#  if defined P_DEBUG && defined P_WIN32
#    define P_ENABLE_MEMORY_DEBUGGING 1
#  else
#    define P_ENABLE_MEMORY_DEBUGGING 0
#  endif
#endif 

#if !defined P_ENABLE_NETWORK
# define P_ENABLE_NETWORK 0
#endif


#endif // !PCONFIGS_H
