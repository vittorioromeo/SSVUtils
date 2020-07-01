// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_CONTAINS
#define SSVU_CORE_MPL_IMPL_CONTAINS

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
constexpr bool containsImpl() noexcept
{
    constexpr std::array<bool, sizeof...(Ts)> res{std::is_same_v<T, Ts>...};

    for(bool b : res)
    {
        if(b)
        {
            return true;
        }
    }

    return false;
}

template <typename T, typename... Ts>
struct Contains : std::bool_constant<containsImpl<T, Ts...>()>
{
};

} // namespace Impl
} // namespace MPL
} // namespace ssvu

#endif
