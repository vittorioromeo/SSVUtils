// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_UTILS_CONTAINERS
#define SSVU_CORE_UTILS_CONTAINERS

namespace ssvu
{
	/// @brief Wrapper around `std::any_of` that takes a container instead of two iterators.
	/// @param mContainer Reference to the container.
	/// @param mPredicate Predicate to use.
	template<typename TC, typename TP> inline auto anyOf(const TC& mContainer, TP mPredicate)
	{
		return std::any_of(std::begin(mContainer), std::end(mContainer), mPredicate);
	}


	/// @brief Wrapper around `std::find` that takes a container instead of two iterators.
	/// @param mContainer Reference to the container.
	/// @param mValue Const reference to the value.
	template<typename TC, typename TV> inline auto find(const TC& mContainer, const TV& mValue)
	{
		return std::find(std::begin(mContainer), std::end(mContainer), mValue);
	}

	/// @brief Wrapper around `std::rotate` that takes a container instead of two iterators.
	/// @param mContainer Reference to the container.
	/// @param mNewBegin New begin iterator after the rotation.
	template<typename TC, typename TI> inline auto rotate(TC& mContainer, const TI& mNewBegin)
	{
		return std::rotate(std::begin(mContainer), mNewBegin, std::end(mContainer));
	}

	/// @brief Finds a item that matches a specific predicate.
	/// @param mContainer Reference to the container.
	/// @param mPredicate Predicate used for checking. Can be std::function, a lambda, a functor, etc...
	/// @return Returns an iterator to the item that matches the predicate (can be end iterator if the item is not found).
	template<typename TC, typename TP> inline auto findIf(const TC& mContainer, TP mPredicate)
	{
		return std::find_if(std::begin(mContainer), std::end(mContainer), mPredicate);
	}

	/// @brief Shifts an item towards the end of the container.
	/// @param mContainer Reference to the container.
	/// @param mValue Const reference to the value.
	/// @return Returns a past-the-end iterator for the new end of the range.
	template<typename TC, typename TV> inline auto remove(TC& mContainer, const TV& mValue)
	{
		return std::remove(std::begin(mContainer), std::end(mContainer), mValue);
	}

	/// @brief Shifts items not matching the predicate towards the beginning of the container.
	/// @param mContainer Reference to the container.
	/// @param mValue Const reference to the value.
	/// @return Returns a past-the-end iterator for the new end of the range.
	template<typename TC, typename TP> inline auto removeIf(TC& mContainer, TP mPredicate)
	{
		return std::remove_if(std::begin(mContainer), std::end(mContainer), mPredicate);
	}

	/// @brief Wraps `std::lower_bound`.
	/// @param mContainer Reference to the container.
	/// @param mValue Const reference to the value.
	/// @return Returns iterator pointing to the first element not less than `mValue`.
	template<typename TC, typename TV> inline auto lowerBound(TC& mContainer, const TV& mValue)
	{
		return std::lower_bound(std::begin(mContainer), std::end(mContainer), mValue);
	}

	/// @brief Wraps `std::lower_bound`.
	/// @param mContainer Reference to the container.
	/// @param mValue Const reference to the value.
	/// @param mComparer Comparison object.
	/// @return Returns iterator pointing to the first element not less than `mValue`.
	template<typename TC, typename TV, typename TCmp> inline auto lowerBound(TC& mContainer, const TV& mValue, TCmp mComparer)
	{
		return std::lower_bound(std::begin(mContainer), std::end(mContainer), mValue, mComparer);
	}

	/// @brief Sorts a container. (no predicate)
	/// @param mContainer Reference to the container.
	template<typename TC> inline void sort(TC& mContainer) { std::sort(std::begin(mContainer), std::end(mContainer)); }

	/// @brief Sorts a container with a user-defined predicate.
	/// @param mContainer Reference to the container.
	/// @param mPredicate Predicate used for sorting. Can be std::function, a lambda, a functor, etc...
	template<typename TC, typename TP> inline void sort(TC& mContainer, TP mPredicate) { std::sort(std::begin(mContainer), std::end(mContainer), mPredicate); }

	/// @brief Sorts a container. (stable, no predicate)
	/// @param mContainer Reference to the container.
	template<typename TC> inline void sortStable(TC& mContainer) { std::stable_sort(std::begin(mContainer), std::end(mContainer)); }

	/// @brief Sorts a container with a user-defined predicate. (stable)
	/// @param mContainer Reference to the container.
	/// @param mPredicate Predicate used for sorting. Can be std::function, a lambda, a functor, etc...
	template<typename TC, typename TP> inline void sortStable(TC& mContainer, TP mPredicate) { std::stable_sort(std::begin(mContainer), std::end(mContainer), mPredicate); }

