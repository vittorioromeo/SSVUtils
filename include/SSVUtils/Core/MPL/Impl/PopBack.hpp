// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_POPBACK
#define SSVU_CORE_MPL_IMPL_POPBACK

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"
#include "SSVUtils/Core/MPL/Impl/RemoveLast.hpp"

namespace ssvu
{
	namespace MPL
	{
		namespace Impl
		{
			template<typename> struct PopBackHlpr;
			template<typename... TLAs> struct PopBackHlpr<List<TLAs...>> { using Type = typename RemoveLastHlpr<List<TLAs...>, List<>>::Type; };
			template<> struct PopBackHlpr<List<>> { using Type = List<>; };
		}
	}
}

#endif
