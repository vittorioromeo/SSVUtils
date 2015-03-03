// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_BASETYPES
#define SSVU_CORE_MPL_IMPL_BASETYPES

#include "SSVUtils/Core/Common/Common.hpp"

namespace ssvu
{
	namespace MPL
	{
		template<typename...> struct List;
		template<typename T, T... Ts> using ListIC = List<CTVal<T, Ts>...>;
		template<int... Ts> using ListInt = ListIC<int, Ts...>;

		namespace Impl
		{
			template<typename T> struct IsList : FalseT { };
			template<typename... Ts> struct IsList<List<Ts...>> : TrueT { };
		}
	}
}

#endif
