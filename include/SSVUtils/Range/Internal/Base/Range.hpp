// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_RANGE_INTERNAL_BASE_RANGE
#define SSVU_RANGE_INTERNAL_BASE_RANGE

namespace ssvu
{
	/// @brief Simple range class that stores a begin and end iterator.
	/// @details Can be iterated upon with C++11-style for loops.
	template<typename TItr> class Range
	{
		private:
			TItr itrBegin, itrEnd;

		public:
			inline constexpr Range(const TItr& mItrBegin, const TItr& mItrEnd) noexcept : itrBegin{mItrBegin}, itrEnd{mItrEnd} { }

			inline constexpr auto begin() const noexcept	{ return itrBegin; }
			inline constexpr auto end() const noexcept		{ return itrEnd; }
	};

	/// @brief Returns a range made from two iterators.
	template<typename TItr> inline constexpr auto makeRange(const TItr& mBegin, const TItr& mEnd) noexcept
	{
		return Range<TItr>(mBegin, mEnd);
	}

	/// @brief Returns a range made by `std::begin(mContainer)` and `std::end(mContainer)`.
	template<typename TC> inline constexpr auto asRange(TC& mContainer) noexcept
	{
		return makeRange(std::begin(mContainer), std::end(mContainer));
	}

	/// @brief Returns a range made by `std::cbegin(mContainer)` and `std::cend(mContainer)`.
	template<typename TC> inline constexpr auto asRange(const TC& mContainer) noexcept
	{
		return makeRange(std::cbegin(mContainer), std::cend(mContainer));
	}
}

#endif
