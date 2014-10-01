// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_RANGE_INTERNAL_CASTREF
#define SSVU_RANGE_INTERNAL_CASTREF

namespace ssvu
{
	namespace Internal
	{
		/// @brief Cast iterator adaptor implementation.
		template<typename T> struct AdaptorImplCast
		{
			template<typename TItr> inline static T get(TItr mItr) noexcept { return reinterpret_cast<T>(**mItr);  }
		};

		/// @typedef Typedef for a cast iterator.
		template<typename T, typename TItr> using ItrCast = AdaptorFromItr<TItr, AdaptorImplCast<T>>;

		/// @brief Creates and returns a cast iterator.
		/// @details The cast adaptor automatically dereferences pointer-like objects and casts them to `T`.
		template<typename T, typename TItr> inline auto makeItrCast(TItr mItr) noexcept { return ItrCast<T, TItr>{mItr}; }

		/// @brief Creates and returns a const cast iterator.
		/// @details The cast adaptor automatically dereferences pointer-like objects and casts them to `const T`.
		template<typename T, typename TItr> inline auto makeItrCastConst(TItr mItr) noexcept { return ItrCast<const T, TItr>{mItr}; }
	}

	/// @brief Creates and returns a cast iterator range.
	/// @details The cast range automatically dereferences pointer-like objects and casts them to `T`.
	template<typename T, typename TC> inline auto asRangeCast(TC& mContainer) noexcept
	{
		return makeRange(Internal::makeItrCast<T>(std::begin(mContainer)), Internal::makeItrCast<T>(std::end(mContainer)));
	}

	/// @brief Creates and returns a const cast iterator range.
	/// @details The cast range automatically dereferences pointer-like objects and casts them to `const T`.
	template<typename T, typename TC> inline auto asRangeCast(const TC& mContainer) noexcept
	{
		return makeRange(Internal::makeItrCastConst<T>(std::cbegin(mContainer)), Internal::makeItrCastConst<T>(std::cend(mContainer)));
	}
}

#endif
