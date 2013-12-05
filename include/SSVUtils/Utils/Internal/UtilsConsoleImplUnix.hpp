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

			inline int getColorFGCode(std::size_t mIdx) noexcept
			{
				static int codes[]
				{
					39,		// Default
					30,		// Black
					31,		// Red
					32,		// Green
					33,		// Yellow
					34,		// Blue
					35,		// Magenta
					36,		// Cyan
					37,		// Light gray
					90,		// Dark gray
					91,		// Light red
					92,		// Light green
					93,		// Light yellow
					94,		// Light blue
					95,		// Light magenta
					96,		// Light cyan
					97		// Light white
				};

				return codes[mIdx];
			}

			inline int getColorBGCode(std::size_t mIdx) noexcept { return getColorFGCode(mIdx) + 10; }

			inline int getModCode(std::size_t mIdx) noexcept
			{
				static int codes[]
				{
					0,	// None
					1,	// Bold
					2,	// Dim
					4,	// Underline
					5,	// Blink
					7,	// Reverse
					8,	// Hidden
					21,	// Reset bold
					22,	// Reset dim
					24,	// Reset underline
					25,	// Reset blink
					27,	// Reset reverse
					28	// Reset hidden
				};

				return codes[mIdx];
			}

			struct StrStorage
			{
				std::string styles[styleCount];
				std::string colorsFG[colorCount];
				std::string colorsBG[colorCount];

				inline StrStorage()
				{
					for(int i{0}; i < styleCount; ++i) styles[i] = {prefix + toStr(getModCode(i)) + postfix};
					for(int i{0}; i < colorCount; ++i) colorsFG[i] = {prefix + toStr(getModCode(0)) + ";" + toStr(getColorFGCode(i)) + postfix};
					for(int i{0}; i < colorCount; ++i) colorsBG[i] = {prefix + toStr(getModCode(0)) + ";" + toStr(getColorFGCode(0)) + ";" + toStr(getColorBGCode(i)) + postfix};
				}
			};

			inline StrStorage& getStorage() noexcept { static StrStorage storage; return storage; }

			inline const std::string& getStrResetFmt() noexcept				{ static std::string result{std::string{prefix} + postfix}; return result; }
			inline const std::string& getStrStyle(Style mStyle) noexcept	{ return getStorage().styles[int(mStyle)]; }
			inline const std::string& getStrColorFG(Color mColor) noexcept	{ return getStorage().colorsFG[int(mColor)]; }
			inline const std::string& getStrColorBG(Color mColor) noexcept	{ return getStorage().colorsBG[int(mColor)]; }
		}
	}
}

#endif
