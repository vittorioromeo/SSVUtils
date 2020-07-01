// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_UNION_UNIONBASE
#define SSVU_IMPL_UNION_UNIONBASE

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
namespace Impl
{
/// @brief Base implementation for `Union` types.
template <typename... Ts>
class UnionBase
{
    SSVU_ASSERT_STATIC(
        MPL::isUnique<Ts...>(), "There must be no duplicate types");

protected:
    /// @brief Storage for the data, using max align and max size of
    /// types.
    AlignedStorage<MPL::getMaxSize<Ts...>(), MPL::getMaxAlign<Ts...>()> data;

    /// @brief Constructs and sets the internal data to `T`.
    template <typename T, typename... TArgs>
    inline void initImpl(TArgs&&... mArgs) noexcept(
        isNothrowCtor<T, TArgs...>())
    {
        SSVU_ASSERT_STATIC_NM(MPL::has<T, Ts...>());
        new(&data) T(FWD(mArgs)...);
    }

    /// @brief Destructs the internal `T` data.
    template <typename T>
    inline void deinitImpl() noexcept(isNothrowDtor<T>())
    {
        SSVU_ASSERT_STATIC_NM(MPL::has<T, Ts...>());
        getImpl<T>().~T();
    }

    template <typename T>
    inline T& getImpl() & noexcept
    {
        SSVU_ASSERT_STATIC_NM(MPL::has<T, Ts...>());
        return castStorage<T>(data);
    }
    template <typename T>
    inline const T& getImpl() const& noexcept
    {
        SSVU_ASSERT_STATIC_NM(MPL::has<T, Ts...>());
        return castStorage<T>(data);
    }
    template <typename T>
    inline T getImpl() && noexcept
    {
        SSVU_ASSERT_STATIC_NM(MPL::has<T, Ts...>());
        return std::move(castStorage<T>(data));
    }
};
} // namespace Impl
} // namespace ssvu

#endif
