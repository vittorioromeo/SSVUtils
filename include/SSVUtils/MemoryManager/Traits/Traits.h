// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVS_UTILS_TRAITS
#define SSVS_UTILS_TRAITS

#include <vector>
#include "SSVUtils/Utils/UtilsContainers.h"

namespace ssvu
{
	namespace Traits
	{
		template<typename TContainer, typename TItem> struct Container
		{
			static void init(TContainer& mContainer);
			static void clear(TContainer& mContainer);
			static void add(TContainer& mContainer, const TItem& mItem);
			static void del(TContainer& mContainer, const TItem& mItem);
		};

		template<typename TItem> struct Container<std::vector<TItem>, TItem>
		{
			typedef std::vector<TItem> TContainer;

			static void init(TContainer&) { }
			static void clear(TContainer& mContainer) { mContainer.clear(); }
			static void add(TContainer& mContainer, const TItem& mItem) { mContainer.push_back(mItem); }
			static void del(TContainer& mContainer, const TItem& mItem) { eraseRemove(mContainer, mItem); }
		};
	}
}

#endif
