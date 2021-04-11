// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_UNIONPOD
#define SSVU_IMPL_UNIONPOD

#include "SSVUtils/Union/Inc/UnionBase.hpp"
#include "SSVUtils/Union/Inc/Union.hpp"

#include <type_traits>

namespace ssvu
{
/// @brief Union variant class that can store one of any `Ts` POD types at
/// one time.
/// @details Intended for use only with POD types. Data cannot be
/// deinitialized.
/// No checks are performed on construction/destruction of the data.
template <typename... Ts>
struct UnionPOD : public Impl::UnionBase<Ts...>
{
    static_assert((std::is_standard_layout_v<Ts> && ...));
    static_assert((std::is_trivial_v<Ts> && ...));

    /// @brief Constructs and sets the internal data to `T`.
    template <typename T, typename... TArgs>
    inline void init(TArgs&&... mArgs) noexcept(
        std::is_nothrow_constructible_v<T, TArgs...>)
    {
        this->template initImpl<T>(FWD(mArgs)...);
    }

    // Getters
    template <typename T>
    inline T& get() & noexcept
    {
        return this->template getImpl<T>();
    }
    template <typename T>
    inline const T& get() const& noexcept
    {
        return this->template getImpl<T>();
    }
    template <typename T>
    inline T get() && noexcept
    {
        return std::move(this->template getImpl<T>());
    }
};
} // namespace ssvu

#endif
