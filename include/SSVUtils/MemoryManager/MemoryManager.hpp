// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER
#define SSVU_MEMORYMANAGER

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	struct MemoryManageable { bool ssvu_mmAlive{true}; };

	namespace Internal
	{
		template<typename TDerived, typename TBase, typename TDeleter = std::default_delete<TBase>> class MemoryManagerBase
		{
			template<typename T1, typename T2> friend void ssvu::eraseRemoveIf(T1&, const T2&);

			protected:
				using Container = VecUptr<TBase, TDeleter>;
				Container items, toAdd;

			private:
				inline TDerived& getThisDerived() noexcept { return *reinterpret_cast<TDerived*>(this); }

			public:
				inline void clear()	noexcept { items.clear(); toAdd.clear(); }
				inline void del(TBase& mItem) const noexcept { mItem.ssvu_mmAlive = false; }

				// Statically polymorphic methods
				inline void refresh() { getThisDerived().refreshImpl(); }
				template<typename T = TBase, typename... TArgs> inline T& create(TArgs&&... mArgs)
				{
					return getThisDerived().template createImpl<T>(std::forward<TArgs>(mArgs)...);
				}

				template<typename T> inline static bool isAlive(const T& mItem) noexcept	{ return mItem->ssvu_mmAlive; }
				template<typename T> inline static bool isDead(const T& mItem) noexcept		{ return !isAlive(mItem); }

				inline auto size() const noexcept -> decltype(items.size()) { return items.size(); }

				// Standard iterator support
				inline auto begin()		noexcept		-> decltype(items.begin())		{ return items.begin(); }
				inline auto end()		noexcept		-> decltype(items.end())		{ return items.end(); }
				inline auto begin()		const noexcept	-> decltype(items.begin())		{ return items.begin(); }
				inline auto end()		const noexcept	-> decltype(items.end())		{ return items.end(); }
				inline auto cbegin()	const noexcept	-> decltype(items.cbegin())		{ return items.cbegin(); }
				inline auto cend()		const noexcept	-> decltype(items.cend())		{ return items.cend(); }
				inline auto rbegin()	noexcept		-> decltype(items.rbegin())		{ return items.rbegin(); }
				inline auto rend()		noexcept		-> decltype(items.rend())		{ return items.rend(); }
				inline auto crbegin()	const noexcept	-> decltype(items.crbegin())	{ return items.crbegin(); }
				inline auto crend()		const noexcept	-> decltype(items.crend())		{ return items.crend(); }
		 };
	}

	template<typename TBase, typename TDeleter = std::default_delete<TBase>> class MemoryManager : public Internal::MemoryManagerBase<MemoryManager<TBase, TDeleter>, TBase, TDeleter>
	{
		public:
			inline void refreshImpl()
			{
				for(auto& i : this->toAdd) this->items.emplace_back(std::move(i)); this->toAdd.clear();
				eraseRemoveIf(this->items, this->template isDead<Uptr<TBase, TDeleter>>);
			}
			template<typename T = TBase, typename... TArgs> inline T& createImpl(TArgs&&... mArgs)
			{
				return getEmplaceUptr<T>(this->toAdd, std::forward<TArgs>(mArgs)...);
			}
	};
}

#endif
