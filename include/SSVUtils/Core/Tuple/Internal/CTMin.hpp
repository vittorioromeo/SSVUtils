// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_TUPLE_INTERNAL_CTMIN
#define SSVU_CORE_TUPLE_INTERNAL_CTMIN

#include "SSVUtils/Core/Common/Common.hpp"

namespace ssvu
{
	namespace Internal
	{
		// TODO: to (new) mpl module along with CTUtils?
		template<SizeT...> struct CTMin;
		template<SizeT TV0> struct CTMin<TV0> : IntegralConstant<SizeT, TV0> { };
		template<SizeT TV0, SizeT TV1, SizeT... TVs> struct CTMin<TV0, TV1, TVs...> : CTMin<(TV0 > TV1) ? TV1 : TV0, TVs...> { };
		template<SizeT... TVs> inline constexpr SizeT getCTMin() noexcept { return CTMin<TVs...>::value; }
	}
}

#endif
