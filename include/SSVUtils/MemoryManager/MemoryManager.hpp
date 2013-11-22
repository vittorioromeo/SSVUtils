// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER
#define SSVU_MEMORYMANAGER

#include <vector>
#include "SSVUtils/Global/Typedefs.hpp"
#include "SSVUtils/Utils/UtilsContainers.hpp"

namespace ssvu
{
	struct MemoryManageable { bool ssvu_mmAlive{true}; };

	namespace Internal
	{
		template<typename TDerived, typename TItem, typename TDeleter = std::default_delete<TItem>> class MemoryManagerBase : protected std::vector<Uptr<TItem, TDeleter>>
		{
			template<typename T, typename P> friend void ssvu::eraseRemoveIf(T&, const P&);

			protected:
				using Container = std::vector<Uptr<TItem, TDeleter>>;
				std::vector<TItem*> toAdd;

			public:
				using Container::begin;
				using Container::end;
				using Container::size;

				inline void clear()	{ Container::clear(); toAdd.clear(); }
				inline void del(TItem& mItem) const noexcept { mItem.ssvu_mmAlive = false; }

				// Statically polymorphic methods
				inline void refresh() { reinterpret_cast<TDerived*>(this)->refreshImpl(); }
				template<typename TType = TItem, typename... TArgs> inline TType& create(TArgs&&... mArgs)
				{
					return reinterpret_cast<TDerived*>(this)->template createTImpl<TType, TArgs...>(std::forward<TArgs>(mArgs)...);
				}

				template<typename TType> inline static bool isAlive(const TType& mItem) noexcept { return mItem->ssvu_mmAlive; }
				template<typename TType> inline static bool isDead(const TType& mItem) noexcept	{ return !mItem->ssvu_mmAlive; }
		 };
	}

	template<typename T, typename TDeleter = std::default_delete<T>> class MemoryManager : public Internal::MemoryManagerBase<MemoryManager<T, TDeleter>, T, TDeleter>
	{
		public:
			inline void refreshImpl()
			{
				for(const auto& i : this->toAdd) this->emplace_back(i); this->toAdd.clear();
				eraseRemoveIf(*this, this->template isDead<Uptr<T, TDeleter>>);
			}
			template<typename TType = T, typename... TArgs> inline TType& createTImpl(TArgs&&... mArgs)
			{
				auto result(new TType(std::forward<TArgs>(mArgs)...)); this->toAdd.push_back(result); return *result;
			}
	};
}

#endif
