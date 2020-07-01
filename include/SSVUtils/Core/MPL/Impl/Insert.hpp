// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_INSERT
#define SSVU_CORE_MPL_IMPL_INSERT

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

namespace ssvu
{
namespace MPL
{
namespace Impl
{
inline constexpr std::size_t getMapIns(
    std::size_t mI, std::size_t mFrom, std::size_t mTo) noexcept
{
    return mI < mTo ? mI : mI == mTo ? mFrom : mI - 1;
}

template <typename, typename, std::size_t, typename...>
struct Insert;

template <std::size_t... TIs, typename T, std::size_t TN, typename... Ts>
struct Insert<IdxSeq<TIs...>, T, TN, Ts...>
{
    SSVU_ASSERT_STATIC(TN <= sizeof...(Ts),
        "Insert index smaller or equal to the size of the list");
    using Type = List<std::tuple_element_t<getMapIns(TIs, sizeof...(Ts), TN),
        Tpl<Ts..., T>>...>;
};
} // namespace Impl
} // namespace MPL
} // namespace ssvu

#endif
