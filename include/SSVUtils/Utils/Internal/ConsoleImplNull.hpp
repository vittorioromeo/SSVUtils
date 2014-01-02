// Copyright (c) 2013-2014 Vittorio Romeo
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
			inline const std::string& getStrResetFmt() noexcept		{ return getEmptyString(); }
			inline const std::string& getStrStyle(Style) noexcept	{ return getEmptyString(); }
			inline const std::string& getStrColorFG(Color) noexcept	{ return getEmptyString(); }
			inline const std::string& getStrColorBG(Color) noexcept	{ return getEmptyString(); }
			inline const std::string& getStrClear() noexcept		{ return getEmptyString(); }
		}
	}
}

#endif
