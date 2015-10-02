// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_MINMAX
#define SSVU_CORE_MPL_IMPL_MINMAX

#include "SSVUtils/Core/Common/Common.hpp"

namespace ssvu
{
namespace MPL
{
    template <typename T>
    inline constexpr T getMin(T mA) noexcept
    {
        return mA;
    }
    template <typename T>
    inline constexpr T getMin(T mA, T mB) noexcept
    {
        return mA < mB ? mA : mB;
    }
    template <typename T, typename... Ts>
    inline constexpr T getMin(T mA, T mB, Ts... mX) noexcept
    {
        return getMin<T>(mA, getMin<T>(mB, mX...));
    }

    template <typename T>
    inline constexpr T getMax(T mA) noexcept
    {
        return mA;
    }
    template <typename T>
    inline constexpr T getMax(T mA, T mB) noexcept
    {
        return mA > mB ? mA : mB;
    }
    template <typename T, typename... Ts>
    inline constexpr T getMax(T mA, T mB, Ts... mX) noexcept
    {
        return getMax<T>(mA, getMax<T>(mB, mX...));
    }

    template <typename T, T TN0>
    inline constexpr T getMin() noexcept
    {
        return TN0;
    }
    template <typename T, T TN0, T TN1>
    inline constexpr T getMin() noexcept
    {
        return TN0 < TN1 ? TN0 : TN1;
    }
    template <typename T, T TN0, T TN1, T... Ts>
    inline constexpr T getMin() noexcept
    {
        return getMin<T, TN0, getMin<TN1, Ts...>()>();
    }

    template <typename T, T TN0>
    inline constexpr T getMax() noexcept
    {
        return TN0;
    }
    template <typename T, T TN0, T TN1>
    inline constexpr T getMax() noexcept
    {
        return TN0 > TN1 ? TN0 : TN1;
    }
    template <typename T, T TN0, T TN1, T... Ts>
    inline constexpr T getMax() noexcept
    {
        return getMax<T, TN0, getMax<TN1, Ts...>()>();
    }
}
}

#endif
