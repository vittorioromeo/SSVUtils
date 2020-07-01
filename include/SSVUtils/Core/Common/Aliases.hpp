// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_ALIASES
#define SSVU_CORE_COMMON_ALIASES

#include <type_traits>
#include <utility>

#include <memory>
#include <limits>
#include <string>
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
template <bool TB>
using CTBool = std::integral_constant<bool, TB>;

template <typename TBase, typename T>
inline constexpr bool isSameOrBaseOf() noexcept
{
    return std::is_same_v<TBase, T> || std::is_base_of_v<TBase, T>;
}
template <typename T>
inline constexpr auto isEnum() noexcept
{
    return std::is_enum<T>();
}
template <typename T>
inline constexpr auto getTplSize() noexcept
{
    return std::tuple_size_v<std::remove_cv_t<std::remove_reference_t<T>>>;
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
    return static_cast<std::conditional_t<std::is_rvalue_reference_v<T>,
        std::remove_reference_t<TV>&&, const TV&>>(mV);
}

} // namespace ssvu

#endif
