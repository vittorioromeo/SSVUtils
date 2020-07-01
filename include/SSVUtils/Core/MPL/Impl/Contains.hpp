// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_CONTAINS
#define SSVU_CORE_MPL_IMPL_CONTAINS

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

namespace ssvu
{
namespace MPL
{
namespace Impl
{
template <typename T, typename... Ts>
struct Contains : std::false_type
{
};

template <typename T, typename THead, typename... Ts>
struct Contains<T, THead, Ts...>
    : std::conditional_t<std::is_same_v<T, THead>, std::true_type, Contains<T, Ts...>>
{
};
} // namespace Impl
} // namespace MPL
} // namespace ssvu

#endif
