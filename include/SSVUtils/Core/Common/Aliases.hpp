// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_ALIASES
#define SSVU_CORE_COMMON_ALIASES

namespace ssvu
{
	// STL aliases/shortcuts
	using SizeT = std::size_t;
	template<typename T>							using DefDel = typename std::default_delete<T>;
	template<typename T, typename TD = DefDel<T>>	using UPtr = std::unique_ptr<T, TD>;
	template<typename T>							using SPtr = std::shared_ptr<T>;
	template<typename... TArgs>						using Common = std::common_type_t<TArgs...>;
	template<bool TBool, typename T = void>			using EnableIf = std::enable_if_t<TBool, T>;
	template<typename T>							using Decay = std::decay_t<T>;
	template<typename T>							using RemoveRef = std::remove_reference_t<T>;
	template<typename T>							using RemoveExtent = std::remove_extent_t<T>;
	template<typename T>							using RemoveVolatile = std::remove_volatile_t<T>;
	template<typename T>							using RemoveConst = std::remove_const_t<T>;
	template<typename T>							using RemoveCV = std::remove_cv_t<T>;
	template<typename T>							using AddVolatile = std::add_volatile_t<T>;
	template<typename T>							using AddConst = std::add_const_t<T>;
	template<typename T>							using AddCv = std::add_cv_t<T>;
	template<typename T>							using AddLVRef = std::add_lvalue_reference_t<T>;
	template<typename T>							using AddRVRef = std::add_rvalue_reference_t<T>;
	template<typename T>							using RemovePtr = std::remove_pointer_t<T>;
	template<typename T>							using AddPtr = std::add_pointer_t<T>;
	template<typename T>							using MakeUnsigned = std::make_unsigned_t<T>;
	template<typename T>							using MakeSigned = std::make_signed_t<T>;
	template<typename T>							using RemoveAllExtents = std::remove_all_extents_t<T>;
	template<SizeT TS, SizeT TAlign>				using AlignedStorage = std::aligned_storage_t<TS, TAlign>;
	template<typename T>							using AlignedStorageFor = AlignedStorage<sizeof(T), alignof(T)>;
	template<bool B, typename T, typename F>		using Conditional = std::conditional_t<B, T, F>;
	template<typename T>							using Underlying = std::underlying_type_t<T>;
	template<typename T1, typename T2>				using IsSame = typename std::is_same<T1, T2>::type;
	template<SizeT TS, typename T>					using TplElem = std::tuple_element_t<TS, T>;
	template<typename T>							using RemoveAll = RemoveCV<RemoveRef<T>>;
	template<typename T, T TV>						using IntegralConstant = std::integral_constant<T, TV>;
	template<typename T, T... TVals>				using IntSeq = std::integer_sequence<T, TVals...>;
	template<SizeT... TS>							using IdxSeq = std::index_sequence<TS...>;
	template<typename... T>							using IdxSeqFor = std::make_index_sequence<sizeof...(T)>;

	template<typename T> inline constexpr auto isArithmetic() noexcept						{ return std::is_arithmetic<T>(); }
	template<typename T> inline constexpr auto isSigned() noexcept							{ return std::is_signed<T>(); }
	template<typename T1, typename T2> inline constexpr auto isSame() noexcept				{ return std::is_same<T1, T2>(); }
	template<typename TBase, typename T> inline constexpr auto isBaseOf() noexcept			{ return std::is_base_of<TBase, T>(); }
	template<typename TBase, typename T> inline constexpr auto isSameOrBaseOf() noexcept	{ return isSame<TBase, T>() || isBaseOf<TBase, T>(); }
	template<typename T> inline constexpr auto isStandardLayout() noexcept					{ return std::is_standard_layout<T>(); }
	template<typename T> inline constexpr auto isEnum() noexcept							{ return std::is_enum<T>(); }
	template<typename T> inline constexpr auto isDefaultCtor() noexcept						{ return std::is_default_constructible<T>(); }
	template<typename T> inline constexpr auto isNothrowMoveCtor() noexcept					{ return std::is_nothrow_move_constructible<T>(); }
	template<typename T> inline constexpr auto isNothrowDtor() noexcept						{ return std::is_nothrow_destructible<T>(); }
	template<typename T> inline constexpr auto isPolymorphic() noexcept						{ return std::is_polymorphic<T>(); }
	template<typename T> inline constexpr auto getTplSize() noexcept						{ return std::tuple_size<RemoveAll<T>>(); }
	template<typename T> inline constexpr auto isLVRef() noexcept							{ return std::is_lvalue_reference<T>(); }
	template<typename T> inline constexpr auto isRVRef() noexcept							{ return std::is_rvalue_reference<T>(); }
	template<typename T, typename... TArgs> inline constexpr auto isNothrowCtor() noexcept	{ return std::is_nothrow_constructible<T, TArgs...>(); }
	template<typename T> inline constexpr auto isPOD() noexcept								{ return std::is_pod<T>(); }
	template<typename T> inline constexpr auto isConst() noexcept							{ return std::is_const<T>(); }
	template<typename T> inline constexpr auto isVolatile() noexcept						{ return std::is_volatile<T>(); }

