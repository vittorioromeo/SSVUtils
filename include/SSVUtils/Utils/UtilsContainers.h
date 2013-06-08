// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UTILS_CONTAINERS
#define SSVU_UTILS_CONTAINERS

#include <vector>
#include <map>
#include <algorithm>

namespace ssvu
{
	/*!
	 *
	 * @brief Removes a specific item from a container.
	 *
	 * @tparam T Type of the container.
	 * @tparam V Type of the value.
	 * @param mContainer Reference to the container.
	 * @param mValue Const reference to the value.
	 *
	 */
	template<typename T, typename V> void eraseRemove(T& mContainer, const V& mValue)
	{
		mContainer.erase(std::remove(std::begin(mContainer), std::end(mContainer), mValue), std::end(mContainer));
	}

	/*!
	 *
	 * @brief Checks if a specific item is in a container.
	 *
	 * @tparam T Type of the container.
	 * @tparam V Type of the value.
	 * @param mContainer Reference to the container.
	 * @param mValue Const reference to the value.
	 *
	 * @return Returns true if the container contains the specific item.
	 *
	 */
	template<typename T, typename V> bool contains(const T& mContainer, const V& mValue)
	{
		return std::find(std::begin(mContainer), std::end(mContainer), mValue) != std::end(mContainer);
	}

	/*!
	 *
	 * @brief Checks if any item from a container is in another container.
	 *
	 * This method calls "contains" on every item.
	 *
	 * @tparam T Type of the container to check.
	 * @tparam T2 Type of the container containing items to check.
	 * @param mContainer Reference to the container to check.
	 * @param mValues Const reference to the container containing the items to check.
	 *
	 * @return Returns true if the container contains any of the items.
	 *
	 */
	template<typename T, typename T2> bool containsAny(T& mContainer, const T2& mValues)
	{
		for(const auto& v : mValues) if(contains(mContainer, v)) return true;
		return false;
	}

	/*!
	 *
	 * @brief Sorts a container. (no predicate)
	 *
	 * @tparam T Type of the container.
	 * @param mContainer Reference to the container.
	 *
	 */
	template<typename T> void sort(T& mContainer) { std::sort(std::begin(mContainer), std::end(mContainer)); }

	/*!
	 *
	 * @brief Sorts a container with a user-defined predicate.
	 *
	 * @tparam T Type of the container.
	 * @tparam P Type of the predicate.
	 * @param mContainer Reference to the container.
	 * @param mPredicate Predicate used for sorting. Can be std::function, a lambda, a functor, etc...
	 *
	 */
	template<typename T, typename P> void sort(T& mContainer, P mPredicate) { std::sort(std::begin(mContainer), std::end(mContainer), mPredicate); }

	/*!
	 *
	 * @brief Gets all the keys from a std::map.
	 *
	 * @tparam K Type of the key.
	 * @tparam V Type of the value.
	 * @param mMap Const reference to the map.
	 *
	 * @return Returns a std::vector containing all the keys.
	 *
	 */
	template<typename K, typename V> std::vector<K> getKeys(const std::map<K, V>& mMap)
	{
		std::vector<K> result;
		for(const auto& itr(std::begin(mMap)); itr != std::end(mMap); ++itr) result.push_back(itr->first);
		return result;
	}
}

#endif
