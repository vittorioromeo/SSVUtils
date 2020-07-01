// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_IDXOF
#define SSVU_CORE_MPL_IMPL_IDXOF

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

#include <array>
#include <type_traits>

namespace ssvu
{
namespace MPL
{
namespace Impl
{

template <typename T, typename... Ts>
constexpr std::size_t idxOfImpl() noexcept
{
    constexpr std::array<std::size_t, sizeof...(Ts)> res{
        std::is_same_v<T, Ts>...};

    for(std::size_t i = 0; i < sizeof...(Ts); ++i)
    {
        if(res[i])
        {
            return i;
        }
    }

    return -1;
}

template <typename T, typename... Ts>
struct IdxOf : std::integral_constant<std::size_t, idxOfImpl<T, Ts...>()>
{
};

} // namespace Impl
} // namespace MPL
} // namespace ssvu

#endif
