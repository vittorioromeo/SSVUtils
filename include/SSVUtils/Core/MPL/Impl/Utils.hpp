// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_UTILS
#define SSVU_CORE_MPL_IMPL_UTILS

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"
#include "SSVUtils/Core/MPL/Impl/List.hpp"

namespace ssvu
{
	namespace MPL
	{
		// List typedef shortcuts
		template<typename... Ts> using Head = typename List<Ts...>::Head;
		template<typename... Ts> using Tail = typename List<Ts...>::Tail;
		template<int TS, typename... Ts> using At = typename List<Ts...>::template At<TS>;

		// List function shortcuts
		template<typename... Ts> inline constexpr auto getSize() noexcept						{ return List<Ts...>::size; }
		template<typename... Ts> inline constexpr auto isEmpty() noexcept						{ return List<Ts...>::empty; }
		template<typename... Ts> inline constexpr auto getMaxSize() noexcept					{ return List<Ts...>::typeMaxSize; }
		template<typename... Ts> inline constexpr auto getMaxAlign() noexcept					{ return List<Ts...>::typeMaxAlign; }
		template<typename T, typename... Ts> inline constexpr auto has() noexcept				{ return List<Ts...>::template has<T>(); }
		template<typename T, typename... Ts> inline constexpr auto getCountOf() noexcept		{ return List<Ts...>::template getCountOf<T>(); }
		template<typename... Ts> inline constexpr bool isUnique() noexcept						{ return List<Ts...>::unique; }
		template<template<typename> class TFilter, typename... Ts> inline constexpr auto all()	{ return List<Ts...>::template all<TFilter>(); }
		template<template<typename> class TFilter, typename... Ts> inline constexpr auto any()	{ return List<Ts...>::template any<TFilter>(); }
		template<typename T, typename... Ts> inline constexpr auto getIdxOf() noexcept			{ return List<Ts...>::template getIdxOf<T>(); }
	}
}

#endif
