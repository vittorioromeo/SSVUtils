// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_BIMAP
#define SSVU_BIMAP

#include <cassert>
#include <map>

namespace ssvu
{
	/// @brief Bi-directional key-value container.
	/// @details Implemented as a pair of std::map
	/// @tparam T1 Key/Value type 1
	/// @tparam T2 Key/Value type 2
	/// @tparam TContainer Underlying container type. Can be std::map, std::unordered_map, ...
	template<typename T1, typename T2, template<typename...> class TContainer = std::map> class Bimap
	{
		private:
			TContainer<T1, T2> map1; /*!< Map container 1. */
			TContainer<T2, const T1*> map2; /*!< Map container 2. */

		public:
			/// @brief Default constructor.
			/// @details Initializes an empty Bimap.
			inline Bimap() = default;

			/// @brief Initializer list constructor.
			/// @details Initializes a Bimap from a std::initalizer_list of pairs.
			/// @param mPairs Initializer list of std::pair<T, U>.
			inline Bimap(const std::initializer_list<std::pair<T1, T2>>& mPairs) { for(const auto& p : mPairs) insert(p); }

			/// @brief Emplace a pair in the Bimap.
			template<typename... TArgs> inline void emplace(TArgs&&... mArgs)
			{
				const auto& pair(map1.emplace(std::forward<TArgs>(mArgs)...));
				assert(pair.second == true);

				const auto& itr(pair.first);
				map2[itr->second] = &(itr->first);
			}

			/// @brief Insert a pair in the Bimap.
			/// @param mPair std::pair<T, U> to insert.
			inline void insert(const std::pair<T1, T2>& mPair) { this->emplace(mPair); }

			/// @brief Replaces a key/value pair with another.
			inline void replace(const T1& mSrc, const T2& mDest) { assert(this->has(mSrc)); this->erase(mSrc); this->emplace(mSrc, mDest); }

			/// @brief Replaces a key/value pair with another.
			inline void replace(const T2& mSrc, const T1& mDest) { assert(this->has(mSrc)); this->erase(mSrc); this->emplace(mSrc, mDest); }

			/// @brief Erase a pair from the Bimap.
			/// @param mKey Key of the pair to erase.
			inline void erase(const T1& mKey) { assert(this->has(mKey)); map2.erase(map1.at(mKey)); map1.erase(mKey); }

			/// @brief Erase a pair from the Bimap.
			/// @param mKey Key of the pair to erase.
			inline void erase(const T2& mKey) { assert(this->has(mKey)); map1.erase(*map2.at(mKey)); map2.erase(mKey); }

			/// @brief Returns a value from the Bimap.
			/// @param mKey Key of the value to return.
			inline const T2& at(const T1& mKey) const { return map1.at(mKey); }

			/// @brief Returns a value from the Bimap.
			/// @param mKey Key of the value to return.
			inline const T1& at(const T2& mKey) const { return *map2.at(mKey); }

			/// @brief Returns a value from the Bimap. (unsafe)
			/// @param mKey Key of the value to return.
			inline const T2& operator[](const T1& mKey) noexcept { assert(this->has(mKey)); return map1[mKey]; }

			/// @brief Returns a value from the Bimap. (unsafe)
			/// @param mKey Key of the value to return.
			inline const T1& operator[](const T2& mKey) noexcept { assert(this->has(mKey)); return *map2[mKey]; }

			/// @brief Looks for a value in the Bimap.
			/// @param mValue Key of the value to find.
			inline auto find(const T1& mValue) const -> decltype(std::declval<const decltype(map1)>().find(mValue)) { return map1.find(mValue); }

			/// @brief Looks for a value in the Bimap.
			/// @param mValue Key of the value to find.
			inline auto find(const T2& mValue) const -> decltype(std::declval<const decltype(map2)>().find(mValue)) { return map2.find(mValue); }

			/// @brief Checks if a value is in the Bimap.
			/// @param mValue Key of the value to find.
			inline bool has(const T1& mValue) const { return this->find(mValue) != std::end(map1); }

			/// @brief Checks if a value is in the Bimap.
			/// @param mValue Key of the value to find.
			inline bool has(const T2& mValue) const { return this->find(mValue) != std::end(map2); }

			/// @brief Returns the first map container. (const version)
			inline const decltype(map1)& getMap1() const noexcept { return map1; }

			/// @brief Returns the second map container. (const version)
			inline const decltype(map2)& getMap2() const noexcept { return map2; }
	};
}

#endif
