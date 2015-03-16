// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_ALIASES
#define SSVU_CORE_COMMON_ALIASES

#include <type_traits>

namespace ssvu
{
	// STL aliases/shortcuts
	using SizeT = std::size_t;
	using TrueT = std::true_type;
	using FalseT = std::false_type;

	template<typename T>							using DefDel = typename std::default_delete<T>;
	template<typename T, typename TD = DefDel<T>>	using UPtr = std::unique_ptr<T, TD>;
	template<typename T>							using SPtr = std::shared_ptr<T>;
	template<typename... TArgs>						using Common = std::common_type_t<TArgs...>;
	template<bool TBool, typename T = void>			using EnableIf = std::enable_if_t<TBool, T>;
	template<typename T>							using Decay = std::decay_t<T>;
	template<typename T>							using RmRef = std::remove_reference_t<T>;
	template<typename T>							using RmExtent = std::remove_extent_t<T>;
	template<typename T>							using RmVolatile = std::remove_volatile_t<T>;
	template<typename T>							using RmConst = std::remove_const_t<T>;
	template<typename T>							using RmCV = std::remove_cv_t<T>;
	template<typename T>							using AddVolatile = std::add_volatile_t<T>;
	template<typename T>							using AddConst = std::add_const_t<T>;
	template<typename T>							using AddCv = std::add_cv_t<T>;
	template<typename T>							using AddLVRef = std::add_lvalue_reference_t<T>;
	template<typename T>							using AddRVRef = std::add_rvalue_reference_t<T>;
	template<typename T>							using RmPtr = std::remove_pointer_t<T>;
	template<typename T>							using AddPtr = std::add_pointer_t<T>;
	template<typename T>							using MkUnsigned = std::make_unsigned_t<T>;
	template<typename T>							using MkSigned = std::make_signed_t<T>;
	template<typename T>							using RmAllExtents = std::remove_all_extents_t<T>;
	template<SizeT TS, SizeT TAlign>				using AlignedStorage = std::aligned_storage_t<TS, TAlign>;
	template<typename T>							using AlignedStorageFor = AlignedStorage<sizeof(T), alignof(T)>;
	template<bool B, typename T, typename F>		using Conditional = std::conditional_t<B, T, F>;
	template<typename T>							using Underlying = std::underlying_type_t<T>;
	template<typename T1, typename T2>				using IsSame = typename std::is_same<T1, T2>::type;
	template<SizeT TS, typename T>					using TplElem = std::tuple_element_t<TS, T>;
	template<typename T>							using RmAll = RmCV<RmRef<T>>;
	template<typename T, T... TVals>				using IntSeq = std::integer_sequence<T, TVals...>;
	template<typename T, T TN>						using MkIntSeq = std::make_integer_sequence<T, TN>;
	template<SizeT... TS>							using IdxSeq = std::index_sequence<TS...>;
	template<SizeT TN>								using MkIdxSeq = std::make_index_sequence<TN>;
	template<typename... T>							using IdxSeqFor = MkIdxSeq<sizeof...(T)>;
	template<typename... Ts>						using Tpl = std::tuple<Ts...>;
	template<typename T>							using IsPod = std::is_pod<T>;
	template<typename T, T TV>						using CTVal = std::integral_constant<T, TV>;
	template<int TI>								using CTInt = CTVal<int, TI>;
	template<bool TB>								using CTBool = CTVal<bool, TB>;

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
	template<typename T> inline constexpr auto getTplSize() noexcept						{ return std::tuple_size<RmAll<T>>(); }
	template<typename T> inline constexpr auto isLVRef() noexcept							{ return std::is_lvalue_reference<T>(); }
	template<typename T> inline constexpr auto isRVRef() noexcept							{ return std::is_rvalue_reference<T>(); }
	template<typename T, typename... TArgs> inline constexpr auto isNothrowCtor() noexcept	{ return std::is_nothrow_constructible<T, TArgs...>(); }
	template<typename T> inline constexpr auto isPOD() noexcept								{ return std::is_pod<T>(); }
	template<typename T> inline constexpr auto isConst() noexcept							{ return std::is_const<T>(); }
	template<typename T> inline constexpr auto isVolatile() noexcept						{ return std::is_volatile<T>(); }
	template<typename T> inline constexpr auto isIntegral() noexcept						{ return std::is_integral<T>(); }
	template<typename T> inline constexpr auto isFloatingPoint() noexcept					{ return std::is_floating_point<T>(); }

	/// @brief Wrapper for `std::forward`.
	template<typename T> inline constexpr decltype(auto) fwd(RmRef<T>& mA) noexcept { return std::forward<T>(mA); }

	/// @brief Wrapper for `std::forward`.
	template<typename T> inline constexpr decltype(auto) fwd(RmRef<T>&& mA) noexcept { return std::forward<T>(mA); }

	/// @brief Bring `std::move` into the `ssvu` namespace.
	using std::move;

	/// @macro Perfect-forwards the arguments by using `ssvu::fwd` and `decltype`.
	#define SSVU_FWD(...) ::ssvu::fwd<decltype(__VA_ARGS__)>(__VA_ARGS__)

