// Copyright 2009 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include "Common/CommonFuncs.h"
#include "Common/MsgHandler.h"

namespace LogTypes
{

enum LOG_TYPE
{
	ACTIONREPLAY,
	AUDIO,
	AUDIO_INTERFACE,
	BOOT,
	COMMANDPROCESSOR,
	COMMON,
	CONSOLE,
	DISCIO,
	FILEMON,
	DSPHLE,
	DSPLLE,
	DSP_MAIL,
	DSPINTERFACE,
	DVDINTERFACE,
	DYNA_REC,
	EXPANSIONINTERFACE,
	GDB_STUB,
	POWERPC,
	GPFIFO,
	OSHLE,
	MASTER_LOG,
	MEMMAP,
	MEMCARD_MANAGER,
	OSREPORT,
	PAD,
	PROCESSORINTERFACE,
	PIXELENGINE,
	SERIALINTERFACE,
	SP1,
	STREAMINGINTERFACE,
	VIDEO,
	VIDEOINTERFACE,
	WII_IPC,
	WII_IPC_DVD,
	WII_IPC_ES,
	WII_IPC_FILEIO,
	WII_IPC_HID,
	WII_IPC_HLE,
	WII_IPC_NET,
	WII_IPC_WC24,
	WII_IPC_SSL,
	WII_IPC_SD,
	WII_IPC_STM,
	WII_IPC_WIIMOTE,
	WIIMOTE,
	NETPLAY,

	NUMBER_OF_LOGS // Must be last
};

enum LOG_LEVELS
{
	LNOTICE  = 1, // VERY important information that is NOT errors. Like startup and OSReports.
	LERROR   = 2, // Critical errors
	LWARNING = 3, // Something is suspicious.
	LINFO    = 4, // General information.
	LDEBUG   = 5, // Detailed debugging - might make things slow.
};

static const char LOG_LEVEL_TO_CHAR[7] = "-NEWID";

}  // namespace

void GenericLog(LogTypes::LOG_LEVELS level, LogTypes::LOG_TYPE type,
		const char *file, int line, const char *fmt, ...)
#ifdef __GNUC__
		__attribute__((format(printf, 5, 6)))
#endif
		;

#if defined LOGGING || defined _DEBUG || defined DEBUGFAST
#define MAX_LOGLEVEL LogTypes::LOG_LEVELS::LDEBUG
#else
#ifndef MAX_LOGLEVEL
#define MAX_LOGLEVEL LogTypes::LOG_LEVELS::LWARNING
#endif // loglevel
#endif // logging

// Let the compiler optimize this out
#define GENERIC_LOG(t, v, ...) { \
	if (v <= MAX_LOGLEVEL) \
		GenericLog(v, t, __FILE__, __LINE__, __VA_ARGS__); \
	}

#define ERROR_LOG(t,...) do { GENERIC_LOG(LogTypes::t, LogTypes::LERROR, __VA_ARGS__) } while (0)
#define WARN_LOG(t,...) do { GENERIC_LOG(LogTypes::t, LogTypes::LWARNING, __VA_ARGS__) } while (0)
#define NOTICE_LOG(t,...) do { GENERIC_LOG(LogTypes::t, LogTypes::LNOTICE, __VA_ARGS__) } while (0)
#define INFO_LOG(t,...) do { GENERIC_LOG(LogTypes::t, LogTypes::LINFO, __VA_ARGS__) } while (0)
#define DEBUG_LOG(t,...) do { GENERIC_LOG(LogTypes::t, LogTypes::LDEBUG, __VA_ARGS__) } while (0)

#ifdef _WIN32
#define _dbg_assert_msg_(_t_, _a_, _msg_, ...)\
	if (MAX_LOGLEVEL >= LogTypes::LOG_LEVELS::LDEBUG && !(_a_)) {\
		ERROR_LOG(_t_, _msg_, __VA_ARGS__); \
		if (!PanicYesNo(_msg_, __VA_ARGS__)) \
			Crash(); \
	}
#else
#define _dbg_assert_msg_(_t_, _a_, _msg_, ...)\
	if (MAX_LOGLEVEL >= LogTypes::LOG_LEVELS::LDEBUG && !(_a_)) {\
		ERROR_LOG(_t_, _msg_, ##__VA_ARGS__); \
		if (!PanicYesNo(_msg_, ##__VA_ARGS__)) \
			Crash(); \
	}
#endif

#ifdef _WIN32
#define _assert_msg_(_t_, _a_, _fmt_, ...) \
	if (!(_a_)) {\
		if (!PanicYesNo(_fmt_, __VA_ARGS__)) \
			Crash(); \
	}
#else // not win32
#define _assert_msg_(_t_, _a_, _fmt_, ...) \
	if (!(_a_)) {\
		if (!PanicYesNo(_fmt_, ##__VA_ARGS__)) \
			Crash(); \
	}
#endif // WIN32

#define _assert_(_a_) \
	_assert_msg_(MASTER_LOG, _a_, "Error...\n\n  Line: %d\n  File: %s\n  Time: %s\n\nIgnore and continue?", \
	             __LINE__, __FILE__, __TIME__)

#define _dbg_assert_(_t_, _a_) \
	if (MAX_LOGLEVEL >= LogTypes::LOG_LEVELS::LDEBUG) \
		_assert_(_a_)
