// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UTILS_CONTAINERS
#define SSVU_UTILS_CONTAINERS

#include <vector>
#include <map>
#include <algorithm>
#include "SSVUtils/Utils/Math.hpp"
#include "SSVUtils/Utils/Iterator.hpp"

namespace ssvu
{
	/// @brief Wrapper around std::find that takes a container instead of two iterators.
	/// @param mContainer Reference to the container.
	/// @param mValue Const reference to the value.
	template<typename T, typename V> inline auto find(const T& mContainer, const V& mValue) -> decltype(std::find(std::begin(mContainer), std::end(mContainer), mValue))
	{
		return std::find(std::begin(mContainer), std::end(mContainer), mValue);
	}

	/// @brief Wrapper around std::rotate that takes a container instead of two iterators.
	/// @param mContainer Reference to the container.
	/// @param mNewBegin New begin iterator after the rotation.
	template<typename T, typename I> inline auto rotate(T& mContainer, const I& mNewBegin) -> decltype(std::rotate(std::begin(mContainer), mNewBegin, std::end(mContainer)))
	{
		return std::rotate(std::begin(mContainer), mNewBegin, std::end(mContainer));
	}

	/// @brief Finds a item that matches a specific predicate.
	/// @param mContainer Reference to the container.
	/// @param mPredicate Predicate used for checking. Can be std::function, a lambda, a functor, etc...
	/// @return Returns an iterator to the item that matches the predicate (can be end iterator if the item is not found).
	template<typename T, typename P> inline auto findIf(const T& mContainer, const P& mPredicate) -> decltype(std::find_if(std::begin(mContainer), std::end(mContainer), mPredicate))
	{
		return std::find_if(std::begin(mContainer), std::end(mContainer), mPredicate);
	}

	/// @brief Shifts an item towards the end of the container.
	/// @param mContainer Reference to the container.
	/// @param mValue Const reference to the value.
	/// @return Returns a past-the-end iterator for the new end of the range.
	template<typename T, typename V> inline auto remove(T& mContainer, const V& mValue) -> decltype(std::remove(std::begin(mContainer), std::end(mContainer), mValue))
	{
		return std::remove(std::begin(mContainer), std::end(mContainer), mValue);
	}

	/// @brief Shifts items not matching the predicate towards the beginning of the container.
	/// @param mContainer Reference to the container.
	/// @param mValue Const reference to the value.
	/// @return Returns a past-the-end iterator for the new end of the range.
	template<typename T, typename P> inline auto removeIf(T& mContainer, const P& mPredicate) -> decltype(std::remove_if(std::begin(mContainer), std::end(mContainer), mPredicate))
	{
		return std::remove_if(std::begin(mContainer), std::end(mContainer), mPredicate);
	}

	/// @brief Sorts a container. (no predicate)
	/// @param mContainer Reference to the container.
	template<typename T> inline void sort(T& mContainer) { std::sort(std::begin(mContainer), std::end(mContainer)); }

	/// @brief Sorts a container with a user-defined predicate.
	/// @param mContainer Reference to the container.
	/// @param mPredicate Predicate used for sorting. Can be std::function, a lambda, a functor, etc...
	template<typename T, typename P> inline void sort(T& mContainer, const P& mPredicate) { std::sort(std::begin(mContainer), std::end(mContainer), mPredicate); }

	/// @brief Sorts a container. (stable, no predicate)
	/// @param mContainer Reference to the container.
	template<typename T> inline void sortStable(T& mContainer) { std::stable_sort(std::begin(mContainer), std::end(mContainer)); }

	/// @brief Sorts a container with a user-defined predicate. (stable)
	/// @param mContainer Reference to the container.
	/// @param mPredicate Predicate used for sorting. Can be std::function, a lambda, a functor, etc...
	template<typename T, typename P> inline void sortStable(T& mContainer, const P& mPredicate) { std::stable_sort(std::begin(mContainer), std::end(mContainer), mPredicate); }

	/// @brief Gets the index of a item in the container, using find and subtracting the begin iterator.
	/// @param mContainer Reference to the container.
	/// @param mValue Const reference to the value.
	template<typename T, typename V> inline typename T::size_type idxOf(const T& mContainer, const V& mValue) { return find(mContainer, mValue) - std::begin(mContainer); }

	/// @brief Checks if a container contains any item that matches a specific predicate.
	/// @param mContainer Reference to the container.
	/// @param mPredicate Predicate used for checking. Can be std::function, a lambda, a functor, etc...
	template<typename T, typename P> inline bool containsAnyIf(T& mContainer, const P& mPredicate) { return findIf(mContainer, mPredicate) != std::end(mContainer); }

	/// @brief Removes a specific item from a container.
	/// @param mContainer Reference to the container.
	/// @param mValue Const reference to the value.
	template<typename T, typename V> inline void eraseRemove(T& mContainer, const V& mValue)
	{
		mContainer.erase(remove(mContainer, mValue), std::end(mContainer));
	}

	/// @brief Checks if a specific item is in a container.
	/// @param mContainer Reference to the container.
	/// @param mValue Const reference to the value.
	/// @return Returns true if the container contains the specific item.
	template<typename T, typename V> inline bool contains(const T& mContainer, const V& mValue) { return find(mContainer, mValue) != std::end(mContainer); }

