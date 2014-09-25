// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_RANGE_INTERNAL_CASTREF
#define SSVU_RANGE_INTERNAL_CASTREF

namespace ssvu
{
	namespace Internal
	{
		template<typename T> struct AdaptorImplCastRef
		{
			template<typename TItr> inline static T& get(TItr mItr) noexcept { return reinterpret_cast<T&>(**mItr);  }
		};

		template<typename T, typename TItr> using ItrCastRef = Internal::BaseAdaptorItrRnd<TItr, Internal::AdaptorImplCastRef<T>>;
	}

	/// @brief Creates and returns a cast ref iterator adaptor.
	/// @details The cast ref adaptor automatically dereferences pointer-like objects and casts them to `T` references.
	template<typename T, typename TItr> inline auto makeItrCastRef(TItr mItr) noexcept
	{
		return Internal::ItrCastRef<T, TItr>{mItr};
	}

	/// @brief Creates and returns a cast ref iterator range.
	/// @details The cast ref range automatically dereferences pointer-like objects and casts them to `T` references.
	template<typename T, typename TC> inline auto makeRangeCastRef(TC& mContainer) noexcept
	{
		auto itrBegin(makeItrCastRef<T>(std::begin(mContainer)));
		auto itrEnd(makeItrCastRef<T>(std::end(mContainer)));

		return Range<decltype(itrBegin)>{itrBegin, itrEnd};
	}
}

#endif
