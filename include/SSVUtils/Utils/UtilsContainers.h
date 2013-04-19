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
	template<typename T, typename V> void eraseRemove(T& mContainer, const V& mValue)
	{
		mContainer.erase(std::remove(std::begin(mContainer), std::end(mContainer), mValue), std::end(mContainer));
	}
	template<typename T, typename V> bool contains(const T& mContainer, const V& mValue)
	{
		return std::find(std::begin(mContainer), std::end(mContainer), mValue) != std::end(mContainer);
	}
	template<typename T, typename T2> bool containsAny(T& mContainer, const T2& mValues)
	{
		for(auto& value : mValues) if(contains(mContainer, value)) return true;
		return false;
	}
	template<typename T> void sort(T& mContainer) { std::sort(std::begin(mContainer), std::end(mContainer)); }
	template<typename T, typename P> void sort(T& mContainer, P mPredicate) { std::sort(std::begin(mContainer), std::end(mContainer), mPredicate); }
	template<typename K, typename V> std::vector<K> getKeys(std::map<K, V> mMap)
	{
		std::vector<K> result;
		for(auto& itr(std::begin(mMap)); itr != std::end(mMap); ++itr) result.push_back(itr->first);
		return result;
	}
}

#endif