	/// @macro Alias for `SSVU_FWD` macro.
	#define FWD(...) SSVU_FWD(__VA_ARGS__)

	/// @brief Wrapper for `std::tuple_cat`.
	template<typename... Ts> inline constexpr decltype(auto) tplCat(Ts&&... mS) noexcept { return std::tuple_cat(FWD(mS)...); }

	/// @brief Wrapper for `std::forward_as_tuple`.
	template<typename... Ts> inline constexpr decltype(auto) fwdAsTpl(Ts&&... mS) noexcept { return std::forward_as_tuple(FWD(mS)...); }

	/// @brief Wrapper for `std::make_tuple`.
	template<typename... Ts> inline constexpr decltype(auto) mkTpl(Ts&&... mS) noexcept { return std::make_tuple(FWD(mS)...); }

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
		return static_cast<Conditional<isRVRef<T>(), RmRef<TV>&&, const TV&>>(mV);
	}

	/// @brief Returns a `TBase&` casted to `T&`. Asserts that `T` is derived from `TBase`.
	template<typename T, typename TBase> inline constexpr T& castUp(TBase& mBase) noexcept { SSVU_ASSERT_STATIC_NM(isSameOrBaseOf<TBase, T>()); return static_cast<T&>(mBase); }

	/// @brief Returns a `TBase*` casted to `T*`. Asserts that `T` is derived from `TBase`.
	template<typename T, typename TBase> inline constexpr T* castUp(TBase* mBase) noexcept { SSVU_ASSERT_STATIC_NM(isSameOrBaseOf<TBase, T>()); return static_cast<T*>(mBase); }

	/// @brief Returns a `const TBase&` casted to `const T&`. Asserts that `T` is derived from `TBase`.
	template<typename T, typename TBase> inline constexpr const T& castUp(const TBase& mBase) noexcept { SSVU_ASSERT_STATIC_NM(isSameOrBaseOf<TBase, T>()); return static_cast<const T&>(mBase); }

	/// @brief Returns a `const TBase*` casted to `const T*`. Asserts that `T` is derived from `TBase`.
	template<typename T, typename TBase> inline constexpr const T* castUp(const TBase* mBase) noexcept { SSVU_ASSERT_STATIC_NM(isSameOrBaseOf<TBase, T>()); return static_cast<const T*>(mBase); }

	/// @brief Wrapper around `reinterpret_cast`, intended for use with aligned storages. Returns a `T&`.
	template<typename T, typename TStorage> inline constexpr T& castStorage(TStorage& mStorage) noexcept { return reinterpret_cast<T&>(mStorage); }

	/// @brief Wrapper around `reinterpret_cast`, intended for use with aligned storages. Returns a `T*`.
	template<typename T, typename TStorage> inline constexpr T* castStorage(TStorage* mStorage) noexcept { return reinterpret_cast<T*>(mStorage); }

	/// @brief Wrapper around `reinterpret_cast`, intended for use with aligned storages. Returns a `const T&`.
	template<typename T, typename TStorage> inline constexpr const T& castStorage(const TStorage& mStorage) noexcept { return reinterpret_cast<const T&>(mStorage); }

	/// @brief Wrapper around `reinterpret_cast`, intended for use with aligned storages. Returns a `const T*`.
	template<typename T, typename TStorage> inline constexpr const T* castStorage(const TStorage* mStorage) noexcept { return reinterpret_cast<const T*>(mStorage); }

	// TODO: undefined behavior?
	// /// @brief Wrapper around `reinterpret_cast`, intended for use with enum classes. Returns a non-const reference to the underlying value type.
	//template<typename T> inline auto& castEnum(T& mX) noexcept { return reinterpret_cast<Underlying<T>&>(mX); }

	/// @brief Wrapper around `static_cast`, intended for use with enums. Returns the underlying value.
	template<typename T> inline constexpr auto castEnum(const T& mX) noexcept { return static_cast<Underlying<T>>(mX); }

	/// @brief Converts a number to another number type.
	template<typename TOut, typename TIn> inline constexpr auto toNum(const TIn& mX) noexcept
		-> EnableIf<isArithmetic<TOut>() && isArithmetic<TIn>() && !isEnum<TOut>() && !isEnum<TIn>(), TOut>
	{
		return static_cast<TOut>(mX);
	}

	/// @brief Converts a number to `int`.
	template<typename T> inline constexpr auto toInt(const T& mX) noexcept { return toNum<int>(mX); }

	/// @brief Converts a number to `float`.
	template<typename T> inline constexpr auto toFloat(const T& mX) noexcept { return toNum<float>(mX); }

	/// @brief Converts a number to `SizeT`.
	template<typename T> inline constexpr auto toSizeT(const T& mX) noexcept { return toNum<SizeT>(mX); }

	// TODO: docs
	template<typename T, typename TV> inline constexpr auto toEnum(const TV& mX) noexcept -> EnableIf<isEnum<T>() && isSame<Underlying<T>, TV>(), T> { return static_cast<T>(mX); }
}

#endif
