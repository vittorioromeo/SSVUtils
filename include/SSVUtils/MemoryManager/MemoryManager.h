// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER2
#define SSVU_MEMORYMANAGER2

#include <vector>
#include <algorithm>
#include "SSVUtils/Global/Typedefs.h"
#include "SSVUtils/MemoryManager/Internal/Deleter.h"

namespace ssvu
{
	template<typename T> class MemoryManager;

	class MemoryManageable
	{
		template<typename T> friend class MemoryManager;

		private:
			bool alive{true};

		public:
			inline bool isAlive() const	{ return alive; }
	};

	template<typename T> class MemoryManager
	{
		public:
			using Container = std::vector<Uptr<T>>;

		private:
			Container items;
			std::vector<T*> toAdd;
			Internal::Deleter<T> deleter;

		public:
			inline void refresh()
			{
				items.erase(std::remove_if(std::begin(items), std::end(items), deleter), std::end(items));
				for(const auto& i : toAdd) items.push_back(Uptr<T>(i)); toAdd.clear();
			}
			inline void clear()	{ items.clear(); toAdd.clear(); }
			inline void del(T& mItem) { mItem.alive = false; }

			template<typename TType, typename... TArgs> inline TType& create(TArgs&&... mArgs) { TType* result{new TType(std::forward<TArgs>(mArgs)...)}; toAdd.push_back(result); return *result; }
			template<typename... TArgs> inline T& create(TArgs&&... mArgs)	{ return create<T, TArgs...>(std::forward<TArgs>(mArgs)...); }

			inline Container& getItems() { return items; }

			// Foreach loop/algorithms iterator support
			using iterator = typename Container::iterator;
			using const_iterator = typename Container::const_iterator;

			iterator begin()				{ return items.begin(); }
			const_iterator begin() const	{ return items.begin(); }
			const iterator cbegin() const	{ return items.cbegin(); }
			iterator end()					{ return items.end(); }
			const_iterator end() const		{ return items.end(); }
			const iterator cend() const		{ return items.cend(); }
	};
}

#endif
