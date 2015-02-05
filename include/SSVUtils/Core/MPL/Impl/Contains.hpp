// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_CONTAINS
#define SSVU_CORE_MPL_IMPL_CONTAINS

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

namespace ssvu
{
	namespace MPL
	{
		namespace Impl
		{
			template<typename T, typename... Ts> struct ContainsHlpr : TrueT { };

			template<typename T, typename THead, typename... Ts> struct ContainsHlpr<T, THead, Ts...>
				: Conditional<isSame<T, THead>(), TrueT, ContainsHlpr<T, Ts...>> { };

			template<typename T> struct ContainsHlpr<T> : FalseT { };
		}
	}
}

#endif
