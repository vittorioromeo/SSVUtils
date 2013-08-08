// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER
#define SSVU_MEMORYMANAGER

#include <vector>
#include <algorithm>
#include "SSVUtils/Global/Typedefs.h"

namespace ssvu
{
	template<typename TDerived, typename T, typename TDeleter = std::default_delete<T>> class MemoryManagerBase;

	class MemoryManageable
	{
		template<typename TDerived, typename T, typename TDeleter> friend class MemoryManagerBase;

		public:
			bool alive{true};

		public:
			inline bool isAlive() const	{ return alive; }
	};

	template<typename TDerived, typename T, typename TDeleter> class MemoryManagerBase
	{
		public:
			using TUptr = Uptr<T, TDeleter>;
			using Container = std::vector<TUptr>;

		protected:
			using Iterator = typename Container::iterator;
			using ConstIterator = typename Container::const_iterator;

			Container items;
			std::vector<T*> toAdd;
			struct Deleter { inline bool operator()(const TUptr& mItem) const { return !mItem->alive; } } deleter;

		 public:
			inline void refresh() { static_cast<TDerived*>(this)->refreshImpl(); }
			inline void clear()	{ items.clear(); toAdd.clear(); }
			inline void del(T& mItem) { mItem.alive = false; }

			template<typename TType, typename... TArgs> inline TType& create(TArgs&&... mArgs)
			{
				return static_cast<TDerived*>(this)->template createTImpl<TType, TArgs...>(std::forward<TArgs>(mArgs)...);
			}
			template<typename... TArgs> inline T& create(TArgs&&... mArgs)
			{
				return static_cast<TDerived*>(this)->template createImpl<TArgs...>(std::forward<TArgs>(mArgs)...);
			}

			inline Container& getItems()				{ return items; }
			inline const Container& getItems() const	{ return items; }

			// Foreach loop/algorithms iterator support
			Iterator begin()				{ return items.begin(); }
			ConstIterator begin() const		{ return items.begin(); }
			const Iterator cbegin() const	{ return items.cbegin(); }
			Iterator end()					{ return items.end(); }
			ConstIterator end() const		{ return items.end(); }
			const Iterator cend() const		{ return items.cend(); }
	 };

	template<typename T, typename TDeleter = std::default_delete<T>> class MemoryManager : public MemoryManagerBase<MemoryManager<T, TDeleter>, T, TDeleter>
	{
		public:
			inline void refreshImpl()
			{
				this->items.erase(std::remove_if(std::begin(this->items), std::end(this->items), this->deleter), std::end(this->items));
				for(const auto& i : this->toAdd) this->items.emplace_back(i); this->toAdd.clear();
			}
			template<typename TType, typename... TArgs> inline TType& createTImpl(TArgs&&... mArgs)
			{
				auto result(new TType(std::forward<TArgs>(mArgs)...)); this->toAdd.push_back(result); return *result;
			}
			template<typename... TArgs> inline T& createImpl(TArgs&&... mArgs)
			{
				return createTImpl<T, TArgs...>(std::forward<TArgs>(mArgs)...);
			}
	};
}

#endif
