// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_DETECTION
#define SSVU_CORE_DETECTION

// Operating system detection
#if (__linux || __unix || __posix || __LINUX__ || __linux__)
	/// @macro This macro is defined if the current OS is Linux.
	#define SSVU_OS_LINUX 1
#elif (_WIN64 || _WIN32 || __CYGWIN32__ || __MINGW32__)
	/// @macro This macro is defined if the current OS is Windows.noexcept
	#define SSVU_OS_WINDOWS 1
#elif (MACOSX || __DARWIN__ || __APPLE__)
	/// @macro This macro is defined if the current OS is Windows.
	#define SSVU_OS_MAC 1
#else
	/// @macro This macro is defined if the current OS is unknown.
	#define SSVU_OS_UNKNOWN 1
#endif

// Architecture detection
#if (WIN_32 || __i386__ || i386 || __x86__)
	/// @macro This macro is defined if the current architecture is 32 bit.
	#define SSVU_ARCH_32 1
#elif (__amd64 || __amd64__ || __x86_64 || __x86_64__ || _M_X64 || __ia64__ || _M_IA64)
	/// @macro This macro is defined if the current architecture is 64 bit.
	#define SSVU_ARCH_64 1
#else
	/// @macro This macro is defined if the current architecture is unknown.
	#define SSVU_ARCH_UNKNOWN 1
#endif

// Compiler detection
#if (__clang__)
	/// @macro This macro is defined if the current compiler is clang.
	#define SSVU_COMPILER_CLANG 1
#elif (__GNUC__)
	/// @macro This macro is defined if the current compiler is gcc.
	#define SSVU_COMPILER_GCC 1
#else
	/// @macro This macro is defined if the current compiler is unknown.
	#define SSVU_COMPILER_UNKNOWN 1
#endif

// Debug mode detection
#if !NDEBUG
	#define SSVU_DEBUG 1
#endif


#endif
