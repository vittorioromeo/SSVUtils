// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_APPEND
#define SSVU_CORE_MPL_IMPL_APPEND

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

namespace ssvu
{
namespace MPL
{
namespace Impl
{
template <typename, typename>
struct Append;
template <typename... Ts1, typename... Ts2>
struct Append<List<Ts1...>, List<Ts2...>>
{
    using Type = List<Ts1..., Ts2...>;
};
} // namespace Impl
} // namespace MPL
} // namespace ssvu

#endif
