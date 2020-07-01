// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_GROWABLEARRAY
#define SSVU_GROWABLEARRAY

#include "SSVUtils/Core/Common/Casts.hpp"

#include <memory>

namespace ssvu
{
/// @brief Low-level array unique ptr wrapper.
/// @details Wraps a `std::unique_ptr<T[]>` instance and provides functions to
/// retrieve items and grow the buffer size. `T` must be movable in order to use
/// the `grow` function.
template <typename T>
class GrowableArray
{
private:
    std::unique_ptr<T[]> data;

public:
    inline GrowableArray() noexcept = default;
    inline ~GrowableArray() noexcept = default;

    inline GrowableArray(GrowableArray&&) noexcept = default;
    inline GrowableArray& operator=(GrowableArray&&) noexcept = default;

    inline GrowableArray(const GrowableArray&) = delete;
    inline GrowableArray& operator=(const GrowableArray&) = delete;

    /// @brief Grows the internal storage from `mCapacityOld` to
    /// `mCapacityNew`.
    /// @details The new capacity must be greater or equal than the old one.
    inline void grow(std::size_t mCapacityOld, std::size_t mCapacityNew)
    {
        SSVU_ASSERT(mCapacityOld <= mCapacityNew);

        auto newData(std::make_unique<T[]>(mCapacityNew));
        for(auto i(0u); i < mCapacityOld; ++i) newData[i] = std::move(data[i]);
        data = std::move(newData);
    }

    // Getters
    inline auto& getData() noexcept
    {
        return data;
    }
    inline const auto& getData() const noexcept
    {
        return data;
    }
    inline auto getDataPtr() noexcept
    {
        return data.get();
    }
    inline T& operator[](std::size_t mI) noexcept
    {
        return data[mI];
    }
    inline const T& operator[](std::size_t mI) const noexcept
    {
        return data[mI];
    }
};

/// @brief Low-level growable array storage class.
/// @details Data must be explicitly constructed and destroyed.
template <typename T>
class GrowableArrayAS
{
private:
    GrowableArray<AlignedStorageFor<T>> data;

public:
    inline GrowableArrayAS() noexcept = default;
    inline ~GrowableArrayAS() noexcept = default;

    inline GrowableArrayAS(GrowableArrayAS&& mGA) noexcept = default;
    inline GrowableArrayAS& operator=(GrowableArrayAS&& mGA) noexcept = default;

    inline GrowableArrayAS(const GrowableArrayAS& mGA) = delete;
    inline auto& operator=(const GrowableArrayAS& mGA) = delete;

    /// @brief Grows the internal storage from `mCapacityOld` to
    /// `mCapacityNew`.
    /// @details The new capacity must be greater or equal than the old one.
    inline void grow(std::size_t mCapacityOld, std::size_t mCapacityNew)
    {
        data.grow(mCapacityOld, mCapacityNew);
    }

    /// @brief Constructs a `T` instance at index `mI`.
    template <typename... TArgs>
    inline void initAt(std::size_t mI, TArgs&&... mArgs) noexcept(
        std::is_nothrow_constructible_v<T, TArgs...>)
    {
        new(&data[mI]) T(FWD(mArgs)...);
    }

    /// @brief Destroyes a `T` instance at index `mI`.
    inline void deinitAt(std::size_t mI) noexcept(
        std::is_nothrow_destructible_v<T>)
    {
        (*this)[mI].~T();
    }

    // Getters
    inline auto& getData() noexcept
    {
        return data;
    }
    inline const auto& getData() const noexcept
    {
        return data;
    }
    inline auto getDataPtr() noexcept
    {
        return data.get();
    }
    inline T& operator[](std::size_t mI) noexcept
    {
        return castStorage<T>(data[mI]);
    }
    inline const T& operator[](std::size_t mI) const noexcept
    {
        return castStorage<T>(data[mI]);
    }
};
} // namespace ssvu

#endif
