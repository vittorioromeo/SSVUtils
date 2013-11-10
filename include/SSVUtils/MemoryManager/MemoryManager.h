// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER
#define SSVU_MEMORYMANAGER

#include <vector>
#include "SSVUtils/Global/Typedefs.h"
#include "SSVUtils/Utils/UtilsContainers.h"

namespace ssvu
{
	struct MemoryManageable
	{
		bool ssvu_mmAlive{true};
		inline static void destroy(MemoryManageable& mValue) noexcept { mValue.ssvu_mmAlive = false; }
	};

	namespace Internal
	{
		template<typename TDerived, typename TItem, typename TDeleter = std::default_delete<TItem>> class MemoryManagerBase
		{
			protected:
				using TUptr = Uptr<TItem, TDeleter>;

			public:
				using Container = std::vector<TUptr>;

			protected:
				using Iterator = typename Container::iterator;
				using ConstIterator = typename Container::const_iterator;
				using ReverseIterator = typename Container::reverse_iterator;
				using ConstReverseIterator = typename Container::const_reverse_iterator;

				Container items;
				std::vector<TItem*> toAdd;

			 public:
				inline void clear()	{ items.clear(); toAdd.clear(); }
				inline void del(TItem& mItem) const noexcept { mItem.ssvu_mmAlive = false; }

				// Statically polymorphic methods
				inline void refresh() { reinterpret_cast<TDerived*>(this)->refreshImpl(); }
				template<typename TType = TItem, typename... TArgs> inline TType& create(TArgs&&... mArgs)
				{
					return reinterpret_cast<TDerived*>(this)->template createTImpl<TType, TArgs...>(std::forward<TArgs>(mArgs)...);
				}

				inline Container& getItems() noexcept				{ return items; }
				inline const Container& getItems() const noexcept	{ return items; }

				template<typename TType> inline static bool isAlive(const TType& mItem) noexcept { return mItem->ssvu_mmAlive; }
				template<typename TType> inline static bool isDead(const TType& mItem) noexcept	{ return !mItem->ssvu_mmAlive; }

				// Foreach loop/algorithms iterator support
				Iterator begin() noexcept						{ return items.begin(); }
				ConstIterator begin() const noexcept			{ return items.begin(); }
				ReverseIterator rbegin() noexcept				{ return items.rbegin(); }
				ConstReverseIterator rbegin() const noexcept	{ return items.rbegin(); }
				const Iterator cbegin() const noexcept			{ return items.cbegin(); }
				Iterator end() noexcept							{ return items.end(); }
				ConstIterator end() const noexcept				{ return items.end(); }
				ReverseIterator rend() noexcept					{ return items.rend(); }
				ConstReverseIterator rend() const noexcept		{ return items.rend(); }
				const Iterator cend() const noexcept			{ return items.cend(); }
		 };
	}

	template<typename T, typename TDeleter = std::default_delete<T>> class MemoryManager : public Internal::MemoryManagerBase<MemoryManager<T, TDeleter>, T, TDeleter>
	{
		public:
			inline void refreshImpl()
			{
				for(const auto& i : this->toAdd) this->items.emplace_back(i); this->toAdd.clear();
				eraseRemoveIf(this->items, this->template isDead<Uptr<T, TDeleter>>);
			}
			template<typename TType = T, typename... TArgs> inline TType& createTImpl(TArgs&&... mArgs)
			{
				auto result(new TType(std::forward<TArgs>(mArgs)...)); this->toAdd.push_back(result); return *result;
			}
	};
}

#endif
