// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_LIST
#define SSVU_CORE_MPL_IMPL_LIST

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"
#include "SSVUtils/Core/MPL/Impl/Negate.hpp"
#include "SSVUtils/Core/MPL/Impl/MinMax.hpp"
#include "SSVUtils/Core/MPL/Impl/VAHead.hpp"
#include "SSVUtils/Core/MPL/Impl/VATail.hpp"
#include "SSVUtils/Core/MPL/Impl/VACount.hpp"
#include "SSVUtils/Core/MPL/Impl/PopFront.hpp"
#include "SSVUtils/Core/MPL/Impl/RemoveLast.hpp"
#include "SSVUtils/Core/MPL/Impl/PopBack.hpp"
#include "SSVUtils/Core/MPL/Impl/SubList.hpp"
#include "SSVUtils/Core/MPL/Impl/Contains.hpp"
#include "SSVUtils/Core/MPL/Impl/Append.hpp"
#include "SSVUtils/Core/MPL/Impl/Unique.hpp"
#include "SSVUtils/Core/MPL/Impl/IdxOf.hpp"

namespace ssvu
{
	namespace MPL
	{
		namespace Impl
		{
			inline constexpr int getCycleIdx(int mX, int mSize) noexcept { return mX >= 0 ? mX : mSize + mX; }
		}

		template<typename... Ts> struct List
		{
			/// @brief Type of the list itself.
			using Type = List<Ts...>;

			/// @brief Returns this list as a tuple.
			using AsTpl = Tpl<Ts...>;

			/// @brief Returns an empty list.
			using Clear = List<>;

			/// @brief First type in the list. `Null` if empty.
			using Head = Impl::VAHead<Ts...>;

			/// @brief Last type in the list. `Null` if empty.
			using Tail = Impl::VATail<Ts...>;

			/// @brief Returns a copy of this list, without duplicate types.
			using Unique = Impl::Unique<Ts...>;

			/// @brief Returns a new list, where every type is the result of applying `TApply`.
			template<template<typename> class TApply> using Apply = List<TApply<Ts>...>;

			/// @brief Returns a copy of this list, discarding the types that do not satisfy `TFilter`.
			template<template<typename> class TFilter> using Filter = Impl::Filter<TFilter, Ts...>;

			/// @brief Returns the type at index `TS`. Negative values are allowed: `-1` is the last type in the list.
			template<int TS> using At = TplElem<Impl::getCycleIdx(TS, sizeof...(Ts)), AsTpl>;

			/// @brief Returns a copy of this list, with `T` added at the back.
			template<typename T> using PushBack = List<Ts..., T>;

			/// @brief Returns a copy of this list, with `T` added at the front.
			template<typename T> using PushFront = List<T, Ts...>;

			/// @brief Returns a copy of this list, without the type at the back.
			using PopBack = typename Impl::PopBackHlpr<List<Ts...>>::Type;

			/// @brief Returns a copy of this list, without the type at the front.
			using PopFront = typename Impl::PopFrontHlpr<List<Ts...>>::Type;

			/// @brief Returns a copy of this list, with all types from the list `TL` added at the back.
			template<typename TL> using Append = typename Impl::AppendHlpr<Type, TL>::Type;

			/// @brief Returns a copy of this list, with all types from the list `TL` added at the front.
			template<typename TL> using Prepend = typename Impl::AppendHlpr<TL, Type>::Type;

			/// @brief Returns the numbers of types in the list.
			inline static constexpr SizeT getSize() noexcept { return sizeof...(Ts); }

			/// @brief Returns true if the list is empty.
			inline static constexpr bool isEmpty() noexcept { return getSize() == 0; }

			/// @brief Returns the number of occurrences of `T` in the list.
			template<typename T> inline static constexpr SizeT getCountOf() noexcept { return Impl::getCountOf<T, Ts...>(); }

			/// @brief Returns true if the list has no duplicate types.
			inline static constexpr bool isUnique() noexcept { return isSame<Type, Unique>(); }

			/// @brief Returns true if at least one occurrence of `T` is present in the list.
			template<typename T> inline static constexpr bool has() noexcept { return Impl::ContainsHlpr<T, Ts...>(); }

			/// @brief Returns the size (using `sizeof`) of the biggest type in the list.
			inline static constexpr SizeT getMaxSize() noexcept { return getMax<SizeT>(sizeof(Ts)...); }

			/// @brief Returns the biggest alignment (using `alignof`) of the types in the list.
			inline static constexpr SizeT getMaxAlign() noexcept { return getMax<SizeT>(alignof(Ts)...); }

			/// @brief Returns true if all types in the list match `TFilter`.
			template<template<typename> class TFilter> inline static constexpr bool all() { return Filter<TFilter>::getSize() == getSize(); }

			/// @brief Returns true if at least one of the types matches `TFilter`.
			template<template<typename> class TFilter> inline static constexpr bool any() { return Filter<TFilter>::getSize() > 0; }

			/// @brief Returns the index of the first occurence of `T` in the type list. Asserts that `T` is in the list.
			template<typename T> inline static constexpr SizeT getIdxOf() { SSVU_ASSERT_STATIC_NM(has<T>()); return Impl::IdxOf<T, Ts...>{}(); }

			// TODO
			// template<int TS1, int TS2> using Slice = typename Impl::SliceHlpr<getCycleIdx(TS1), getCycleIdx(TS2), getCycleIdx(TS1), List<Ts...>>::Type;
			template<int TS1, int TS2> using Slice = typename Impl::SliceHlpr<TS1, TS2, TS1, List<Ts...>>::Type;

			// TODO: insert
			// TODO: reverse
		};
	}
}

#endif
