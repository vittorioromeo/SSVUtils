// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_CASTS
#define SSVU_CORE_COMMON_CASTS

#include "SSVUtils/Core/Common/Aliases.hpp"

#include <string>

namespace ssvu
{
namespace Impl
{
template <typename T, typename TStorage>
using IsValidStorage = std::integral_constant<bool,
    sizeof(TStorage) >= sizeof(T) &&
        alignof(TStorage) >= alignof(T)>;
}

/// @brief Returns a `TBase&` casted to `T&`. Asserts that `T` is derived
/// from `TBase`.
template <typename T, typename TBase>
inline constexpr T& castUp(TBase& mBase) noexcept
{
    static_assert(isSameOrBaseOf<TBase, T>());
    return static_cast<T&>(mBase);
}

/// @brief Returns a `TBase*` casted to `T*`. Asserts that `T` is derived
/// from `TBase`.
template <typename T, typename TBase>
inline constexpr T* castUp(TBase* mBase) noexcept
{
    static_assert(isSameOrBaseOf<TBase, T>());
    return static_cast<T*>(mBase);
}

/// @brief Returns a `const TBase&` casted to `const T&`. Asserts that `T`
/// is derived from `TBase`.
template <typename T, typename TBase>
inline constexpr const T& castUp(const TBase& mBase) noexcept
{
    static_assert(isSameOrBaseOf<TBase, T>());
    return static_cast<const T&>(mBase);
}

/// @brief Returns a `const TBase*` casted to `const T*`. Asserts that `T`
/// is derived from `TBase`.
template <typename T, typename TBase>
inline constexpr const T* castUp(const TBase* mBase) noexcept
{
    static_assert(isSameOrBaseOf<TBase, T>());
    return static_cast<const T*>(mBase);
}

/// @brief Wrapper around `reinterpret_cast`, intended for use with aligned
/// storages. Returns a `T&`.
template <typename T, typename TStorage>
inline constexpr T& castStorage(TStorage& mStorage) noexcept
{
    static_assert(Impl::IsValidStorage<T, TStorage>{});
    return reinterpret_cast<T&>(mStorage);
}

/// @brief Wrapper around `reinterpret_cast`, intended for use with aligned
/// storages. Returns a `T*`.
template <typename T, typename TStorage>
inline constexpr T* castStorage(TStorage* mStorage) noexcept
{
    static_assert(Impl::IsValidStorage<T, TStorage>{});
    return reinterpret_cast<T*>(mStorage);
}

/// @brief Wrapper around `reinterpret_cast`, intended for use with aligned
/// storages. Returns a `const T&`.
template <typename T, typename TStorage>
inline constexpr const T& castStorage(const TStorage& mStorage) noexcept
{
    static_assert(Impl::IsValidStorage<T, TStorage>{});
    return reinterpret_cast<const T&>(mStorage);
}

/// @brief Wrapper around `reinterpret_cast`, intended for use with aligned
/// storages. Returns a `const T*`.
template <typename T, typename TStorage>
inline constexpr const T* castStorage(const TStorage* mStorage) noexcept
{
    static_assert(Impl::IsValidStorage<T, TStorage>{});
    return reinterpret_cast<const T*>(mStorage);
}

/// @brief Wrapper around `static_cast`, intended for use with enums.
/// Returns the underlying value.
template <typename T>
inline constexpr auto castEnum(const T& mX) noexcept
{
    return static_cast<std::underlying_type_t<T>>(mX);
}

/// @brief Converts a number to another number type.
template <typename TOut, typename TIn>
inline constexpr auto toNum(const TIn& mX) noexcept
    -> std::enable_if_t<std::is_arithmetic_v<TOut> &&
                            std::is_arithmetic_v<TIn> &&
                            !std::is_enum_v<TOut> && !std::is_enum_v<TIn>,
        TOut>
{
    return static_cast<TOut>(mX);
}

/// @brief Converts a number to `int`.
template <typename T>
inline constexpr auto toInt(const T& mX) noexcept
{
    return toNum<int>(mX);
}

/// @brief Converts a number to `float`.
template <typename T>
inline constexpr auto toFloat(const T& mX) noexcept
{
    return toNum<float>(mX);
}

/// @brief Converts a number to `double`.
template <typename T>
inline constexpr auto toDouble(const T& mX) noexcept
{
    return toNum<double>(mX);
}

/// @brief Converts a number to `std::size_t`.
template <typename T>
inline constexpr auto toSizeT(const T& mX) noexcept
{
    return toNum<std::size_t>(mX);
}

/// @brief Converts a string to `int`.
inline auto sToInt(const std::string& mX) noexcept
{
    return toInt(std::stoi(mX));
}

/// @brief Converts a string to `float`.
inline auto sToFloat(const std::string& mX) noexcept
{
    return toFloat(std::stof(mX));
}

/// @brief Converts a string to `double`.
inline auto sToDouble(const std::string& mX) noexcept
{
    return toDouble(std::stod(mX));
}

/// @brief Converts a string to `std::size_t`.
inline auto sToSizeT(const std::string& mX) noexcept
{
    return toSizeT(std::stol(mX));
}

/// @brief Converts a number to an enum of type `T`. The number type and
/// `std::underlying_type_t<T>` must match.
template <typename T, typename TV>
inline constexpr auto toEnum(const TV& mX) noexcept -> std::enable_if_t<
    std::is_enum_v<T> && std::is_same_v<std::underlying_type_t<T>, TV>, T>
{
    return static_cast<T>(mX);
}
} // namespace ssvu

#endif
