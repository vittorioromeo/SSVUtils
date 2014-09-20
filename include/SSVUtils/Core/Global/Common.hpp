// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_GLOBAL_COMMON
#define SSVU_CORE_GLOBAL_COMMON

// The following macros check the current operating system and architecture.

#if (__linux || __unix || __posix || __LINUX__ || __linux__)
	/// @macro This macro is defined if the current OS is Linux.
	#define SSVU_OS_LINUX 1
#elif (_WIN64 || _WIN32 || __CYGWIN32__ || __MINGW32__)
	/// @macro This macro is defined if the current OS is Windows.
	#define SSVU_OS_WINDOWS 1
#elif (MACOSX || __DARWIN__ || __APPLE__)
	/// @macro This macro is defined if the current OS is Windows.
	#define SSVU_OS_MAC 1
#else
	/// @macro This macro is defined if the current OS is unknown.
	#define SSVU_OS_UNKNOWN 1
#endif

#if (WIN_32 || __i386__ || i386 || __x86__)
	#define SSVU_ARCH_32
#elif (__amd64 || __amd64__ || __x86_64 || __x86_64__ || _M_X64 || __ia64__ || _M_IA64)
	#define SSVU_ARCH_64
#endif

namespace std
{
	// C++14: will be in standard (should be?)
	template<typename T> inline auto rbegin(const T& mContainer) noexcept	{ return mContainer.rbegin(); }
	template<typename T> inline auto rbegin(T& mContainer) noexcept 		{ return mContainer.rbegin(); }
	template<typename T> inline auto crbegin(const T& mContainer) noexcept	{ return rbegin(mContainer); }
	template<typename T> inline auto rend(const T& mContainer) noexcept 	{ return mContainer.rend(); }
	template<typename T> inline auto rend(T& mContainer) noexcept 			{ return mContainer.rend(); }
	template<typename T> inline auto crend(const T& mContainer) noexcept 	{ return rend(mContainer); }
}

namespace ssvu
{
	// Type traits shortcuts
	template<typename T, typename TDeleter = std::default_delete<T>> using UPtr = std::unique_ptr<T, TDeleter>;
	template<typename T> using SPtr = std::shared_ptr<T>;
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

	template<typename T> inline constexpr T&& fwd(RemoveReference<T>& mA) noexcept { return std::forward<T>(mA); }
	template<typename T> inline constexpr T&& fwd(RemoveReference<T>&& mA) noexcept { return std::forward<T>(mA); }
}

namespace ssvu
{
	/// @brief Returns a const reference to a statically allocated empty `std::string`.
	inline const std::string& getEmptyString() noexcept { static std::string result; return result; }

	/// @typedef Alias for `std::function`.
	template<typename T> using Func = std::function<T>;

	/// @typedef Alias for represents a `void()` function,
	using Action = Func<void()>;

	/// @typedef Alias for a `bool()` function,
	using Predicate = Func<bool()>;

	/// @typedef Alias for `std::chrono::high_resolution_clock`.
	using HRClock = std::chrono::high_resolution_clock;

	/// @typedef `FT` is an alias for `float` intended to be used in frametime related
	/// contexts. It can be read both as "frametime" and "float time", and should be
	/// used instead of plain float for frametime-driven timers, delays and durations.
	using FT = float;

	/// @typedef `FTDuration` is a millisecond-precision `std::chrono::duration` intended
	/// for use in frametime-related contexts.
	using FTDuration = std::chrono::duration<FT, std::milli>;

	/// @typedef Alias for `std::vector<ssvu::UPtr<T>>`.
	template<typename T, typename TDeleter = std::default_delete<T>> using VecUPtr = std::vector<ssvu::UPtr<T, TDeleter>>;
}

namespace ssvu
{
	// Frametime utils
	constexpr float secondsFTRatio{60.f};
	template<typename T> inline constexpr T getFTToSeconds(T mFT) noexcept		{ return mFT / secondsFTRatio; }
	template<typename T> inline constexpr T getSecondsToFT(T mSeconds) noexcept	{ return mSeconds * secondsFTRatio; }
	template<typename T> inline constexpr T getFTToFPS(T mFT) noexcept			{ return secondsFTRatio / mFT; }
}

namespace ssvu
{
	/// @brief Creates and returns an `ssvu::UPtr<T>`.
	/// @details Wraps `std::make_unique<T>`.
	template<typename T, typename... TArgs> inline UPtr<T> makeUPtr(TArgs&&... mArgs) { return std::make_unique<T>(fwd<TArgs>(mArgs)...); }

	/// @brief Creates and returns an `ssvu::SPtr<T>`.
	/// @details Wraps `std::make_shared<T>`.
	template<typename T, typename... TArgs> inline SPtr<T> makeSPtr(TArgs&&... mArgs) { return std::make_shared<T>(fwd<TArgs>(mArgs)...); }

	namespace Internal
	{
		/// @brief Internal functor that creates an `ssvu::UPtr`.
		template<typename T> struct MakerUPtr { template<typename... TArgs> static inline auto make(TArgs&&... mArgs) { return makeUPtr<T>(fwd<TArgs>(mArgs)...); } };

		/// @brief Internal functor that creates an `ssvu::SPtr`.
		template<typename T> struct MakerSPtr { template<typename... TArgs> static inline auto make(TArgs&&... mArgs) { return makeSPtr<T>(fwd<TArgs>(mArgs)...); } };
	}
}

#endif

// TODO: comply to standards _t _v and ::type ::value
// * maybe avoid using shortcuts? or use ShortcutT?
// TODO: separate
