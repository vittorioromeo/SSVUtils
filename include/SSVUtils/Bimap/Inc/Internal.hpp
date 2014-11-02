// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_BIMAP_INTERNAL
#define SSVU_IMPL_BIMAP_INTERNAL

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	/// @namespace Internal bimap implementation details.
	namespace Internal
	{
		/// @brief Comparison functor that compares pointer's values.
		template<typename T> struct PtrComparator
		{
			inline bool operator()(const T* mA, const T* mB) const noexcept { return *mA < *mB; }
		};

		/// @typedef Set of pointers, sorted by pointer values.
		template<typename T> using PtrSet = std::set<const T*, Internal::PtrComparator<T>>;

		/// @brief Helper bimap struct.
		template<typename T1, typename T2, typename T> struct BimapHelper;

		/// @brief Helper bimap struct. (specialized for the first type)
		template<typename T1, typename T2> struct BimapHelper<T1, T2, T1>
		{
			/// @typedef The other type.
			using Other = T2;

			/// @brief Returns a const reference to the set of the first type.
			template<typename TBimap> inline static const auto& getSetCurrent(TBimap& mBimap) noexcept { return mBimap.set1; }
		};

		/// @brief Helper bimap struct. (specialized for the second type)
		template<typename T1, typename T2> struct BimapHelper<T1, T2, T2>
		{
			/// @typedef The other type.
			using Other = T1;

			/// @brief Returns a const reference to the set of the second type.
			template<typename TBimap> inline static const auto& getSetCurrent(TBimap& mBimap) noexcept { return mBimap.set2; }
		};
	}
}

#endif