	template<typename T> inline constexpr decltype(auto) fwd(RemoveRef<T>& mA) noexcept		{ return std::forward<T>(mA); }
	template<typename T> inline constexpr decltype(auto) fwd(RemoveRef<T>&& mA) noexcept	{ return std::forward<T>(mA); }

	/// @typedef Alias for `std::function`.
	template<typename T> using Func = std::function<T>;

	/// @typedef Alias for represents a `void()` function,
	using Action = Func<void()>;

	/// @typedef Alias for a `bool()` function,
	using Predicate = Func<bool()>;

	/// @typedef Alias for `std::chrono::high_resolution_clock`.
	using HRClock = std::chrono::high_resolution_clock;

	/// @typedef Alias for `std::vector<ssvu::UPtr<T>>`.
	template<typename T, typename TD = DefDel<T>> using VecUPtr = std::vector<UPtr<T, TD>>;

	/// @brief Moves `mV` if `T` is an rvalue reference.
	template<typename T, typename TV> inline constexpr decltype(auto) moveIfRValue(TV&& mV) noexcept
	{
		return static_cast<Conditional<isRVRef<T>(), RemoveRef<TV>&&, const TV&>>(mV);
	}

	/// @brief Returns a `TBase&` casted to `T&`. Asserts that `T` is derived from `TBase`.
	template<typename T, typename TBase> inline T& castUp(TBase& mBase) noexcept { SSVU_ASSERT_STATIC_NM(isSameOrBaseOf<TBase, T>()); return static_cast<T&>(mBase); }

	/// @brief Returns a `TBase*` casted to `T*`. Asserts that `T` is derived from `TBase`.
	template<typename T, typename TBase> inline T* castUp(TBase* mBase) noexcept { SSVU_ASSERT_STATIC_NM(isSameOrBaseOf<TBase, T>()); return static_cast<T*>(mBase); }

	/// @brief Returns a `const TBase&` casted to `const T&`. Asserts that `T` is derived from `TBase`.
	template<typename T, typename TBase> inline const T& castUp(const TBase& mBase) noexcept { SSVU_ASSERT_STATIC_NM(isSameOrBaseOf<TBase, T>()); return static_cast<const T&>(mBase); }

	/// @brief Returns a `const TBase*` casted to `const T*`. Asserts that `T` is derived from `TBase`.
	template<typename T, typename TBase> inline const T* castUp(const TBase* mBase) noexcept { SSVU_ASSERT_STATIC_NM(isSameOrBaseOf<TBase, T>()); return static_cast<const T*>(mBase); }

	/// @brief Wrapper around `reinterpret_cast`, intended for use with aligned storages. Returns a `T&`.
	template<typename T, typename TStorage> inline T& castStorage(TStorage& mStorage) noexcept { return reinterpret_cast<T&>(mStorage); }

	/// @brief Wrapper around `reinterpret_cast`, intended for use with aligned storages. Returns a `T*`.
	template<typename T, typename TStorage> inline T* castStorage(TStorage* mStorage) noexcept { return reinterpret_cast<T*>(mStorage); }

	/// @brief Wrapper around `reinterpret_cast`, intended for use with aligned storages. Returns a `const T&`.
	template<typename T, typename TStorage> inline const T& castStorage(const TStorage& mStorage) noexcept { return reinterpret_cast<const T&>(mStorage); }

	/// @brief Wrapper around `reinterpret_cast`, intended for use with aligned storages. Returns a `const T*`.
	template<typename T, typename TStorage> inline const T* castStorage(const TStorage* mStorage) noexcept { return reinterpret_cast<const T*>(mStorage); }
}

#endif
