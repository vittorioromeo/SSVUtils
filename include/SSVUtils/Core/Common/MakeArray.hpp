// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_MAKEARRAY
#define SSVU_CORE_COMMON_MAKEARRAY

// C++14/C++17: hopefully will be in standard
namespace ssvu
{
	template<typename... TArgs> inline constexpr auto mkArray(TArgs&&... mArgs)
		noexcept(noexcept(std::array<Decay<Common<TArgs...>>, sizeof...(TArgs)>{{FWD(mArgs)...}}))
	{
		return std::array<Decay<Common<TArgs...>>, sizeof...(TArgs)>{{FWD(mArgs)...}};
	}
}

#endif
