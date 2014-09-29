// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_RANGE_INTERNAL_REVERSE
#define SSVU_RANGE_INTERNAL_REVERSE

namespace ssvu
{
	/// @brief Creates and returns a reverse range.
	/// @details Uses `std::rbegin` and `std::rend` to build the range.
	template<typename TC> inline constexpr auto makeRangeReverse(TC& mContainer) noexcept
	{
		return Range<decltype(std::rbegin(mContainer))>{std::rbegin(mContainer), std::rend(mContainer)};
	}

	template<typename TC> inline constexpr auto makeRangeReverseConst(const TC& mContainer) noexcept
	{
		return Range<decltype(std::crbegin(mContainer))>{std::crbegin(mContainer), std::crend(mContainer)};
	}
}

#endif
