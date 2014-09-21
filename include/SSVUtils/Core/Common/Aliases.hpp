// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_ALIASES
#define SSVU_CORE_COMMON_ALIASES

namespace ssvu
{
	// Type traits aliases
	template<typename T, typename TDeleter = std::default_delete<T>>	using UPtr = std::unique_ptr<T, TDeleter>;
	template<typename T>												using SPtr = std::shared_ptr<T>;
	template<typename... TArgs>											using Common = std::common_type_t<TArgs...>;
	template<bool TBool, typename T = void>								using EnableIf = std::enable_if_t<TBool, T>;
	template<typename T>												using Decay = std::decay_t<T>;
	template<typename T>												using RemoveRef = std::remove_reference_t<T>;
	template<typename T>												using RemoveExtent = std::remove_extent_t<T>;
	template<typename T>												using RemoveVolatile = std::remove_volatile_t<T>;
	template<typename T>												using RemoveConst = std::remove_const_t<T>;
	template<typename T>												using RemoveCv = std::remove_cv_t<T>;
	template<typename T>												using AddVolatile = std::add_volatile_t<T>;
	template<typename T>												using AddConst = std::add_const_t<T>;
	template<typename T>												using AddCv = std::add_cv_t<T>;
	template<typename T>												using AddLValueRef = std::add_lvalue_reference_t<T>;
	template<typename T>												using AddRValueRef = std::add_rvalue_reference_t<T>;
	template<typename T>												using RemovePtr = std::remove_pointer_t<T>;
	template<typename T>												using AddPtr = std::add_pointer_t<T>;
	template<typename T>												using MakeUnsigned = std::make_unsigned_t<T>;
	template<typename T>												using MakeSigned = std::make_signed_t<T>;
	template<typename T>												using RemoveAllExtents = std::remove_all_extents_t<T>;
	template<std::size_t TS, std::size_t TAlign> 						using AlignedStorage = std::aligned_storage_t<TS, TAlign>;
	template<typename T>												using AlignedStorageBasic = AlignedStorage<sizeof(T), alignof(T)>;
	template<bool B, typename T, typename F>							using Conditional = std::conditional_t<B, T, F>;
	template<typename T>												using Underlying = std::underlying_type_t<T>;
	template<typename T1, typename T2>									using IsSame = typename std::is_same<T1, T2>::type;
	template<std::size_t TS, typename T>								using TupleElem = std::tuple_element_t<TS, T>;

	template<typename T> inline constexpr bool isArithmetic() noexcept					{ return std::is_arithmetic<T>::value; }
	template<typename T> inline constexpr bool isSigned() noexcept						{ return std::is_signed<T>::value; }
	template<typename T1, typename T2> inline constexpr bool isSame() noexcept			{ return std::is_same<T1, T2>::value; }
	template<typename T1, typename T2> inline constexpr bool isBaseOf() noexcept		{ return std::is_base_of<T1, T2>::value; }
	template<typename T1, typename T2> inline constexpr bool isSameOrBaseOf() noexcept	{ return isSame<T1, T2>() || isBaseOf<T1, T2>(); }
	template<typename T> inline constexpr bool isStandardLayout() noexcept				{ return std::is_standard_layout<T>::value; }
	template<typename T> inline constexpr bool isEnum() noexcept						{ return std::is_enum<T>::value; }
	template<typename T> inline constexpr bool isDefaultConstructible() noexcept		{ return std::is_default_constructible<T>::value; }
	template<typename T> inline constexpr bool isNothrowConstructible() noexcept		{ return std::is_nothrow_constructible<T>::value; }
	template<typename T> inline constexpr bool isNothrowDestructible() noexcept			{ return std::is_nothrow_destructible<T>::value; }
	template<typename T> inline constexpr bool isPolymorphic() noexcept					{ return std::is_polymorphic<T>::value; }
	template<typename T> inline constexpr auto getTupleSize() noexcept					{ return std::tuple_size<T>::value; }

	template<typename T> inline constexpr T&& fwd(RemoveRef<T>& mA) noexcept	{ return std::forward<T>(mA); }
	template<typename T> inline constexpr T&& fwd(RemoveRef<T>&& mA) noexcept	{ return std::forward<T>(mA); }

	/// @typedef Alias for `std::function`.
	template<typename T> using Func = std::function<T>;

	/// @typedef Alias for represents a `void()` function,
	using Action = Func<void()>;

	/// @typedef Alias for a `bool()` function,
	using Predicate = Func<bool()>;

	/// @typedef Alias for `std::chrono::high_resolution_clock`.
	using HRClock = std::chrono::high_resolution_clock;

	/// @typedef Alias for `std::vector<ssvu::UPtr<T>>`.
	template<typename T, typename TDeleter = std::default_delete<T>> using VecUPtr = std::vector<ssvu::UPtr<T, TDeleter>>;
}

#endif