	/// @brief Gets the index of a item in the container, using find and subtracting the begin iterator.
	/// @param mContainer Reference to the container.
	/// @param mValue Const reference to the value.
	template<typename TC, typename TV> inline auto idxOf(const TC& mContainer, const TV& mValue) { return find(mContainer, mValue) - std::begin(mContainer); }

	/// @brief Checks if a container contains any item that matches a specific predicate.
	/// @param mContainer Reference to the container.
	/// @param mPredicate Predicate used for checking. Can be std::function, a lambda, a functor, etc...
	template<typename TC, typename TP> inline bool containsAnyIf(TC& mContainer, TP mPredicate) { return findIf(mContainer, mPredicate) != std::end(mContainer); }

	/// @brief Removes a specific item from a container.
	/// @param mContainer Reference to the container.
	/// @param mValue Const reference to the value.
	template<typename TC, typename TV> inline void eraseRemove(TC& mContainer, const TV& mValue)
	{
		mContainer.erase(remove(mContainer, mValue), std::end(mContainer));
	}

	/// @brief Checks if a specific item is in a container.
	/// @param mContainer Reference to the container.
	/// @param mValue Const reference to the value.
	/// @return Returns true if the container contains the specific item.
	template<typename TC, typename TV> inline bool contains(const TC& mContainer, const TV& mValue) { return find(mContainer, mValue) != std::end(mContainer); }

	/// @brief Checks if any item from a container is in another container.
	/// @details This method calls "contains" on every item.
	/// @param mContainer Reference to the container to check.
	/// @param mValues Const reference to the container containing the items to check.
	/// @return Returns true if the container contains any of the items.
	template<typename TC, typename TVC> inline bool containsAnyOf(const TC& mContainer, const TVC& mValues)
	{
		for(const auto& v : mValues) if(contains(mContainer, v)) return true;
		return false;
	}

	/// @brief Checks if all items from a container are in another container.
	/// @details This method calls "contains" on every item.
	/// @param mContainer Reference to the container to check.
	/// @param mValues Const reference to the container containing the items to check.
	/// @return Returns true if the container contains all of the items.
	template<typename TC, typename TVC> inline bool containsAllOf(const TC& mContainer, const TVC& mValues)
	{
		for(const auto& v : mValues) if(!contains(mContainer, v)) return false;
		return true;
	}

	/// @brief Gets all the keys from a map container.
	/// @param mMap Const reference to the map container.
	/// @return Returns a std::vector containing all the keys.
	template<typename TMap> inline auto getKeys(const TMap& mMap)
	{
		std::vector<typename TMap::key_type> result;
		for(const auto& p : mMap) result.emplace_back(p.first);
		return result;
	}

	/// @brief Removes all items that satisfy a predicate from a container.
	/// @param mContainer Reference to the container.
	/// @param mPredicate Const reference to the predicate.
	template<typename TC, typename TP> inline void eraseRemoveIf(TC& mContainer, TP mPredicate)
	{
		mContainer.erase(removeIf(mContainer, mPredicate), std::end(mContainer));
	}

	/// @brief Cyclically gets items from a container.
	/// @details Interally uses getMod to correctly wrap the entered index.
	/// @param mContainer Reference to the container.
	/// @param mIdx Index to use (may get wrapped).
	/// @return Non-const reference to the item at the wrapped index mIdx.
	template<typename TC, typename TIdx> inline auto& getByModIdx(TC& mContainer, const TIdx& mIdx) { return mContainer[getMod(mIdx, mContainer.size())]; }

	/// @brief Cyclically gets items from a container. (const version)
	/// @details Interally uses getMod to correctly wrap the entered index.
	/// @param mContainer Const reference to the container.
	/// @param mIdx Index to use (may get wrapped).
	/// @return Const reference to the item at the wrapped index mIdx.
	template<typename TC, typename TIdx> inline const auto& getByModIdx(const TC& mContainer, const TIdx& mIdx) { return mContainer.at(getMod(mIdx, mContainer.size())); }

	/// @brief Trims items matching a certain predicate from the left.
	/// @details Items are trimmed until one that doesn't match the predicate is found.
	/// @param mContainer Reference to the container.
	/// @param mPredicate Predicate to use.
	template<typename TC, typename TP> inline void trimL(TC& mContainer, TP mPredicate)
	{
		mContainer.erase(std::begin(mContainer), std::find_if_not(std::begin(mContainer), std::end(mContainer), mPredicate));
	}

