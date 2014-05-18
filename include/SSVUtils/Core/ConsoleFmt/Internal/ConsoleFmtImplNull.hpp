// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_CONSOLEFMT_INTERNAL_IMPL
#define SSVU_CORE_CONSOLEFMT_INTERNAL_IMPL

// This file contains a null implementation for console formatting.
// Everything returns an empty string.

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
			inline bool isInfoValid() noexcept						{ return false; }

			namespace Info
			{
				inline std::size_t getColumnCount() noexcept	{ return 80; }
				inline std::size_t getRowCount() noexcept		{ return 50; }
			}
		}
	}
}

#endif
