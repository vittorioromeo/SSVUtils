// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER2
#define SSVU_MEMORYMANAGER2

#include <vector>
#include "SSVUtils/Global/Typedefs.h"

namespace ssvu
{
	template<typename T, typename TDeleter> class MemoryManager2
	{
		private:
			std::vector<Uptr<T>> items;
			std::vector<T*> toAdd;
			TDeleter deleter;

		public:
			inline void cleanUp()	{ items.erase(std::remove_if(std::begin(items), std::end(items), deleter), std::end(items)); }
			inline void populate()	{ for(const auto& i : toAdd) items.push_back(Uptr<T>(i)); toAdd.clear(); }
			inline void clear()		{ items.clear(); toAdd.clear(); }

			template<typename TType, typename... TArgs> inline TType& create(TArgs&&... mArgs) { TType* result{new TType(std::forward<TArgs>(mArgs)...)}; toAdd.push_back(result); return *result; }
			template<typename... TArgs> inline T& create(TArgs&&... mArgs)	{ return create<T, TArgs...>(std::forward<TArgs>(mArgs)...); }

			inline std::vector<Uptr<T>>& getItems() { return items; }

			// Foreach loop/algorithms iterator support
			typedef typename std::vector<Uptr<T>>::iterator iterator;
			typedef typename std::vector<Uptr<T>>::const_iterator const_iterator;

			iterator begin()				{ return items.begin(); }
			const_iterator begin() const	{ return items.begin(); }
			const iterator cbegin() const	{ return items.cbegin(); }
			iterator end()					{ return items.end(); }
			const_iterator end() const		{ return items.end(); }
			const iterator cend() const		{ return items.cend(); }
	};
}

#endif
