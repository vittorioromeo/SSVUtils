// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_COMMON_CASTS
#define SSVU_CORE_COMMON_CASTS

#include "SSVUtils/Core/Common/Aliases.hpp"
#include "SSVUtils/Core/Assert/Assert.hpp"

namespace ssvu
{
    namespace Impl
    {
        template <typename T, typename TStorage>
        using IsValidStorage = std::integral_constant<bool,
            sizeof(typename TStorage::type) >= sizeof(T) &&
                alignof(typename TStorage::type) >= alignof(T)>;
    }

    /// @brief Returns a `TBase&` casted to `T&`. Asserts that `T` is derived
    /// from `TBase`.
    template <typename T, typename TBase>
    inline constexpr T& castUp(TBase& mBase) noexcept
    {
        SSVU_ASSERT_STATIC_NM(isSameOrBaseOf<TBase, T>());
        return static_cast<T&>(mBase);
    }

    /// @brief Returns a `TBase*` casted to `T*`. Asserts that `T` is derived
    /// from `TBase`.
    template <typename T, typename TBase>
    inline constexpr T* castUp(TBase* mBase) noexcept
    {
        SSVU_ASSERT_STATIC_NM(isSameOrBaseOf<TBase, T>());
        return static_cast<T*>(mBase);
    }

    /// @brief Returns a `const TBase&` casted to `const T&`. Asserts that `T`
    /// is derived from `TBase`.
    template <typename T, typename TBase>
    inline constexpr const T& castUp(const TBase& mBase) noexcept
    {
        SSVU_ASSERT_STATIC_NM(isSameOrBaseOf<TBase, T>());
        return static_cast<const T&>(mBase);
    }

    /// @brief Returns a `const TBase*` casted to `const T*`. Asserts that `T`
    /// is derived from `TBase`.
    template <typename T, typename TBase>
    inline constexpr const T* castUp(const TBase* mBase) noexcept
    {
        SSVU_ASSERT_STATIC_NM(isSameOrBaseOf<TBase, T>());
        return static_cast<const T*>(mBase);
    }

    /// @brief Wrapper around `reinterpret_cast`, intended for use with aligned
    /// storages. Returns a `T&`.
    template <typename T, typename TStorage>
    inline constexpr T& castStorage(TStorage& mStorage) noexcept
    {
        SSVU_ASSERT_STATIC_NM(IsValidStorage<T, TStorage>{});
        return reinterpret_cast<T&>(mStorage);
    }

    /// @brief Wrapper around `reinterpret_cast`, intended for use with aligned
    /// storages. Returns a `T*`.
    template <typename T, typename TStorage>
    inline constexpr T* castStorage(TStorage* mStorage) noexcept
    {
        SSVU_ASSERT_STATIC_NM(IsValidStorage<T, TStorage>{});
        return reinterpret_cast<T*>(mStorage);
    }

    /// @brief Wrapper around `reinterpret_cast`, intended for use with aligned
    /// storages. Returns a `const T&`.
    template <typename T, typename TStorage>
    inline constexpr const T& castStorage(const TStorage& mStorage) noexcept
    {
        SSVU_ASSERT_STATIC_NM(IsValidStorage<T, TStorage>{});
        return reinterpret_cast<const T&>(mStorage);
    }

    /// @brief Wrapper around `reinterpret_cast`, intended for use with aligned
    /// storages. Returns a `const T*`.
    template <typename T, typename TStorage>
    inline constexpr const T* castStorage(const TStorage* mStorage) noexcept
    {
        SSVU_ASSERT_STATIC_NM(IsValidStorage<T, TStorage>{});
        return reinterpret_cast<const T*>(mStorage);
    }

    /// @brief Wrapper around `static_cast`, intended for use with enums.
    /// Returns the underlying value.
    template <typename T>
    inline constexpr auto castEnum(const T& mX) noexcept
    {
        return static_cast<Underlying<T>>(mX);
    }

    /// @brief Converts a number to another number type.
    template <typename TOut, typename TIn>
    inline constexpr auto toNum(const TIn& mX) noexcept
        -> EnableIf<isArithmetic<TOut>() && isArithmetic<TIn>() &&
                        !isEnum<TOut>() && !isEnum<TIn>(),
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

    /// @brief Converts a number to `SizeT`.
    template <typename T>
    inline constexpr auto toSizeT(const T& mX) noexcept
    {
        return toNum<SizeT>(mX);
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

    /// @brief Converts a string to `SizeT`.
    inline auto sToSizeT(const std::string& mX) noexcept
    {
        return toSizeT(std::stol(mX));
    }

    /// @brief Converts a number to an enum of type `T`. The number type and
    /// `Underlying<T>` must match.
    template <typename T, typename TV>
    inline constexpr auto toEnum(const TV& mX) noexcept
        -> EnableIf<isEnum<T>() && isSame<Underlying<T>, TV>(), T>
    {
        return static_cast<T>(mX);
    }
}

#endif
