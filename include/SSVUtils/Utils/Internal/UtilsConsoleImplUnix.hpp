// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UTILS_CONSOLE_IMPL
#define SSVU_UTILS_CONSOLE_IMPL

#include <string>

namespace ssvu
{
	namespace Console
	{
		namespace Internal
		{
			constexpr const char* prefix{"\033["};
			constexpr const char* postfix{"m"};
			constexpr const char* ansiClearSequence{"\e[1;1H\e[2J"};

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
			inline int getModCode(Style mStyle) noexcept
			{
				static int codes[]
				{
					0,	// 0 = Style::None
					1,	// 1 = Style::Bold
					2,	// 2 = Style::Dim
					4,	// 3 = Style::Underline
					5,	// 4 = Style::Blink
					7,	// 5 = Style::Reverse
					8,	// 6 = Style::Hidden
					21,	// 7 = Style::ResetBold
					22,	// 8 = Style::ResetDim
					24,	// 9 = Style::ResetUnderline
					25,	// 10 = Style::ResetBlink
					27,	// 11 = Style::ResetReverse
					28	// 12 = Style::ResetHidden
				};

				return codes[int(mStyle)];
			}

			struct StrStorage
			{
				Style lastStyle{Style::None};
				Color lastColorFG{Color::Default};
				Color lastColorBG{Color::Default};
				std::string lastFmtString;

				inline const std::string& getFmtString() noexcept
				{
					lastFmtString = {prefix + toStr(getModCode(lastStyle)) + ";" + toStr(getColorFGCode(lastColorFG)) + ";" + toStr(getColorBGCode(lastColorBG)) + postfix};
					return lastFmtString;
				}
			};

			inline StrStorage& getStorage() noexcept { static StrStorage storage; return storage; }

			inline const std::string& getStrResetFmt() noexcept
			{
				getStorage().lastStyle = Style::None;
				getStorage().lastColorFG = getStorage().lastColorBG = Color::Default;
				return getStorage().getFmtString();
			}
			inline const std::string& getStrStyle(Style mStyle) noexcept	{ getStorage().lastStyle = mStyle; return getStorage().getFmtString(); }
			inline const std::string& getStrColorFG(Color mColor) noexcept	{ getStorage().lastColorFG = mColor; return getStorage().getFmtString(); }
			inline const std::string& getStrColorBG(Color mColor) noexcept	{ getStorage().lastColorBG = mColor; return getStorage().getFmtString(); }
			inline const std::string& getStrClear() noexcept				{ static std::string result{ansiClearSequence}; return result; }
		}
	}
}

#endif
