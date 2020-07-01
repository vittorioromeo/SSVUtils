// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_REMOVE
#define SSVU_CORE_MPL_IMPL_REMOVE

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

namespace ssvu
{
namespace MPL
{
namespace Impl
{
inline constexpr std::size_t getMapRem(
    std::size_t mI, std::size_t mIdx) noexcept
{
    return mI < mIdx ? mI : mI + 1;
}

template <typename, std::size_t, typename...>
struct RemoveHlpr;

template <std::size_t... TIs, std::size_t TN, typename... Ts>
struct RemoveHlpr<IdxSeq<TIs...>, TN, Ts...>
{
    using Type = List<std::tuple_element_t<getMapRem(TIs, TN), Tpl<Ts...>>...>;
};

template <std::size_t TN, typename... Ts>
struct Remove
{
    SSVU_ASSERT_STATIC(
        TN < sizeof...(Ts), "Remove index smaller than the size of the list");
    using Type =
        typename RemoveHlpr<MkIdxSeq<sizeof...(Ts) - 1>, TN, Ts...>::Type;
};
} // namespace Impl
} // namespace MPL
} // namespace ssvu

#endif
