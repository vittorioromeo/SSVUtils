// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER
#define SSVU_MEMORYMANAGER

#include "MemoryManager/Traits/Traits.h"

namespace ssvu
{
	template<typename TItem, typename TContainer = std::vector<TItem*>, typename TDelContainer = std::vector<TItem*>> class MemoryManager
	{
		private:
			typedef Traits::Container<TContainer, TItem*> ContainerTraits;
			typedef Traits::Container<TDelContainer, TItem*> DelContainerTraits;

			TContainer items;
			TDelContainer itemsToDel;

		public:
			MemoryManager() { ContainerTraits::init(items); DelContainerTraits::init(itemsToDel); }
			~MemoryManager() { for(auto& i : items) delete i; }

			template<typename TItemType, typename... TArgs> TItemType& create(TArgs&&... mArgs)
			{
				TItemType* result{new TItemType(std::forward<TArgs>(mArgs)...)};
				ContainerTraits::add(items, static_cast<TItem*>(result)); return *result;
			}
			template<typename... TArgs> TItem& create(TArgs&&... mArgs)
			{
				TItem* result{new TItem(std::forward<TArgs>(mArgs)...)};
				ContainerTraits::add(items, result); return *result;
			}
			void del(TItem* mItem) { DelContainerTraits::add(itemsToDel, mItem); }
			void cleanUp()
			{
				for(auto& i : itemsToDel) { ContainerTraits::del(items, i); delete i; }
				DelContainerTraits::clear(itemsToDel);
			}
			void clear()
			{
				for(auto& i : items) delete i;
				ContainerTraits::clear(items);
				DelContainerTraits::clear(itemsToDel);
			}
			TContainer& getItems() { return items; }
	};
}

#endif
