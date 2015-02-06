// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_BASETYPES
#define SSVU_CORE_MPL_IMPL_BASETYPES

#include "SSVUtils/Core/Common/Common.hpp"

namespace ssvu
{
	namespace MPL
	{
		template<typename...> struct List;

		struct Null { };
		template<bool TCond> using BoolResult = Conditional<TCond, TrueT, FalseT>;

		template<typename T> inline constexpr T getMin(T mA) noexcept { return mA; }
		template<typename T> inline constexpr T getMin(T mA, T mB) noexcept { return mA < mB ? mA : mB; }
		template<typename T, typename... TArgs> inline constexpr T getMin(T mA, T mB, TArgs... mX) noexcept { return getMin<T>(mA, getMin<T>(mB, mX...)); }

		template<typename T> inline constexpr T getMax(T mA) noexcept { return mA; }
		template<typename T> inline constexpr T getMax(T mA, T mB) noexcept { return mA > mB ? mA : mB; }
		template<typename T, typename... TArgs> inline constexpr T getMax(T mA, T mB, TArgs... mX) noexcept { return getMax<T>(mA, getMax<T>(mB, mX...)); }

		template<typename T, T TN0> inline constexpr T getMin() noexcept { return TN0; }
		template<typename T, T TN0, T TN1> inline constexpr T getMin() noexcept { return TN0 < TN1 ? TN0 : TN1; }
		template<typename T, T TN0, T TN1, T... TArgs> inline constexpr T getMin() noexcept { return getMin<T, TN0, getMin<T, TN1, TArgs...>()>(); }

		template<typename T, T TN0> inline constexpr T getMax() noexcept { return TN0; }
		template<typename T, T TN0, T TN1> inline constexpr T getMax() noexcept { return TN0 > TN1 ? TN0 : TN1; }
		template<typename T, T TN0, T TN1, T... TArgs> inline constexpr T getMax() noexcept { return getMax<T, TN0, getMax<T, TN1, TArgs...>()>(); }
	}
}

#endif
