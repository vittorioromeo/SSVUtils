// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_ALIASES
#define SSVU_CORE_COMMON_ALIASES

#include <string>
#include <type_traits>
#include <memory>
#include <limits>
#include <chrono>
#include <vector>

// Import STL literals
using namespace std::literals;
using namespace std::string_literals;
using namespace std::chrono_literals;

namespace ssvu
{
template <std::size_t TS, std::size_t TAlign>
using AlignedStorage = std::aligned_storage_t<TS, TAlign>;
template <typename T>
using AlignedStorageFor = AlignedStorage<sizeof(T), alignof(T)>;
template <typename T1, typename T2>
using IsSame = typename std::is_same<T1, T2>::type;
template <typename T>
using RmAll = std::remove_cv_t<std::remove_reference_t<T>>;
template <typename T, T... TVals>
using IntSeq = std::integer_sequence<T, TVals...>;
template <typename T, T TN>
using MkIntSeq = std::make_integer_sequence<T, TN>;
template <std::size_t... TS>
using IdxSeq = std::index_sequence<TS...>;
template <std::size_t TN>
using MkIdxSeq = std::make_index_sequence<TN>;
template <typename... T>
using IdxSeqFor = MkIdxSeq<sizeof...(T)>;
template <typename... Ts>
using Tpl = std::tuple<Ts...>;
template <typename T>
using IsPOD = std::is_pod<T>;
template <typename T, T TV>
using CTVal = std::integral_constant<T, TV>;
template <int TI>
using CTInt = CTVal<int, TI>;
template <bool TB>
using CTBool = CTVal<bool, TB>;
template <std::size_t TS>
using CTSizeT = CTVal<std::size_t, TS>;
template <typename T>
using NumLimits = std::numeric_limits<T>;

template <typename T>
inline constexpr auto isArithmetic() noexcept
{
    return std::is_arithmetic<T>();
}
template <typename T>
inline constexpr auto isSigned() noexcept
{
    return std::is_signed<T>();
}
template <typename T1, typename T2>
inline constexpr auto isSame() noexcept
{
    return std::is_same<T1, T2>();
}
template <typename TBase, typename T>
inline constexpr auto isBaseOf() noexcept
{
    return std::is_base_of<TBase, T>();
}
template <typename TBase, typename T>
inline constexpr auto isSameOrBaseOf() noexcept
{
    return isSame<TBase, T>() || isBaseOf<TBase, T>();
}
template <typename T>
inline constexpr auto isStandardLayout() noexcept
{
    return std::is_standard_layout<T>();
}
template <typename T>
inline constexpr auto isEnum() noexcept
{
    return std::is_enum<T>();
}
template <typename T>
inline constexpr auto isDefaultCtor() noexcept
{
    return std::is_default_constructible<T>();
}
template <typename T>
inline constexpr auto isNothrowMoveCtor() noexcept
{
    return std::is_nothrow_move_constructible<T>();
}
template <typename T>
inline constexpr auto isNothrowDtor() noexcept
{
    return std::is_nothrow_destructible<T>();
}
template <typename T>
inline constexpr auto isPolymorphic() noexcept
{
    return std::is_polymorphic<T>();
}
template <typename T>
inline constexpr auto getTplSize() noexcept
{
    return std::tuple_size<RmAll<T>>();
}
template <typename T>
inline constexpr auto isLVRef() noexcept
{
    return std::is_lvalue_reference<T>();
}
template <typename T>
inline constexpr auto isRVRef() noexcept
{
    return std::is_rvalue_reference<T>();
}
template <typename T, typename... TArgs>
inline constexpr auto isNothrowCtor() noexcept
{
    return std::is_nothrow_constructible<T, TArgs...>();
}
template <typename T>
inline constexpr auto isPOD() noexcept
{
    return std::is_pod<T>();
}
template <typename T>
inline constexpr auto isConst() noexcept
{
    return std::is_const<T>();
}
template <typename T>
inline constexpr auto isVolatile() noexcept
{
    return std::is_volatile<T>();
}
template <typename T>
inline constexpr auto isIntegral() noexcept
{
    return std::is_integral<T>();
}
template <typename T>
inline constexpr auto isFloatingPoint() noexcept
{
    return std::is_floating_point<T>();
}
template <typename T>
inline constexpr auto isUnsigned() noexcept
{
    return std::is_unsigned<T>();
}

/// @macro Perfect-forwards the arguments by using `ssvu::fwd` and `decltype`.
#define SSVU_FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#if !defined(FWD)
/// @macro Alias for `SSVU_FWD` macro.
#define FWD(...) SSVU_FWD(__VA_ARGS__)
#endif

/// @brief Moves `mV` if `T` is an rvalue reference.
template <typename T, typename TV>
inline constexpr decltype(auto) moveIfRValue(TV&& mV) noexcept
{
    return static_cast<std::conditional_t<isRVRef<T>(),
        std::remove_reference_t<TV>&&, const TV&>>(mV);
}

} // namespace ssvu

#endif
