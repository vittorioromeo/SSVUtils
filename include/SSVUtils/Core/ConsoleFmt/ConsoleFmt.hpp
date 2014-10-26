// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_CONSOLEFMT
#define SSVU_CORE_CONSOLEFMT

namespace ssvu
{
	namespace Console
	{
		/// @brief Number of styles.
		constexpr SizeT styleCount{13};

		/// @brief Number of colors.
		constexpr SizeT colorCount{16};

		/// @brief Enum class representing all the possible styles.
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

		/// @brief Enum class representing all the possible colors.
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

// Depending on the OS, the correct implementation file is included.

#if defined(SSVU_OS_LINUX)
	#include "SSVUtils/Core/ConsoleFmt/Internal/ConsoleFmtImplUnix.hpp"
#elif defined(SSVU_OS_WINDOWS)
	#include "SSVUtils/Core/ConsoleFmt/Internal/ConsoleFmtImplWin.hpp"
#else
	#include "SSVUtils/Core/ConsoleFmt/Internal/ConsoleFmtImplNull.hpp"
#endif

namespace ssvu
{
	namespace Console
	{
		/// @brief Returns a format string that resets the current formatting.
		inline const auto& resetFmt() noexcept { return Internal::getStrResetFmt(); }

		/// @brief Returns a format string that sets the current style.
		/// @param mStyle Desired style. (ssvu::Console::Style member)
		inline const auto& setStyle(Style mStyle) noexcept { return Internal::getStrStyle(mStyle); }

		/// @brief Returns a format string that sets the current foreground color.
		/// @param mStyle Desired color. (ssvu::Console::Color member)
		inline const auto& setColorFG(Color mColor) noexcept { return Internal::getStrColorFG(mColor); }

		/// @brief Returns a format string that sets the current background color.
		/// @param mStyle Desired color. (ssvu::Console::Color member)
		inline const auto& setColorBG(Color mColor) noexcept { return Internal::getStrColorBG(mColor); }

		/// @brief Returns a format string that clears the console window.
		inline const auto& clear() noexcept { return Internal::getStrClear(); }

		/// @brief Returns true if valid console information is available.
		inline bool isInfoValid() noexcept { return Internal::isInfoValid(); }

		namespace Info
		{
			/// @brief Returns then number of columns of the console screen.
			inline SizeT getColumnCount() noexcept { return Internal::Info::getColumnCount(); }

			/// @brief Returns then number of rows of the console screen.
			inline SizeT getRowCount() noexcept { return Internal::Info::getRowCount(); }
		}
	}
}

#endif
