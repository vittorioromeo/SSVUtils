// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_CONSOLEFMT_IMPL
#define SSVU_CORE_CONSOLEFMT_IMPL

// This file contains an Unix implementation for console formatting.
// Everything returns ANSI formatting codes.

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

namespace ssvu
{
	namespace Console
	{
		namespace Internal
		{
			constexpr const char* prefix{"\033["};
			constexpr const char* postfix{"m"};
			constexpr const char* clear{"\033[1;1H\033[2J"};

			inline auto& getStyleCodes() noexcept
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

				return codes;
			}
			inline auto& getColorFGCodes() noexcept
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

				return codes;
			}
			inline auto& getColorBGCodes() noexcept
			{
				static int codes[]
				{
					49,		// 0 = Color::Default
					40,		// 1 = Color::Black
					41,		// 2 = Color::Red
					42,		// 3 = Color::Green
					43,		// 4 = Color::Yellow
					44,		// 5 = Color::Blue
					45,		// 6 = Color::Magenta
					46,		// 7 = Color::Cyan
					47,		// 8 = Color::LightGray
					100,	// 9 = Color::DarkGray
					101,	// 10 = Color::LightRed
					102,	// 11 = Color::LightGreen
					103,	// 12 = Color::LightYellow
					104,	// 13 = Color::LightBlue
					105,	// 14 = Color::LightMagenta
					106,	// 15 = Color::LightCyan
					107		// 16 = Color::LightWhite
				};

				return codes;
			}

			struct FmtCache
			{
				Style lastStyle{Style::None};
				Color lastColorFG{Color::Default}, lastColorBG{Color::Default};
				std::vector<std::string> styleCodeStrs, colorFGCodeStrs, colorBGCodeStrs;
				std::string strClear{clear};

				inline FmtCache()
				{
					for(const auto& i : getStyleCodes())	styleCodeStrs.emplace_back(toStr(i));
					for(const auto& i : getColorFGCodes())	colorFGCodeStrs.emplace_back(toStr(i));
					for(const auto& i : getColorBGCodes())	colorBGCodeStrs.emplace_back(toStr(i));
				}
			};

			inline auto& getFmtCache() noexcept { static FmtCache result; return result; }

			inline auto& getLastStyle() noexcept	{ return getFmtCache().lastStyle; }
			inline auto& getLastColorFG() noexcept	{ return getFmtCache().lastColorFG; }
			inline auto& getLastColorBG() noexcept	{ return getFmtCache().lastColorBG; }

			inline const auto& getStyleStr(Style mStyle) noexcept	{ return getFmtCache().styleCodeStrs[int(mStyle)]; }
			inline const auto& getColorFGStr(Color mColor) noexcept	{ return getFmtCache().colorFGCodeStrs[int(mColor)]; }
			inline const auto& getColorBGStr(Color mColor) noexcept	{ return getFmtCache().colorBGCodeStrs[int(mColor)]; }

			inline const auto& getFmtStr() noexcept
			{
				static IgnoreManip result;
				result = {prefix + getStyleStr(getLastStyle()) + ";" + getColorFGStr(getLastColorFG()) + ";" + getColorBGStr(getLastColorBG()) + postfix};
				return result;
			}

			inline const auto& getStrResetFmt() noexcept
			{
				getLastStyle() = Style::None;
				getLastColorFG() = getLastColorBG() = Color::Default;
				return getFmtStr();
			}
			inline const auto& getStrStyle(Style mStyle) noexcept	{ getLastStyle() = mStyle;		return getFmtStr(); }
			inline const auto& getStrColorFG(Color mColor) noexcept	{ getLastColorFG() = mColor;	return getFmtStr(); }
			inline const auto& getStrColorBG(Color mColor) noexcept	{ getLastColorBG() = mColor;	return getFmtStr(); }
			inline const auto& getStrClear() noexcept				{ return getFmtCache().strClear; }

			struct InfoImpl
			{
				SizeT columns, rows;

				inline InfoImpl()
				{
					struct winsize w;
					ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
					columns = w.ws_col;
					rows = w.ws_row;
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
