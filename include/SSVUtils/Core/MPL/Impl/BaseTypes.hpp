// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_BASETYPES
#define SSVU_CORE_MPL_IMPL_BASETYPES

#include <utility>
#include <type_traits>

namespace ssvu
{
namespace MPL
{
template <typename...>
struct List;
template <typename T, T... Ts>
using ListIC = List<std::integral_constant<T, Ts>...>;
template <int... Ts>
using ListInt = ListIC<int, Ts...>;

namespace Impl
{
template <typename T>
struct IsList : std::false_type
{
};
template <typename... Ts>
struct IsList<List<Ts...>> : std::true_type
{
};
} // namespace Impl
} // namespace MPL
} // namespace ssvu

#endif
