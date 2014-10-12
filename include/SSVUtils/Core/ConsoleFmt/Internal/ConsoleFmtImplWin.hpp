// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_CONSOLEFMT_IMPL
#define SSVU_CORE_CONSOLEFMT_IMPL

// This file contains a Windows implementation for console formatting.
// Currently work-in-progress.

#include <windows.h>

namespace ssvu
{
	namespace Console
	{
		namespace Internal
		{
			// TODO: fix (http://www.cplusplus.com/articles/2ywTURfi/)

			inline int getColorCode(SizeT mIdx) noexcept
			{
				static int codes[]
				{
					0,	// 0 = Color::Default
					0,	// 1 = Color::Black
					4,	// 2 = Color::Red
					2,	// 3 = Color::Green
					6,	// 4 = Color::Yellow
					1,	// 5 = Color::Blue
					5,	// 6 = Color::Magenta
					3,	// 7 = Color::Cyan
					8,	// 8 = Color::LightGray
					7,	// 9 = Color::DarkGray
					12,	// 10 = Color::LightRed
					10,	// 11 = Color::LightGreen
					14,	// 12 = Color::LightYellow
					9,	// 13 = Color::LightBlue
					13,	// 14 = Color::LightMagenta
					11,	// 15 = Color::LightCyan
					15	// 16 = Color::LightWhite
				};

				return codes[mIdx];
			}
/*
			inline void setColor(int mFGCode, int mBGCode)
			{
				mFGCode %= 16; mBGCode %= 16;
				unsigned short wAttributes{((unsigned)mBGCode << 4) | (unsigned)mFGCode};
				SetConsoleTextAttribute(getStorage().hConsole, wAttributes);
			}

			#if defined(_INC_OSTREAM)||defined(_IOSTREAM_)
			ostream& operator<<(ostream& os,concol c)
			{os.flush();setcolor(c,backcolor());return os;}
			#endif

			#if defined(_INC_ISTREAM)||defined(_IOSTREAM_)
			istream& operator>>(istream& is,concol c)
			{cout.flush();setcolor(c,backcolor());return is;}
			#endif*/

			struct StrStorage
			{
				HANDLE hConsole;

				inline StrStorage() { hConsole = GetStdHandle(STD_OUTPUT_HANDLE); }

				Color lastColorFG{Color::Default};
				Color lastColorBG{Color::Default};
				std::string lastFmtString;

				inline void apply()
				{
					unsigned short wAttributes{((unsigned short)getColorCode((unsigned short)(lastColorFG) % 16) << 4) | (unsigned short)getColorCode((unsigned short)(lastColorBG) % 16)};
					SetConsoleTextAttribute(hConsole, wAttributes);
				}
			};

			inline StrStorage& getStorage() noexcept { static StrStorage storage; return storage; }
/*
			inline const std::string& getStrResetFmt() noexcept
			{
				getStorage().lastColorFG = Color::Default;
				getStorage().lastColorBG = Color::Default;
				getStorage().apply();
				return getStorage().emptyString;
			}
			inline const std::string& getStrStyle(Style) noexcept			{ getStorage().apply(); return getStorage().emptyString; }
			inline const std::string& getStrColorFG(Color mColor) noexcept	{ getStorage().lastColorFG = mColor; getStorage().apply(); return getStorage().emptyString; }
			inline const std::string& getStrColorBG(Color mColor) noexcept	{ getStorage().lastColorBG = mColor; getStorage().apply(); return getStorage().emptyString; }
			*/

			inline const auto& getStrResetFmt() noexcept		{ return getEmptyString(); }
			inline const auto& getStrStyle(Style) noexcept		{ return getEmptyString(); }
			inline const auto& getStrColorFG(Color) noexcept	{ return getEmptyString(); }
			inline const auto& getStrColorBG(Color) noexcept	{ return getEmptyString(); }
			inline const auto& getStrClear() noexcept			{ return getEmptyString(); }

			struct InfoImpl
			{
				SizeT columns, rows;

				inline InfoImpl()
				{
					CONSOLE_SCREEN_BUFFER_INFO csbi;

					GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
					columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
					rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
				}
			};

			inline const auto& getInfoImpl() noexcept { static InfoImpl result; return result; }

			inline bool isInfoValid() noexcept { return true; }

			namespace Info
			{
				inline SizeT getColumnCount() noexcept	{ return getInfoImpl().columns; }
				inline SizeT getRowCount() noexcept		{ return getInfoImpl().rows; }
			}
		}
	}
}

#endif
