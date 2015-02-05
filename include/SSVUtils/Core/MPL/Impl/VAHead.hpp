// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_VAHEAD
#define SSVU_CORE_MPL_IMPL_VAHEAD

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

namespace ssvu
{
	namespace MPL
	{
		namespace Impl
		{
			template<typename... Ts> struct VAHeadHlpr;
			template<typename T, typename... Ts> struct VAHeadHlpr<T, Ts...>	{ using Type = T; };
			template<> struct VAHeadHlpr<>										{ using Type = Null; };

			template<typename... Ts> using VAHead = typename Impl::VAHeadHlpr<Ts...>::Type;
		}
	}
}

#endif
