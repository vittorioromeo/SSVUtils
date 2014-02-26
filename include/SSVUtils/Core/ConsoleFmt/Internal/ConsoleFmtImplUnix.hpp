// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_CONSOLEFMT_IMPL
#define SSVU_CORE_CONSOLEFMT_IMPL

// This file contains an Unix implementation for console formatting.
// Everything returns ASCII formatting codes.

#include <string>

namespace ssvu
{
	namespace Console
	{
		namespace Internal
		{
			constexpr const char* prefix{"\033["};
			constexpr const char* postfix{"m"};
			constexpr const char* clear{"\033[1;1H\033[2J"};

			inline int getStyleCode(Style mStyle) noexcept
			{
				static int codes[]
				{
					0,		// 0 = Style::None
					1,		// 1 = Style::Bold
					2,		// 2 = Style::Dim
					4,		// 3 = Style::Underline
					5,		// 4 = Style::Blink
					7,		// 5 = Style::Reverse
					8,		// 6 = Style::Hidden
					21,		// 7 = Style::ResetBold
					22,		// 8 = Style::ResetDim
					24,		// 9 = Style::ResetUnderline
					25,		// 10 = Style::ResetBlink
					27,		// 11 = Style::ResetReverse
					28		// 12 = Style::ResetHidden
				};

				return codes[int(mStyle)];
			}
			inline int getColorFGCode(Color mColor) noexcept
			{
				static int codes[]
				{
					39,		// 0 = Color::Default
					30,		// 1 = Color::Black
					31,		// 2 = Color::Red
					32,		// 3 = Color::Green
					33,		// 4 = Color::Yellow
					34,		// 5 = Color::Blue
					35,		// 6 = Color::Magenta
					36,		// 7 = Color::Cyan
					37,		// 8 = Color::LightGray
					90,		// 9 = Color::DarkGray
					91,		// 10 = Color::LightRed
					92,		// 11 = Color::LightGreen
					93,		// 12 = Color::LightYellow
					94,		// 13 = Color::LightBlue
					95,		// 14 = Color::LightMagenta
					96,		// 15 = Color::LightCyan
					97		// 16 = Color::LightWhite
				};

				return codes[int(mColor)];
			}
			inline int getColorBGCode(Color mColor) noexcept { return getColorFGCode(mColor) + 10; }

			inline Style& getLastStyle() noexcept	{ static Style result{Style::None};		return result; }
			inline Color& getLastColorFG() noexcept	{ static Color result{Color::Default};	return result; }
			inline Color& getLastColorBG() noexcept	{ static Color result{Color::Default};	return result; }
			inline const std::string& getFmtStr() noexcept
			{
				static std::string result;
				result = {prefix + toStr(getStyleCode(getLastStyle())) + ";" + toStr(getColorFGCode(getLastColorFG())) + ";" + toStr(getColorBGCode(getLastColorBG())) + postfix};
				return result;
			}

			inline const std::string& getStrResetFmt() noexcept
			{
				getLastStyle() = Style::None;
				getLastColorFG() = Color::Default;
				getLastColorBG() = Color::Default;
				return getFmtStr();
			}
			inline const std::string& getStrStyle(Style mStyle) noexcept	{ getLastStyle() = mStyle;				return getFmtStr(); }
			inline const std::string& getStrColorFG(Color mColor) noexcept	{ getLastColorFG() = mColor;			return getFmtStr(); }
			inline const std::string& getStrColorBG(Color mColor) noexcept	{ getLastColorBG() = mColor;			return getFmtStr(); }
			inline const std::string& getStrClear() noexcept				{ static std::string result{clear};		return result; }
		}
	}
}

#endif
