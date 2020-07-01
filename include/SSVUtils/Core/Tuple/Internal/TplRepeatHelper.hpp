// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_TUPLE_INTERNAL_TPLREPEATHELPER
#define SSVU_CORE_TUPLE_INTERNAL_TPLREPEATHELPER

#include <tuple>
#include "SSVUtils/Core/Common/Common.hpp"

namespace ssvu
{
namespace Impl
{
template <typename, std::size_t, typename>
struct TplRepeatImplHelper;

template <typename T, std::size_t TS, typename... TArgs>
struct TplRepeatImplHelper<T, TS, Tpl<TArgs...>>
{
    using Type =
        typename TplRepeatImplHelper<T, TS - 1, Tpl<TArgs..., T>>::Type;
};

template <typename T, typename... TArgs>
struct TplRepeatImplHelper<T, 0, Tpl<TArgs...>>
{
    using Type = Tpl<TArgs...>;
};

template <typename T, std::size_t TS>
struct TplRepeatImpl
{
    using Type = typename TplRepeatImplHelper<T, TS, Tpl<>>::Type;
};
} // namespace Impl
} // namespace ssvu

#endif
