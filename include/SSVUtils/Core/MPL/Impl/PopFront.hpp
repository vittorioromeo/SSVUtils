// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_POPFRONT
#define SSVU_CORE_MPL_IMPL_POPFRONT

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

namespace ssvu
{
	namespace MPL
	{
		namespace Impl
		{
			template<typename> struct PopFrontHlpr;
			template<typename TL, typename... TLAs> struct PopFrontHlpr<List<TL, TLAs...>>	{ using Type = List<TLAs...>; };
			template<> struct PopFrontHlpr<List<>>											{ using Type = List<>; };
		}
	}
}

#endif
