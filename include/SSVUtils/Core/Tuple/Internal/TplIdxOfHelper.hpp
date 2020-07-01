// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_TUPLE_INTERNAL_TPLIDXOFHELPER
#define SSVU_CORE_TUPLE_INTERNAL_TPLIDXOFHELPER

#include <tuple>
#include "SSVUtils/Core/Common/Common.hpp"

namespace ssvu
{
namespace Impl
{
template <typename T, typename TTpl>
struct TplIdxOf;

template <typename T, typename... TTypes>
struct TplIdxOf<T, std::tuple<T, TTypes...>>
{
    static constexpr std::size_t value{0u};
};

template <typename T1, typename T2, typename... TTypes>
struct TplIdxOf<T1, std::tuple<T2, TTypes...>>
{
    static constexpr std::size_t value{1 + TplIdxOf<T1, std::tuple<TTypes...>>::value};
};
} // namespace Impl
} // namespace ssvu

#endif
