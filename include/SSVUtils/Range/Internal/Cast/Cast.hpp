// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_RANGE_INTERNAL_CASTREF
#define SSVU_RANGE_INTERNAL_CASTREF

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Range/Internal/Base/Adaptor.hpp"
#include "SSVUtils/Range/Internal/Base/Range.hpp"

namespace ssvu
{
namespace Impl
{
    /// @brief Cast ptr iterator adaptor implementation.
    template <typename T>
    struct AdaptorImplCastPtr
    {
        template <typename TItr>
        inline static T get(TItr mItr) noexcept
        {
            return reinterpret_cast<T>(**mItr);
        }
    };

    /// @typedef Typedef for a cast ptr iterator.
    template <typename T, typename TItr>
    using ItrCastPtr = AdaptorFromItr<TItr, AdaptorImplCastPtr<T>>;

    /// @brief Creates and returns a cast ptr iterator.
    /// @details The cast adaptor automatically dereferences pointer-like
    /// objects and casts them to `T`.
    template <typename T, typename TItr>
    inline auto makeItrCastPtr(TItr mItr) noexcept
    {
        return ItrCastPtr<T, TItr>{mItr};
    }

    /// @brief Creates and returns a const cast ptr iterator.
    /// @details The cast adaptor automatically dereferences pointer-like
    /// objects and casts them to `const T`.
    template <typename T, typename TItr>
    inline auto makeItrCastPtrConst(TItr mItr) noexcept
    {
        return ItrCastPtr<const T, TItr>{mItr};
    }
}

/// @brief Creates and returns a cast iterator range.
/// @details The cast range automatically dereferences pointer-like objects
/// and casts them to `T`.
template <typename T, typename TC>
inline auto asRangeCastPtr(TC& mContainer) noexcept
{
    return makeRange(Impl::makeItrCastPtr<T>(std::begin(mContainer)),
    Impl::makeItrCastPtr<T>(std::end(mContainer)));
}

/// @brief Creates and returns a const cast iterator range.
/// @details The cast range automatically dereferences pointer-like objects
/// and casts them to `const T`.
template <typename T, typename TC>
inline auto asRangeCastPtr(const TC& mContainer) noexcept
{
    return makeRange(Impl::makeItrCastPtrConst<T>(std::cbegin(mContainer)),
    Impl::makeItrCastPtrConst<T>(std::cend(mContainer)));
}
}

#endif
