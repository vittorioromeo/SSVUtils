// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_GLOBAL_COMMON
#define SSVU_CORE_GLOBAL_COMMON

// The following macros check the current operating system.

#if (__linux || __unix || __posix)
	/// @macro This macro is defined if the current OS is Linux.
	#define SSVU_OS_LINUX 1
#elif (_WIN64 || _WIN32)
	/// @macro This macro is defined if the current OS is Windows.
	#define SSVU_OS_WINDOWS 1
#else
	/// @macro This macro is defined if the current OS is unknown.
	#define SSVU_OS_UNKNOWN 1
#endif

// C++14: will be in standard
namespace ssvu
{
	namespace Internal
	{
		template<std::size_t TIdx, typename TSearch, typename First, typename... TArgs> struct GetImpl
		{
			using type = typename GetImpl<TIdx + 1, TSearch, TArgs...>::type;
			static constexpr int idx{TIdx};
		};

		template<std::size_t TIdx, typename TSearch, typename... TArgs> struct GetImpl<TIdx, TSearch, TSearch, TArgs...>
		{
			using type = GetImpl;
			static constexpr int idx{TIdx};
		};
	}
}

namespace std
{
	// C++14: will be in standard
	template<typename T, typename... TArgs> inline auto get(const std::tuple<TArgs...>& mTpl) noexcept
		-> decltype(std::get<ssvu::Internal::GetImpl<0, T, TArgs...>::type::idx>(mTpl))
	{
		return std::get<ssvu::Internal::GetImpl<0, T, TArgs...>::type::idx>(mTpl);
	}

	// C++14: will be in standard
	template<typename T> inline auto cbegin(const T& mContainer) noexcept -> decltype(std::begin(mContainer))	{ return std::begin(mContainer); }
	template<typename T> inline auto cend(const T& mContainer) noexcept -> decltype(std::end(mContainer))		{ return std::end(mContainer); }
	template<typename T> inline auto rbegin(const T& mContainer) noexcept -> decltype(mContainer.rbegin())		{ return mContainer.rbegin(); }
	template<typename T> inline auto rbegin(T& mContainer) noexcept -> decltype(mContainer.rbegin())			{ return mContainer.rbegin(); }
	template<typename T> inline auto crbegin(const T& mContainer) noexcept -> decltype(rbegin(mContainer))		{ return rbegin(mContainer); }
	template<typename T> inline auto rend(const T& mContainer) noexcept -> decltype(mContainer.rend())			{ return mContainer.rend(); }
	template<typename T> inline auto rend(T& mContainer) noexcept -> decltype(mContainer.rend())				{ return mContainer.rend(); }
	template<typename T> inline auto crend(const T& mContainer) noexcept -> decltype(rend(mContainer))			{ return rend(mContainer); }

	// C++14: will be in standard
	template<typename... TArgs> using common_type_t = typename std::common_type<TArgs...>::type;
	template<bool B, typename T = void> using enable_if_t = typename std::enable_if<B, T>::type;
	template<typename T> using decay_t = typename std::decay<T>::type;
	template<typename T> using remove_reference_t = typename std::remove_reference<T>::type;
	template<typename T> using remove_extent_t = typename std::remove_extent<T>::type;
	template<typename T> using remove_volatile_t = typename std::remove_volatile<T>::type;
	template<typename T> using remove_const_t = typename std::remove_const<T>::type;
	template<typename T> using remove_cv_t = typename std::remove_cv<T>::type;
	template<typename T> using add_volatile_t = typename std::add_volatile<T>::type;
	template<typename T> using add_const_t = typename std::add_const<T>::type;
	template<typename T> using add_cv_t = typename std::add_cv<T>::type;
	template<typename T> using add_lvalue_reference_t = typename std::add_lvalue_reference<T>::type;
	template<typename T> using add_rvalue_reference_t = typename std::add_rvalue_reference<T>::type;
	template<typename T> using remove_pointer_t = typename std::remove_pointer<T>::type;
	template<typename T> using add_pointer_t = typename std::add_pointer<T>::type;
	template<typename T> using make_unsigned_t = typename std::make_unsigned<T>::type;
	template<typename T> using make_signed_t = typename std::make_signed<T>::type;
	template<typename T> using remove_all_extents_t = typename std::remove_all_extents<T>::type;
	template<std::size_t Len, std::size_t Align> using aligned_storage_t = typename std::aligned_storage<Len, Align>::type;
	template<bool B, typename T, typename F> using conditional_t = typename std::conditional<B, T, F>::type;
	template<typename T> using underlying_type_t = typename std::underlying_type<T>::type;
	template<std::size_t TS, typename T> using tuple_element_t = typename std::tuple_element<TS, T>::type;
}

