// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_VATAIL
#define SSVU_CORE_MPL_IMPL_VATAIL

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

namespace ssvu
{
	namespace MPL
	{
		namespace Impl
		{
			template<typename...> struct VATailHlpr;
			template<typename T, typename... Ts> struct VATailHlpr<T, Ts...>	{ using Type = typename VATailHlpr<Ts...>::Type; };
			template<typename T> struct VATailHlpr<T>							{ using Type = T; };

			template<typename... Ts> using VATail = typename Impl::VATailHlpr<Ts...>::Type;
		}
	}
}

#endif