	/// @brief Checks if any item from a container is in another container.
	/// @details This method calls "contains" on every item.
	/// @param mContainer Reference to the container to check.
	/// @param mValues Const reference to the container containing the items to check.
	/// @return Returns true if the container contains any of the items.
	template<typename T, typename T2> inline bool containsAnyOf(const T& mContainer, const T2& mValues)
	{
		for(const auto& v : mValues) if(contains(mContainer, v)) return true;
		return false;
	}

	/// @brief Checks if all items from a container are in another container.
	/// @details This method calls "contains" on every item.
	/// @param mContainer Reference to the container to check.
	/// @param mValues Const reference to the container containing the items to check.
	/// @return Returns true if the container contains all of the items.
	template<typename T, typename T2> inline bool containsAllOf(const T& mContainer, const T2& mValues)
	{
		for(const auto& v : mValues) if(!contains(mContainer, v)) return false;
		return true;
	}

	/// @brief Gets all the keys from a map container.
	/// @param mMap Const reference to the map container.
	/// @return Returns a std::vector containing all the keys.
	template<typename TMap> inline std::vector<typename TMap::key_type> getKeys(const TMap& mMap)
	{
		std::vector<typename TMap::key_type> result;
		for(const auto& p : mMap) result.push_back(p.first);
		return result;
	}

	/// @brief Removes all items that satisfy a predicate from a container.
	/// @param mContainer Reference to the container.
	/// @param mPredicate Const reference to the predicate.
	template<typename T, typename P> inline void eraseRemoveIf(T& mContainer, const P& mPredicate)
	{
		mContainer.erase(removeIf(mContainer, mPredicate), std::end(mContainer));
	}

	/// @brief Cyclically gets items from a container.
	/// @details Interally uses getWrapIdx to correctly wrap the entered index.
	/// @param mContainer Reference to the container.
	/// @param mIdx Index to use (may get wrapped).
	/// @return Non-const reference to the item at the wrapped index mIdx.
	template<typename T, typename TIdx> inline typename T::value_type& getByWrapIdx(T& mContainer, const TIdx& mIdx) { return mContainer[getWrapIdx(mIdx, mContainer.size())]; }

	/// @brief Cyclically gets items from a container. (const version)
	/// @details Interally uses getWrapIdx to correctly wrap the entered index.
	/// @param mContainer Const reference to the container.
	/// @param mIdx Index to use (may get wrapped).
	/// @return Const reference to the item at the wrapped index mIdx.
	template<typename T, typename TIdx> inline const typename T::value_type& getByWrapIdx(const T& mContainer, const TIdx& mIdx) { return mContainer.at(getWrapIdx(mIdx, mContainer.size())); }

	/// @brief Trims items matching a certain predicate from the left.
	/// @details Items are trimmed until one that doesn't match the predicate is found.
	/// @param mContainer Reference to the container.
	/// @param mPredicate Predicate to use.
	template<typename T, typename P> inline void trimL(T& mContainer, const P& mPredicate)
	{
		mContainer.erase(std::begin(mContainer), std::find_if_not(std::begin(mContainer), std::end(mContainer), mPredicate));
	}

	/// @brief Trims items matching a certain predicate from the right.
	/// @details Items are trimmed until one that doesn't match the predicate is found.
	/// @param mContainer Reference to the container.
	/// @param mPredicate Predicate to use.
	template<typename T, typename P> inline void trimR(T& mContainer, const P& mPredicate)
	{
		mContainer.erase(std::find_if_not(std::rbegin(mContainer), std::rend(mContainer), mPredicate).base(), std::end(mContainer));
	}

	/// @brief Trims items matching a certain predicate both from the left and the right.
	/// @details Items are trimmed until one that doesn't match the predicate is found.
	/// @param mContainer Reference to the container.
	/// @param mPredicate Predicate to use.
	template<typename T, typename P> inline void trimLR(T& mContainer, const P& mPredicate) { trimL(mContainer, mPredicate); trimR(mContainer, mPredicate); }

	/// @brief Gets a copy of the container with items matching a certain predicate trimmed from the left.
	/// @details Items are trimmed until one that doesn't match the predicate is found.
	/// @param mContainer Copy of the container. (original won't be modified)
	/// @param mPredicate Predicate to use.
	/// @return Returns a copy of the container, trimmed.
	template<typename T, typename P> inline T getTrimmedL(T mContainer, const P& mPredicate) { trimL(mContainer, mPredicate); return mContainer; }

	/// @brief Gets a copy of the container with items matching a certain predicate trimmed from the right.
	/// @details Items are trimmed until one that doesn't match the predicate is found.
	/// @param mContainer Copy of the container. (original won't be modified)
	/// @param mPredicate Predicate to use.
	/// @return Returns a copy of the container, trimmed.
	template<typename T, typename P> inline T getTrimmedR(T mContainer, const P& mPredicate) { trimR(mContainer, mPredicate); return mContainer; }

	/// @brief Gets a copy of the container with items matching a certain predicate trimmed both from the left and the right.
	/// @details Items are trimmed until one that doesn't match the predicate is found.
	/// @param mContainer Copy of the container. (original won't be modified)
	/// @param mPredicate Predicate to use.
	/// @return Returns a copy of the container, trimmed.
	template<typename T, typename P> inline T getTrimmedLR(T mContainer, const P& mPredicate) { trimLR(mContainer, mPredicate); return mContainer; }
}

#endif
