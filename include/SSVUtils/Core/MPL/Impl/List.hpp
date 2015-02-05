// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_LIST
#define SSVU_CORE_MPL_IMPL_LIST

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"
#include "SSVUtils/Core/MPL/Impl/VAHead.hpp"
#include "SSVUtils/Core/MPL/Impl/VATail.hpp"
#include "SSVUtils/Core/MPL/Impl/PopFront.hpp"
#include "SSVUtils/Core/MPL/Impl/RemoveLast.hpp"
#include "SSVUtils/Core/MPL/Impl/PopBack.hpp"
#include "SSVUtils/Core/MPL/Impl/SubList.hpp"
#include "SSVUtils/Core/MPL/Impl/Contains.hpp"
#include "SSVUtils/Core/MPL/Impl/Append.hpp"
#include "SSVUtils/Core/MPL/Impl/Prepend.hpp"

namespace ssvu
{
	namespace MPL
	{
		template<typename... Ts> struct List
		{
			using Type = List<Ts...>;

			using Head = Impl::VAHead<Ts...>;
			using Tail = Impl::VATail<Ts...>;

			using AsTuple = Tpl<Ts...>;

			inline static constexpr SizeT getSize() noexcept { return sizeof...(Ts); }

			template<int TS> using At = TplElem<TS >= 0 ? TS : getSize() + TS, AsTuple>;

			template<typename T> using PushBack = List<Ts..., T>;
			template<typename T> using PushFront = List<T, Ts...>;

			template<typename TL> using Append = typename Impl::AppendHlpr<Type, TL>::Type;
			template<typename TL> using Prepend = typename Impl::PrependHlpr<Type, TL>::Type;

			using PopBack = typename Impl::PopBackHlpr<List<Ts...>>::Type;
			using PopFront = typename Impl::PopFrontHlpr<List<Ts...>>::Type;
			using Clear = List<>;

			template<SizeT TS1, SizeT TS2> using SubList = typename Impl::SubListHlpr<TS1, TS2, TS1, List<Ts...>>::Type;

			template<typename T> inline static constexpr bool has() noexcept { return Impl::ContainsHlpr<T, Ts...>(); }

			// TODO: insert
			// TODO: unique
		};

		// TODO: move to utils

		template<typename... Ts> inline constexpr auto getMaxSize() noexcept		{ return MPL::getMax<SizeT>(sizeof(Ts)...); }
		template<typename... Ts> inline constexpr auto getMaxAlign() noexcept		{ return MPL::getMax<SizeT>(alignof(Ts)...); }
		template<typename T, typename... Ts> inline constexpr auto has() noexcept	{ return MPL::List<Ts...>::template has<T>(); }

		template<template<typename> class TApply, typename...> struct Applier;
		template<template<typename> class TApply, typename T> struct Applier<TApply, T> : public Conditional<TApply<T>{}(), TrueT, FalseT> { };
		template<template<typename> class TApply, typename T1, typename T2, typename... Ts> struct Applier<TApply, T1, T2, Ts...> : public IntegralConstant<bool, TApply<T1>{}() && Applier<TApply, T2, Ts...>()> { };

		namespace Impl
		{
			template<typename... Ts> using ApplierIsPOD = Applier<std::is_pod, Ts...>;
		}

		template<typename... Ts> inline constexpr auto arePOD() noexcept { return Impl::ApplierIsPOD<Ts...>(); }
	}
}

#endif
