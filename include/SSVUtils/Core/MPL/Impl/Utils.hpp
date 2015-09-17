// Copyright (c) 2013-2015 Vittorio Romeo
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
		template<typename... Ts>
		using Head = typename List<Ts...>::Head;

		template<typename... Ts>
		using Tail = typename List<Ts...>::Tail;

		template<int TS, typename... Ts>
		using At = typename List<Ts...>::template At<TS>;

		template<template<typename> class TFilter, typename... Ts>
		using Filter = typename List<Ts...>::template Filter<TFilter>;

		template<typename TFilter, typename... Ts>
		using FilterIdx = typename List<Ts...>::template FilterIdx<TFilter>;

		// template<template<typename...> class T, typename TL>
		// using Rename = typename TL::template Rename<T>;

		template<template<typename> class TApply, typename... Ts>
		using Apply = typename List<Ts...>::template Apply<TApply>;

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

		// TODO: refactor
		namespace L
		{
			template<typename TL>
			using Head = typename TL::Head;

			template<typename TL>
			using Tail = typename TL::Tail;

			template<int TS, typename TL>
			using At = typename TL::template At<TS>;

			template<template<typename> class TFilter, typename TL>
			using Filter = typename TL::template Filter<TFilter>;

			template<typename TFilter, typename TL>
			using FilterIdx = typename TL::template FilterIdx<TFilter>;

			template<template<typename...> class T, typename TL>
			using Rename = typename TL::template Rename<T>;

			template<template<typename> class TApply, typename TL>
			using Apply = typename TL::template Apply<TApply>;

			template<typename TL> inline constexpr auto getSize() noexcept						{ return TL::size; }
			template<typename TL> inline constexpr auto isEmpty() noexcept						{ return TL::empty; }
			template<typename TL> inline constexpr auto getMaxSize() noexcept					{ return TL::typeMaxSize; }
			template<typename TL> inline constexpr auto getMaxAlign() noexcept					{ return TL::typeMaxAlign; }
			template<typename T, typename TL> inline constexpr auto has() noexcept				{ return TL::template has<T>(); }
			template<typename T, typename TL> inline constexpr auto getCountOf() noexcept		{ return TL::template getCountOf<T>(); }
			template<typename TL> inline constexpr bool isUnique() noexcept						{ return TL::unique; }
			template<template<typename> class TFilter, typename TL> inline constexpr auto all()	{ return TL::template all<TFilter>(); }
			template<template<typename> class TFilter, typename TL> inline constexpr auto any()	{ return TL::template any<TFilter>(); }
			template<typename T, typename TL> inline constexpr auto getIdxOf() noexcept			{ return TL::template getIdxOf<T>(); }
		}
	}
}

#endif
