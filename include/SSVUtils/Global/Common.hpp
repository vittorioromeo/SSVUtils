// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_GLOBAL_COMMON
#define SSVU_GLOBAL_COMMON

#if (__linux || __unix || __posix)
	#define SSVU_OS_LINUX
#elif (_WIN64 || _WIN32)
	#define SSVU_OS_WINDOWS
#else
	#define SSVU_OS_UNKNOWN
#endif

#include <chrono>
#include <memory>
#include <functional>
#include <type_traits>

namespace std
{
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
}

namespace ssvu
{
	// Type traits shortcuts
	template<typename T, typename TDeleter = std::default_delete<T>> using Uptr = std::unique_ptr<T, TDeleter>;
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
	template<std::size_t Len, std::size_t Align> using AlignedStorage = std::aligned_storage_t<Len, Align>;
	template<bool B, typename T, typename F> using Conditional = std::conditional_t<B, T, F>;
	template<typename T> using Underlying = std::underlying_type_t<T>;

	template<typename T1, typename T2> inline constexpr bool isBaseOf() noexcept { return std::is_base_of<T1, T2>::value; }
}

namespace ssvu
{

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
}

#endif
