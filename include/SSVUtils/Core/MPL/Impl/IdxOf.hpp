// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_IDXOF
#define SSVU_CORE_MPL_IMPL_IDXOF

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

namespace ssvu
{
	namespace MPL
	{
		namespace Impl
		{
			template<typename T, typename... Ts> struct IdxOf;
			template<typename T, typename... Ts> struct IdxOf<T, T, Ts...> : CTInt<0>{ };
			template<typename T, typename TTail, typename... Ts> struct IdxOf<T, TTail, Ts...> : CTInt<1 + IdxOf<T, Ts...>::value>{ };
		}
	}
}

#endif
