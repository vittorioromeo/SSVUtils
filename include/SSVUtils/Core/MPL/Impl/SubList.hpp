// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_SUBLIST
#define SSVU_CORE_MPL_IMPL_SUBLIST

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

namespace ssvu
{
	namespace MPL
	{
		namespace Impl
		{
			template<int, int, int, typename, typename, bool> struct SliceImpl;
			template<int, int, int, typename, typename> struct SliceDispatch;
			template<int TS1, int TS2, int TSC, typename... TLA1s, typename... TLA2s> struct SliceImpl<TS1, TS2, TSC, List<TLA1s...>, List<TLA2s...>, false>
			{
				using Type = typename SliceDispatch<TS1, TS2, TSC + 1, List<TLA1s...>, typename List<TLA2s...>::template PushBack<typename List<TLA1s...>::template At<TSC>>>::Type;
			};
			template<int TS1, int TS2, int TSC, typename... TLA1s, typename... TLA2s> struct SliceImpl<TS1, TS2, TSC, List<TLA1s...>, List<TLA2s...>, true>
			{
				using Type = List<TLA2s...>;
			};
			template<int TS1, int TS2, int TSC, typename TL1, typename TL2> struct SliceDispatch
			{
				using Type = typename SliceImpl<TS1, TS2, TSC, TL1, TL2, TSC == TS2 || TSC >= int(TL1::size)>::Type;
			};
			template<int, int, int, typename> struct SliceHlpr;
			template<int TS1, int TS2, int TSC, typename... TLAs> struct SliceHlpr<TS1, TS2, TSC, List<TLAs...>>
			{
				using Type = typename SliceDispatch<TS1, TS2, TSC, List<TLAs...>, List<>>::Type;
			};
		}
	}
}

#endif
