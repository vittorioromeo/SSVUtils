// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_UNIQUE
#define SSVU_CORE_MPL_IMPL_UNIQUE

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

namespace ssvu
{
	namespace MPL
	{
		namespace Impl
		{
			template<typename, typename> struct UniqueHlpr;
			template<typename TResult> struct UniqueHlpr<List<>, TResult>
			{
				using Type = TResult;
			};
			template<typename T, typename... T1s, typename... T2s> struct UniqueHlpr<List<T, T1s...>, List<T2s...>>
			{
				using CurrentList = typename UniqueHlpr<List<T1s...>, List<T2s...>>::Type;
				using AddedList = typename UniqueHlpr<List<T1s...>, List<T2s..., T>>::Type;

				using Type = Conditional<List<T2s...>::template has<T>(), CurrentList, AddedList>;
			};

			template<typename... Ts> using Unique = typename UniqueHlpr<List<Ts...>, List<>>::Type;


			// TODO: move
			template<template<typename> class, typename, typename> struct FilterHlpr;
			template<template<typename> class TFilter, typename TResult> struct FilterHlpr<TFilter, List<>, TResult>
			{
				using Type = TResult;
			};
			template<template<typename> class TFilter, typename T, typename... T1s, typename TResult> struct FilterHlpr<TFilter, List<T, T1s...>, TResult>
			{
				using CurrentList = typename FilterHlpr<TFilter, List<T1s...>, TResult>::Type;
				using AddedList = typename FilterHlpr<TFilter, List<T1s...>, typename TResult::template PushBack<T>>::Type;

				using Type = Conditional<!TFilter<T>{}(), CurrentList, AddedList>;
			};

			template<template<typename> class TFilter, typename... Ts> using Filter = typename FilterHlpr<TFilter, List<Ts...>, List<>>::Type;
		}
	}
}

#endif
