// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER
#define SSVU_MEMORYMANAGER

#include <vector>
#include <algorithm>
#include "SSVUtils/Global/Typedefs.h"
#include "SSVUtils/Utils/UtilsContainers.h"

namespace ssvu
{
	struct MemoryManageable { bool mmAlive{true}; };

	namespace Internal
	{
		template<typename TDerived, typename T, typename TDeleter = std::default_delete<T>> class MemoryManagerBase
		{
			protected:
				using TUptr = Uptr<T, TDeleter>;

			public:
				using Container = std::vector<TUptr>;

			protected:
				using Iterator = typename Container::iterator;
				using ConstIterator = typename Container::const_iterator;

				Container items;
				std::vector<T*> toAdd;

			 public:
				inline void clear()			{ items.clear(); toAdd.clear(); }
				inline void del(T& mItem)	{ mItem.mmAlive = false; }

				// Statically polymorphic methods
				inline void refresh() { static_cast<TDerived*>(this)->refreshImpl(); }
				template<typename TType, typename... TArgs> inline TType& create(TArgs&&... mArgs)
				{
					return static_cast<TDerived*>(this)->template createTImpl<TType, TArgs...>(std::forward<TArgs>(mArgs)...);
				}
				template<typename... TArgs> inline T& create(TArgs&&... mArgs)
				{
					return static_cast<TDerived*>(this)->template createImpl<TArgs...>(std::forward<TArgs>(mArgs)...);
				}

				inline Container& getItems()					{ return items; }
				inline const Container& getItems() const		{ return items; }

				template<typename TType> inline static bool isAlive(const TType& mItem) { return mItem->mmAlive; }
				template<typename TType> inline static bool isDead(const TType& mItem) 	{ return !mItem->mmAlive; }

				// Foreach loop/algorithms iterator support
				Iterator begin()				{ return items.begin(); }
				ConstIterator begin() const		{ return items.begin(); }
				const Iterator cbegin() const	{ return items.cbegin(); }
				Iterator end()					{ return items.end(); }
				ConstIterator end() const		{ return items.end(); }
				const Iterator cend() const		{ return items.cend(); }
		 };
	}

	template<typename T, typename TDeleter = std::default_delete<T>> class MemoryManager : public Internal::MemoryManagerBase<MemoryManager<T, TDeleter>, T, TDeleter>
	{
		public:
			inline void refreshImpl()
			{
				eraseRemoveIf(this->items, this->template isDead<Uptr<T, TDeleter>>);
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
