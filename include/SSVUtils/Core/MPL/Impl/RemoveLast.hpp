// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_REMOVELAST
#define SSVU_CORE_MPL_IMPL_REMOVELAST

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"
#include "SSVUtils/Core/MPL/Impl/PopFront.hpp"

namespace ssvu
{
	namespace MPL
	{
		namespace Impl
		{
			template<typename, typename> struct RemoveLastHlpr;
			template<typename TLA1, typename TLA2, typename... TLAs1, typename... TLAs2> struct RemoveLastHlpr<List<TLA1, TLA2, TLAs1...>, List<TLAs2...>>
			{
				using List1 = List<TLA1, TLA2, TLAs1...>;
				using List2 = List<TLAs2...>;
				using List1WithPop = typename PopFrontHlpr<List1>::Type;
				using List2WithPush = typename List2::template PushBack<typename List1::Head>;
				using Type = typename RemoveLastHlpr<List1WithPop, List2WithPush>::Type;
			};
			template<typename TL1, typename... TLAs2> struct RemoveLastHlpr<List<TL1>, List<TLAs2...>> { using Type = List<TLAs2...>; };
		}
	}
}

#endif
