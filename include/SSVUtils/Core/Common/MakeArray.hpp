// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_MAKEARRAY
#define SSVU_CORE_COMMON_MAKEARRAY

// C++14/C++17: hopefully will be in standard
namespace ssvu
{
	template<typename... TArgs> constexpr auto make_array(TArgs&&... mArgs)
	{
		return std::array<Decay<Common<TArgs...>>, sizeof...(TArgs)>{fwd<TArgs>(mArgs)...};
	}
}

#endif
