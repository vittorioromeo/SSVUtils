// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UTILS_CONSOLE
#define SSVU_UTILS_CONSOLE

#include <string>

namespace ssvu
{
	namespace Console
	{
		constexpr std::size_t styleCount{13};
		constexpr std::size_t colorCount{16};

		enum class Style : int
		{
			None			= 0,
			Bold			= 1,
			Dim				= 2,
			Underline		= 3,
			Blink			= 4,
			ReverseFGBG		= 5,
			Hidden			= 6,
			ResetBold		= 7,
			ResetDim		= 8,
			ResetUnderline	= 9,
			ResetBlink		= 10,
			ResetReverse	= 11,
			ResetHidden		= 12
		};
		enum class Color : int
		{
			Default			= 0,
			Black			= 1,
			Red				= 2,
			Green			= 3,
			Yellow			= 4,
			Blue			= 5,
			Magenta			= 6,
			Cyan			= 7,
			LightGray		= 8,
			DarkGray		= 9,
			LightRed		= 10,
			LightGreen		= 11,
			LightYellow		= 12,
			LightBlue		= 13,
			LightMagenta	= 14,
			LightCyan		= 15,
			LightWhite		= 16
		};
	}
}

#ifdef SSVU_OS_LINUX
	#include "SSVUtils/Utils/Internal/ConsoleImplUnix.hpp"
#elif SSVU_OS_WINDOWS
	#include "SSVUtils/Utils/Internal/ConsoleImplWin.hpp"
#else
	#include "SSVUtils/Utils/Internal/ConsoleImplNull.hpp"
#endif

namespace ssvu
{
	namespace Console
	{
		inline const std::string& resetFmt() noexcept				{ return Internal::getStrResetFmt(); }
		inline const std::string& setStyle(Style mStyle) noexcept	{ return Internal::getStrStyle(mStyle); }
		inline const std::string& setColorFG(Color mColor) noexcept	{ return Internal::getStrColorFG(mColor); }
		inline const std::string& setColorBG(Color mColor) noexcept	{ return Internal::getStrColorBG(mColor); }
		inline const std::string& clear() noexcept					{ return Internal::getStrClear(); }
	}
}

#endif
