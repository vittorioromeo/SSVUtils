// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_DETECTION
#define SSVU_CORE_DETECTION

// Operating system detection
#if (defined(__linux) || defined(__unix) || defined(__posix) || defined(__LINUX__) || defined(__linux__))
	/// @macro This macro is defined if the current OS is Linux.
	#define SSVU_OS_LINUX 1
#elif (defined(_WIN64) || defined(_WIN32) || defined(__CYGWIN32__) || defined(__MINGW32__))
	/// @macro This macro is defined if the current OS is Windows.
	#define SSVU_OS_WINDOWS 1
#elif (defined(MACOSX) || defined(__DARWIN__) || defined(__APPLE__))
	/// @macro This macro is defined if the current OS is Mac.
	#define SSVU_OS_MAC 1
#else
	/// @macro This macro is defined if the current OS is unknown.
	#define SSVU_OS_UNKNOWN 1
#endif

// Architecture detection
#if (defined(WIN_32) || defined(__i386__) || defined(i386) || defined(__x86__))
	/// @macro This macro is defined if the current architecture is 32 bit.
	#define SSVU_ARCH_32 1
#elif (defined(__amd64) || defined(__amd64__) || defined(__x86_64) || defined(__x86_64__) || defined(_M_X64) || defined(__ia64__) || defined(_M_IA64))
	/// @macro This macro is defined if the current architecture is 64 bit.
	#define SSVU_ARCH_64 1
#else
	/// @macro This macro is defined if the current architecture is unknown.
	#define SSVU_ARCH_UNKNOWN 1
#endif

// Compiler detection
#if (defined(__clang__))
	/// @macro This macro is defined if the current compiler is clang.
	#define SSVU_COMPILER_CLANG 1
#elif (defined(__GNUC__))
	/// @macro This macro is defined if the current compiler is gcc.
	#define SSVU_COMPILER_GCC 1
#else
	/// @macro This macro is defined if the current compiler is unknown.
	#define SSVU_COMPILER_UNKNOWN 1
#endif

// Debug mode detection
#if !defined(NDEBUG)
	#define SSVU_DEBUG 1
#endif


#endif
