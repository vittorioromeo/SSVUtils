// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_IDXSOFSEQ
#define SSVU_CORE_MPL_IMPL_IDXSOFSEQ

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

namespace ssvu
{
	namespace MPL
	{
		namespace Impl
		{
			template<typename, typename, typename TR, int, bool> struct IdxsOfSeqHelper
			{
				using Type = TR;
			};

			template<typename TS1, typename TM1, typename... Ts, typename... Tm, int... Tr, int TI>
			struct IdxsOfSeqHelper<List<TS1, Ts...>, List<TM1, Tm...>, ListInt<Tr...>, TI, true>
			{
				using LS = List<TS1, Ts...>;
				using LM = List<TM1, Tm...>;
				using LR = ListInt<Tr...>;
				using LSSlice = typename LS::template Slice<TI, TI + LM::size>;
				using LRPushed = typename LR::template PushBack<CTInt<TI>>;
				using LRNext = Conditional<isSame<LSSlice, LM>(), LRPushed, LR>;
				static constexpr bool bln{TI <= LS::size - LM::size};
				using Type = typename IdxsOfSeqHelper<LS, LM, LRNext, TI + 1, bln>::Type;
			};

			template<typename TS, typename TM> using IdxsOfSeq = typename IdxsOfSeqHelper<TS, TM, ListInt<>, 0, true>::Type;



			template<typename, typename, typename, typename, typename, int> struct ReplaceAllOfSeqHelper;

			template<typename TS, typename TM, typename TN, typename TR, int TI>
			struct ReplaceAllOfSeqHelper<TS, TM, TN, TR, ListInt<>, TI>
			{
				using SliceLast = typename TS::template Slice<TI, TS::size>;
				using Type = typename TR::template Append<SliceLast>;
			};

			template<typename TS, typename TM, typename TN, typename TR, int TILast, int TI, int... TIs>
			struct ReplaceAllOfSeqHelper<TS, TM, TN, TR, ListInt<TI, TIs...>, TILast>
			{
				using SliceBefore = typename TS::template Slice<TILast, TI>;

				using Append1 = typename TR::template Append<SliceBefore>;
				using Append2 = typename Append1::template Append<TN>;

				using Type = typename ReplaceAllOfSeqHelper<TS, TM, TN, Append2, ListInt<TIs...>, TI + TM::size>::Type;
			};

			template<typename TS, typename TM, typename TN> using ReplaceAllOfSeq = typename ReplaceAllOfSeqHelper<TS, TM, TN, List<>, IdxsOfSeq<TS, TM>, 0>::Type;
		}
	}
}

#endif