namespace ssvu
{
	// Type traits shortcuts
	template<typename T, typename TDeleter = std::default_delete<T>> using Uptr = std::unique_ptr<T, TDeleter>;
	template<typename T> using Sptr = std::shared_ptr<T>;
	template<typename... TArgs> using Common = std::common_type_t<TArgs...>;
	template<bool TBool, typename T = void> using EnableIf = std::enable_if_t<TBool, T>;
	template<typename T> using Decay = std::decay_t<T>;
	template<typename T> using RemoveReference = std::remove_reference_t<T>;
	template<typename T> using RemoveExtent = std::remove_extent_t<T>;
	template<typename T> using RemoveVolatile = std::remove_volatile_t<T>;
	template<typename T> using RemoveConst = std::remove_const_t<T>;
	template<typename T> using RemoveCv = std::remove_cv_t<T>;
	template<typename T> using AddVolatile = std::add_volatile_t<T>;
	template<typename T> using AddConst = std::add_const_t<T>;
	template<typename T> using AddCv = std::add_cv_t<T>;
	template<typename T> using AddLValueRef = std::add_lvalue_reference_t<T>;
	template<typename T> using AddRValueRef = std::add_rvalue_reference_t<T>;
	template<typename T> using RemovePtr = std::remove_pointer_t<T>;
	template<typename T> using AddPtr = std::add_pointer_t<T>;
	template<typename T> using MakeUnsigned = std::make_unsigned_t<T>;
	template<typename T> using MakeSigned = std::make_signed_t<T>;
	template<typename T> using RemovellExtents = std::remove_all_extents_t<T>;
	template<std::size_t TS, std::size_t TAlign> using AlignedStorage = std::aligned_storage_t<TS, TAlign>;
	template<typename T> using AlignedStorageBasic = AlignedStorage<sizeof(T), alignof(T)>;
	template<bool B, typename T, typename F> using Conditional = std::conditional_t<B, T, F>;
	template<typename T> using Underlying = std::underlying_type_t<T>;
	template<typename T1, typename T2> using IsSame = typename std::is_same<T1, T2>::type;
	template<std::size_t TS, typename T> using TupleElement = std::tuple_element_t<TS, T>;

	template<typename T> inline constexpr bool isArithmetic() noexcept				{ return std::is_arithmetic<T>::value; }
	template<typename T> inline constexpr bool isSigned() noexcept					{ return std::is_signed<T>::value; }
	template<typename T1, typename T2> inline constexpr bool isSame() noexcept		{ return std::is_same<T1, T2>::value; }
	template<typename T1, typename T2> inline constexpr bool isBaseOf() noexcept	{ return std::is_base_of<T1, T2>::value; }
	template<typename T> inline constexpr bool isStandardLayout() noexcept			{ return std::is_standard_layout<T>::value; }
	template<typename T> inline constexpr bool isEnum() noexcept					{ return std::is_enum<T>::value; }
	template<typename T> inline constexpr bool isDefaultConstructible() noexcept	{ return std::is_default_constructible<T>::value; }
	template<typename T> inline constexpr bool isNothrowConstructible() noexcept	{ return std::is_nothrow_constructible<T>::value; }
	template<typename T> inline constexpr bool isNothrowDestructible() noexcept		{ return std::is_nothrow_destructible<T>::value; }
}

namespace ssvu
{
	/// @brief Returns a const reference to a statically allocated empty std::string.
	inline const std::string& getEmptyString() noexcept { static std::string result; return result; }

	/// @typedef Func is an `std::function` alias.
	template<typename T> using Func = std::function<T>;

	/// @typedef Action represents a `void()` function,
	using Action = Func<void()>;

	/// @typedef Predicate represents a `bool()` function,
	using Predicate = Func<bool()>;

	/// @typedef HRClock is a simple shortcut for `std::chrono::high_resolution_clock`.
	using HRClock = std::chrono::high_resolution_clock;

	/// @typedef FT is a shortcut for `float` intended to be used in frametime related
	/// contexts. It can be read both as "frametime" and "float time", and should be
	/// used instead of plain float for frametime-driven timers, delays and durations.
	using FT = float;

	/// @typedef FTDuration is a millisecond-precision `std::chrono::duration` intended
	/// for use in frametime-related contexts.
	using FTDuration = std::chrono::duration<FT, std::milli>;

	/// @typedef Shortcut typedef for `std::vector<ssvu::Uptr<T>>`.
	template<typename T, typename TDeleter = std::default_delete<T>> using VecUptr = std::vector<ssvu::Uptr<T, TDeleter>>;
}

// C++14: will be in standard
namespace ssvu
{
	namespace Internal
	{
		template<typename T, typename... TArgs> inline ssvu::Uptr<T> makeUptrHelper(std::false_type, TArgs&&... mArgs) { return ssvu::Uptr<T>(new T(std::forward<TArgs>(mArgs)...)); }
		template<typename T, typename... TArgs> inline ssvu::Uptr<T> makeUptrHelper(std::true_type, TArgs&&... mArgs)
		{
			SSVU_ASSERT_STATIC(std::extent<T>::value == 0, "make_unique<T[N]>() is forbidden, please use make_unique<T[]>().");
			using U = ssvu::RemoveExtent<T>;
			return ssvu::Uptr<T>(new U[sizeof...(TArgs)]{std::forward<TArgs>(mArgs)...});
		}
	}
}

namespace std
{
	// C++14: will be in standard
	template<typename T, typename... TArgs> inline ssvu::Uptr<T> make_unique(TArgs&&... mArgs) { return ssvu::Internal::makeUptrHelper<T>(std::is_array<T>(), std::forward<TArgs>(mArgs)...); }
}

namespace ssvu
{
	/// @brief Creates and returns an `ssvu::Uptr<T>`.
	/// @details Wraps `std::make_unique<T>`.
	template<typename T, typename... TArgs> inline Uptr<T> makeUptr(TArgs&&... mArgs) { return std::make_unique<T>(std::forward<TArgs>(mArgs)...); }

	/// @brief Creates and returns an `ssvu::Sptr<T>`.
	/// @details Wraps `std::make_shared<T>`.
	template<typename T, typename... TArgs> inline Sptr<T> makeSptr(TArgs&&... mArgs) { return std::make_shared<T>(std::forward<TArgs>(mArgs)...); }

	namespace Internal
	{
		// TODO: docs
		template<typename T> struct MakerUptr { template<typename... TArgs> static inline Uptr<T> make(TArgs&&... mArgs) { return makeUptr<T>(std::forward<TArgs>(mArgs)...); } };
		template<typename T> struct MakerSptr { template<typename... TArgs> static inline Sptr<T> make(TArgs&&... mArgs) { return makeSptr<T>(std::forward<TArgs>(mArgs)...); } };
	}
}

#endif
