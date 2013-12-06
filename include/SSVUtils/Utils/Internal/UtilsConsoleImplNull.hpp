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
			inline const std::string& getStrResetFmt() noexcept				{ return ""; }
			inline const std::string& getStrStyle(Style mStyle) noexcept	{ return ""; }
			inline const std::string& getStrColorFG(Color mColor) noexcept	{ return ""; }
			inline const std::string& getStrColorBG(Color mColor) noexcept	{ return ""; }
		}
	}
}

#endif
