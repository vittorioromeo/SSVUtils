// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_ALIASES
#define SSVU_CORE_COMMON_ALIASES

#include <type_traits>
#include <utility>

namespace ssvu
{
template <std::size_t TS, std::size_t TAlign>
using AlignedStorage = std::aligned_storage_t<TS, TAlign>;
template <typename T>
using AlignedStorageFor = AlignedStorage<sizeof(T), alignof(T)>;

template <typename TBase, typename T>
constexpr bool isSameOrBaseOf() noexcept
{
    return std::is_same_v<TBase, T> || std::is_base_of_v<TBase, T>;
}

/// @macro Perfect-forwards the arguments by using `ssvu::fwd` and `decltype`.
#define SSVU_FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#if !defined(FWD)
/// @macro Alias for `SSVU_FWD` macro.
#define FWD(...) SSVU_FWD(__VA_ARGS__)
#endif

/// @brief Moves `mV` if `T` is an rvalue reference.
template <typename T, typename TV>
constexpr decltype(auto) moveIfRValue(TV&& mV) noexcept
{
    if constexpr(std::is_rvalue_reference_v<T>)
    {
        return std::move(mV);
    }
    else
    {
        return mV;
    }
}

} // namespace ssvu

#endif
