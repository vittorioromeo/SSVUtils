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

template <bool TAllTrue, int TStart>
struct Matches;
template <int TStart>
struct Matches<true, TStart>
{
    using Type = ListInt<TStart>;
};
template <int TStart>
struct Matches<false, TStart>
{
    using Type = ListInt<>;
};

template <typename TS, typename TM, int TStart, typename TMIdxs>
struct GetMatch;
template <typename TS, typename TM, int TStart, std::size_t... TMIdxs>
struct GetMatch<TS, TM, TStart, std::index_sequence<TMIdxs...>>
{
    using Type = typename Matches<
        (std::is_same_v<typename TS::template At<TStart + TMIdxs>,
             typename TM::template At<TMIdxs>> &&
            ...),
        TStart>::Type;
};


template <typename TR, typename... TLs>
struct ConcatTest;
template <typename TR, typename TL, typename... TLs>
struct ConcatTest<TR, TL, TLs...>
{
    using Type = typename TR::template Append<TL>::template Append<
        typename ConcatTest<TR, TLs...>::Type>;
};
template <typename TR>
struct ConcatTest<TR>
{
    using Type = TR;
};

template <typename TS, typename TM, typename TSIdxs, typename TMIdxs>
struct IdxsOfSeqHlpr;
template <typename TS, typename TM, std::size_t... TSIdxs, typename TMIdxs>
struct IdxsOfSeqHlpr<TS, TM, std::index_sequence<TSIdxs...>, TMIdxs>
{
    using Type = typename ConcatTest<ListInt<>,
        typename GetMatch<TS, TM, TSIdxs, TMIdxs>::Type...>::Type;
};
template <typename TS, std::size_t... TSIdxs, typename TMIdxs>
struct IdxsOfSeqHlpr<TS, List<>, std::index_sequence<TSIdxs...>, TMIdxs>
{
    using Type = ListInt<>;
};



template <typename TS, typename TM>
using IdxsOfSeq = typename IdxsOfSeqHlpr<TS, TM,
    std::make_index_sequence<getClampedMin(int(TS::size - TM::size + 1), 0)>,
    std::make_index_sequence<TM::size>>::Type;



template <typename, typename, typename, typename, typename, int>
struct ReplaceAllOfSeqHlpr;

template <typename TS, typename TM, typename TN, typename TR, int TI>
struct ReplaceAllOfSeqHlpr<TS, TM, TN, TR, ListInt<>, TI>
{
    using SliceLast = typename TS::template Slice<TI, TS::size>;
    using Type = typename TR::template Append<SliceLast>;
};

template <typename TS, typename TM, typename TN, typename TR, int TILast,
    int TI, int... TIs>
struct ReplaceAllOfSeqHlpr<TS, TM, TN, TR, ListInt<TI, TIs...>, TILast>
{
    using SliceBefore = typename TS::template Slice<TILast, TI>;

    using Append1 = typename TR::template Append<SliceBefore>;
    using Append2 = typename Append1::template Append<TN>;

    using Type = typename ReplaceAllOfSeqHlpr<TS, TM, TN, Append2,
        ListInt<TIs...>, TI + TM::size>::Type;
};

template <typename TS, typename TM, typename TN>
using ReplaceAllOfSeq = typename ReplaceAllOfSeqHlpr<TS, TM, TN, List<>,
    IdxsOfSeq<TS, TM>, 0>::Type;
} // namespace Impl
} // namespace MPL
} // namespace ssvu

#endif
