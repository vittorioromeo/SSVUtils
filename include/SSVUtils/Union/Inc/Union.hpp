// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_UNION
#define SSVU_IMPL_UNION

#include "SSVUtils/Core/Detection/Detection.hpp"
#include "SSVUtils/Core/Assert/Assert.hpp"
#include "SSVUtils/Core/MPL/Impl/IdxOf.hpp"

#include "SSVUtils/Union/Inc/UnionBase.hpp"

#include <type_traits>
#include <utility>

namespace ssvu
{
/// @brief Union variant class that can store one of any `Ts` types at one
/// time.
/// @details Intended for use with types that require construction and
/// destruction.
/// In debug mode, checks are performed to make sure the data was correctly
/// constructed/destructed.
template <typename... Ts>
class Union : public Impl::UnionBase<Ts...>
{
private:
// If debug mode is enabled, store and check a "clean" storage flag for
// additional safety and debugging ease
#if defined(SSVU_DEBUG)
    // `-1` means clean, other numbers refer to the types in the list
    int dirtyFlag{-1};
    inline void setClean() noexcept
    {
        dirtyFlag = -1;
    }
    inline void assertClean() const noexcept
    {
        SSVU_ASSERT(dirtyFlag == -1, "Union needs to be clean");
    }
    template <typename T>
    inline void setDirty() noexcept
    {
        dirtyFlag = MPL::getIdxOf<T, Ts...>();
    }
    template <typename T>
    inline void assertDirty() const noexcept
    {
        SSVU_ASSERT(dirtyFlag == static_cast<int>(MPL::getIdxOf<T, Ts...>()),
            "Union needs to be dirty with the correct type");
    }
#else
    inline void setClean() noexcept
    {
    }
    inline void assertClean() const noexcept
    {
    }
    inline void assertNotClean() const noexcept
    {
    }
    template <typename>
    inline void setDirty() const noexcept
    {
    }
    template <typename>
    inline void assertDirty() const noexcept
    {
    }
#endif

public:
    inline Union() = default;

    // Prevent copy/move
    inline Union(Union&&) = delete;
    inline Union(const Union&) = delete;
    inline Union& operator=(Union&&) = delete;
    inline Union& operator=(const Union&) = delete;

    /// @brief Constructs and sets the internal data to `T`.
    /// @details Asserts that any previous data was destroyed.
    template <typename T, typename... TArgs>
    inline void init(TArgs&&... mArgs) noexcept(
        std::is_nothrow_constructible_v<T, TArgs...>)
    {
        assertClean();
        setDirty<T>();
        this->template initImpl<T>(FWD(mArgs)...);
    }

    /// @brief Destroys the internal `T` data.
    /// @details Asserts that any previous data was constructed.
    template <typename T>
    inline void deinit() noexcept(std::is_nothrow_destructible_v<T>)
    {
        assertDirty<T>();
        setClean();
        this->template deinitImpl<T>();
    }

    // The Union should be cleaned before being destroyed
    inline ~Union() noexcept
    {
        assertClean();
    }

    // Getters
    template <typename T>
    inline T& get() & noexcept
    {
        assertDirty<T>();
        return this->template getImpl<T>();
    }
    template <typename T>
    inline const T& get() const& noexcept
    {
        assertDirty<T>();
        return this->template getImpl<T>();
    }
    template <typename T>
    inline T get() && noexcept
    {
        assertDirty<T>();
        return std::move(this->template getImpl<T>());
    }
};
} // namespace ssvu

#endif
