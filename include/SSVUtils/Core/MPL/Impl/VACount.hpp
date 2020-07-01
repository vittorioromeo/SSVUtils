// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_VACOUNT
#define SSVU_CORE_MPL_IMPL_VACOUNT

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

namespace ssvu
{
namespace MPL
{
namespace Impl
{
template <typename, typename...>
struct VACountHlpr;
template <typename T, typename T1, typename... Ts>
struct VACountHlpr<T, T1, Ts...>
{
    static constexpr std::size_t value{
        (isSame<T, T1>() ? std::size_t(1) : std::size_t(0)) +
        VACountHlpr<T, Ts...>::value};
};

template <typename T>
struct VACountHlpr<T>
{
    static constexpr std::size_t value{0};
};

template <typename T, typename... Ts>
inline constexpr std::size_t getCountOf()
{
    return VACountHlpr<T, Ts...>::value;
}
} // namespace Impl
} // namespace MPL
} // namespace ssvu

#endif
