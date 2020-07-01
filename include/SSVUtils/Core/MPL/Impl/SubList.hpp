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
template <typename TL, typename TIdxs, int TS>
struct SliceHlpr;
template <typename TL, std::size_t... TIdxs, int TS>
struct SliceHlpr<TL, std::index_sequence<TIdxs...>, TS>
{
    using Type = List<typename TL::template At<TS + TIdxs>...>;
};

template <typename TL>
inline constexpr int clampListIdx(int mX) noexcept
{
    return getClamped(mX, 0, TL::size);
}
template <typename TL, int TS1, int TS2>
using Slice = typename SliceHlpr<TL,
    std::make_index_sequence<clampListIdx<TL>(TS2) - clampListIdx<TL>(TS1)>,
    clampListIdx<TL>(TS1)>::Type;
} // namespace Impl
} // namespace MPL
} // namespace ssvu

#endif
