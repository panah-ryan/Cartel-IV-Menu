#pragma once
#include <constants.h>
#include <intrinsics.h>
#include <types.h>
#include <varargs.h>
#include <natives.h>
#include <common.h>

#define UNUSED_PARAM(x) [[unused]] int x
//#define STEALTH //stealthy for risky users / no dicklist prints etc

#define PlayerDB
//#define Patron //some early access only features//
//#define Developer //adds print obfuscated ints to admin options
//#define Moderator

//#define OUTPUT_AUTH_DEBUG

//DON'T USE //#define AdminOptions //shouldn't need to use this on it's own, just use developer or moderator

#ifdef Developer
#define AdminOptions
#define Crashes
#define Moderator
#define Security
//#define PlayerDB
#endif

#ifdef AdminOptions
//#define Moderator
#define Security
#endif

#ifdef Patron
#define Security
#define CartelAdvertiseString "Cartel Patron" //change to "Cartel User" once early access is done.
#else
#define CartelAdvertiseString "Cartel User"
#endif

#ifdef Moderator
#define Security
#endif

#ifndef Developer
#undef AUTH_DEBUG
#undef OUTPUT_AUTH_DEBUG
#endif