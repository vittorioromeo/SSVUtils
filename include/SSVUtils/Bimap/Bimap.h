// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_BIMAP
#define SSVU_BIMAP

#include <map>

namespace ssvu
{
	template<typename T, typename U, template<typename... TContainerArgs> class TContainer = std::map> class Bimap
	{
		private:
			TContainer<T, U> map1;
			TContainer<U, T> map2;

		public:
			Bimap() = default;
			Bimap(const std::initializer_list<std::pair<T, U>>& mPairs) { for(const auto& p : mPairs) insert(p); }

			inline void insert(const std::pair<T, U>& mPair)	{ map1[mPair.first] = mPair.second; map2[mPair.second] = mPair.first; }
			inline const U& at(const T& mValue) const			{ return map1.at(mValue); }
			inline const T& at(const U& mValue) const			{ return map2.at(mValue); }
			inline bool has(const T& mValue) const				{ return map1.find(mValue) != end(map1); }
			inline bool has(const U& mValue) const				{ return map2.find(mValue) != end(map2); }
			inline void erase(const T& mValue)					{ map2.erase(map1.at(mValue)); map1.erase(mValue); }
			inline void erase(const U& mValue)					{ map1.erase(map2.at(mValue)); map2.erase(mValue); }
	};

}

#endif
