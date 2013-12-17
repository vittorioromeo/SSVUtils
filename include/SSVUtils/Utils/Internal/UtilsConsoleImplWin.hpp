// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UTILS_CONSOLE_IMPL
#define SSVU_UTILS_CONSOLE_IMPL

#include <string>
#include <windows.h>

namespace ssvu
{
	namespace Console
	{
		namespace Internal
		{
			// TODO: implement

			inline char getColorCode(std::size_t mIdx) noexcept
			{
				static int codes[]
				{
					' ',	// 0 = Color::Default
					'0',	// 1 = Color::Black
					'4',	// 2 = Color::Red
					'2',	// 3 = Color::Green
					'6',	// 4 = Color::Yellow
					'1',	// 5 = Color::Blue
					'5',	// 6 = Color::Magenta
					'3',	// 7 = Color::Cyan
					'7',	// 8 = Color::LightGray
					'8',	// 9 = Color::DarkGray
					'c',	// 10 = Color::LightRed
					'a',	// 11 = Color::LightGreen
					'e',	// 12 = Color::LightYellow
					'9',	// 13 = Color::LightBlue
					'd',	// 14 = Color::LightMagenta
					'b',	// 15 = Color::LightCyan
					'f'		// 16 = Color::LightWhite
				};

				return codes[mIdx];
			}

			struct StrStorage
			{
				Color lastColorFG{Color::Default};
				Color lastColorBG{Color::Default};
				std::string lastFmtString;

				inline const std::string& getFmtString()
				{
					lastFmtString = "COLOR " + toStr(getColorCode(int(lastColorFG))) + toStr(getColorCode(int(lastColorBG)));
					return lastFmtString;
				}
			};

			inline const std::string& getStrResetFmt() noexcept
			{
				getStorage().lastColorFG = Color::Default;
				getStorage().lastColorBG = Color::Default;
				return getStorage().getFmtString();
			}
			inline const std::string& getStrStyle(Style) noexcept			{ system(getStorage().getFmtString()); return ""; }
			inline const std::string& getStrColorFG(Color mColor) noexcept	{ getStorage().lastColorFG = mColor; system(getStorage().getFmtString()); return ""; }
			inline const std::string& getStrColorBG(Color mColor) noexcept	{ getStorage().lastColorBG = mColor; system(getStorage().getFmtString()); return ""; }
		}
	}
}

#endif
