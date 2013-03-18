// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UTILS_COLLECTIONS
#define SSVU_UTILS_COLLECTIONS

#include <vector>
#include <map>
#include <algorithm>

namespace ssvu
{
	namespace Utils
	{
		template<typename T, typename V> void eraseRemove(T& mItems, const V& mValue) { mItems.erase(std::remove(std::begin(mItems), std::end(mItems), mValue), std::end(mItems)); }
		template<typename T, typename V> bool contains(const T& mItems, const V& mValue) { return std::find(std::begin(mItems), std::end(mItems), mValue) != std::end(mItems); }
		template<typename T, typename T2> bool containsAny(T& mItems, const T2& mValues) { for(auto& value : mValues) if(contains(mItems, value)) return true; return false; }

		template<typename K, typename V> std::vector<K> getKeys(std::map<K, V> mMap)
		{
			std::vector<K> result;
			for(auto itr = mMap.begin(); itr != mMap.end(); ++itr) result.push_back(itr->first);
			return result;
		}
	}
}

#endif
