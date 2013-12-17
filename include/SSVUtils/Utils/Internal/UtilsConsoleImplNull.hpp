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
			inline const std::string& getStrResetFmt() noexcept		{ return ""; }
			inline const std::string& getStrStyle(Style) noexcept	{ return ""; }
			inline const std::string& getStrColorFG(Color) noexcept	{ return ""; }
			inline const std::string& getStrColorBG(Color) noexcept	{ return ""; }
		}
	}
}

#endif