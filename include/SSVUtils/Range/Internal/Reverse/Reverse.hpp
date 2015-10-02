// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_RANGE_INTERNAL_REVERSE
#define SSVU_RANGE_INTERNAL_REVERSE

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Range/Internal/Base/Adaptor.hpp"
#include "SSVUtils/Range/Internal/Base/Range.hpp"

namespace ssvu
{
/// @brief Creates and returns a reverse range.
/// @details Uses `std::rbegin` and `std::rend` to build the range.
template <typename TC>
inline constexpr auto asRangeReverse(TC& mContainer) noexcept
{
    return makeRange(std::rbegin(mContainer), std::rend(mContainer));
}

/// @brief Creates and returns a const reverse range.
/// @details Uses `std::crbegin` and `std::crend` to build the range.
template <typename TC>
inline constexpr auto asRangeReverse(const TC& mContainer) noexcept
{
    return makeRange(std::crbegin(mContainer), std::crend(mContainer));
}
}

#endif