	/// @brief Trims items matching a certain predicate from the right.
	/// @details Items are trimmed until one that doesn't match the predicate is found.
	/// @param mContainer Reference to the container.
	/// @param mPredicate Predicate to use.
	template<typename TC, typename TP> inline void trimR(TC& mContainer, TP mPredicate)
	{
		mContainer.erase(std::find_if_not(std::rbegin(mContainer), std::rend(mContainer), mPredicate).base(), std::end(mContainer));
	}

	/// @brief Trims items matching a certain predicate both from the left and the right.
	/// @details Items are trimmed until one that doesn't match the predicate is found.
	/// @param mContainer Reference to the container.
	/// @param mPredicate Predicate to use.
	template<typename TC, typename TP> inline void trimLR(TC& mContainer, TP mPredicate) { trimL(mContainer, mPredicate); trimR(mContainer, mPredicate); }

	/// @brief Gets a copy of the container with items matching a certain predicate trimmed from the left.
	/// @details Items are trimmed until one that doesn't match the predicate is found.
	/// @param mContainer Copy of the container. (original won't be modified)
	/// @param mPredicate Predicate to use.
	/// @return Returns a copy of the container, trimmed.
	template<typename TC, typename TP> inline TC getTrimL(TC mContainer, TP mPredicate) { trimL(mContainer, mPredicate); return mContainer; }

	/// @brief Gets a copy of the container with items matching a certain predicate trimmed from the right.
	/// @details Items are trimmed until one that doesn't match the predicate is found.
	/// @param mContainer Copy of the container. (original won't be modified)
	/// @param mPredicate Predicate to use.
	/// @return Returns a copy of the container, trimmed.
	template<typename TC, typename TP> inline TC getTrimR(TC mContainer, TP mPredicate) { trimR(mContainer, mPredicate); return mContainer; }

	/// @brief Gets a copy of the container with items matching a certain predicate trimmed both from the left and the right.
	/// @details Items are trimmed until one that doesn't match the predicate is found.
	/// @param mContainer Copy of the container. (original won't be modified)
	/// @param mPredicate Predicate to use.
	/// @return Returns a copy of the container, trimmed.
	template<typename TC, typename TP> inline TC getTrimLR(TC mContainer, TP mPredicate) { trimLR(mContainer, mPredicate); return mContainer; }

	namespace Internal
	{
		/// @brief Internal implementation method for UPtr emplacement in linear containers.
		template<typename T, typename TC, typename TM, typename... TArgs> inline T& getEmplaceUPtrImpl(TC& mContainer, TArgs&&... mArgs)
		{
			mContainer.emplace_back(TM::template make<TArgs...>(fwd<TArgs>(mArgs)...));
			return castUp<T>(*mContainer.back());
		}

		/// @brief Internal implementation method for UPtr emplacement in map containers.
		template<typename T, typename TC, typename TK, typename TM, typename... TArgs> inline T& getEmplaceUPtrMapImpl(TC& mContainer, TK&& mKey, TArgs&&... mArgs)
		{
			return castUp<T>(*(*(mContainer.emplace(fwd<TK>(mKey), TM::template make<TArgs...>(fwd<TArgs>(mArgs)...)).first)).second);
		}
	}

	/// @brief Emplaces a `ssvu::UPtr<T>` inside mContainer and returns a reference to the allocated T instance.
	/// @details Internally uses `ssvu::makeUPtr` and `ssvu::UPtr<T>::get`.
	/// @param mContainer Container of `ssvu::UPtr<T>` where the newly created smart pointer will be emplaced.
	/// @param mArgs Emplacement variadic arguments.
	/// @return Returns a reference to the newly allocated T instance.
	template<typename T, typename TC, typename... TArgs> inline T& getEmplaceUPtr(TC& mContainer, TArgs&&... mArgs)
	{
		return Internal::getEmplaceUPtrImpl<T, TC, Internal::MakerUPtr<T>>(mContainer, fwd<TArgs>(mArgs)...);
	}

	/// @brief Emplaces a `ssvu::UPtr<T>` inside a map-like mContainer and returns a reference to the allocated T instance.
	/// @details Internally uses `ssvu::makeUPtr` and `ssvu::UPtr<T>::get`.
	/// @param mContainer Map container of `ssvu::UPtr<T>` where the newly created smart pointer will be emplaced.
	/// @param mKey Key of the element.
	/// @param mArgs Emplacement variadic arguments.
	/// @return Returns a reference to the newly allocated T instance.
	template<typename T, typename... TArgs, typename TC, typename TK> inline T& getEmplaceUPtrMap(TC& mContainer, TK&& mKey, TArgs&&... mArgs)
	{
		return Internal::getEmplaceUPtrMapImpl<T, TC, TK, Internal::MakerUPtr<T>>(mContainer, mKey, fwd<TArgs>(mArgs)...);
	}
}

#endif
