// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER
#define SSVU_MEMORYMANAGER

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	template<typename> class PolyManager;
	template<typename> class MonoManager;
	template<typename, typename, typename> class BaseManager;
}

#include "SSVUtils/MemoryManager/Internal/ItemStorage.hpp"
#include "SSVUtils/MemoryManager/Internal/Impl.hpp"

namespace ssvu
{
	template<typename TBase, typename TStorage, typename TDerived> class BaseManager
	{
		template<typename T1, typename T2> friend void ssvu::eraseRemoveIf(T1&, const T2&);

		protected:
			using ChunkType = Internal::Chunk;
			using ChunkDeleterType = Internal::ChunkDeleter<TBase>;
			using PtrType = Uptr<TBase, ChunkDeleterType>;
			using Container = std::vector<PtrType>;
			TStorage storage;
			Container items, toAdd;

		public:
			template<typename T = TBase, typename... TArgs> inline T& create(TArgs&&... mArgs)
			{
				SSVU_ASSERT_STATIC(sizeof(TBase) >= sizeof(char*), "sizeof(TBase) must be >= sizeof(char*)");

				return reinterpret_cast<TDerived*>(this)->template createImpl<T>(std::forward<TArgs>(mArgs)...);
			}

			inline void clear()	noexcept { items.clear(); toAdd.clear(); }
			inline void del(TBase& mBase) noexcept { Internal::ItemStorage::setBool(&mBase, false); }

			inline void reserve(std::size_t mCapacity)
			{
				SSVU_ASSERT(items.capacity() < mCapacity);
				items.reserve(mCapacity);
				toAdd.reserve(mCapacity);
			}

			inline void refresh()
			{
				auto kItr(std::begin(toAdd));

				// While there currently are items in the main container...
				for(auto iItr(std::begin(items)); iItr != std::end(items); ++iItr)
				{
					// Skip alive items
					if(TDerived::isAlive(iItr->get())) continue;

					// Found a dead item - no more items to add
					if(kItr == std::end(toAdd))
					{
						// Erase-remove-if all dead items from this point forward and exit
						items.erase(std::remove_if(iItr, std::end(items), [](const PtrType& mP){ return TDerived::isDead(mP.get()); }), std::end(items));
						toAdd.clear();
						return;
					}

					// Found a dead item - items to add
					*iItr = std::move(*kItr++);
				}

				// Emplace_back remaining items in the main container
				while(kItr != std::end(toAdd)) items.emplace_back(std::move(*kItr++));
				toAdd.clear();
			}

			inline static bool isAlive(const TBase* mBase) noexcept	{ return Internal::ItemStorage::getBool(mBase); }
			inline static bool isDead(const TBase* mBase) noexcept	{ return !isAlive(mBase); }

			inline auto size()		const noexcept	-> decltype(items.size())		{ return items.size(); }
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

	template<typename TBase> class MonoManager final : public Internal::MonoManagerBase<TBase>
	{
		friend Internal::MonoManagerBase<TBase>;

		private:
			using ChunkDeleterType = Internal::ChunkDeleter<TBase>;

			template<typename T, typename... TArgs> inline T& createImpl(TArgs&&... mArgs)
			{
				SSVU_ASSERT_STATIC(isSame<TBase, T>(), "MonoManager can only store objects of the same type");

				auto result(this->storage.chunk.template create<T>(std::forward<TArgs>(mArgs)...));
				this->toAdd.emplace_back(result, ChunkDeleterType{this->storage.chunk});
				return *result;
			}
	};

	template<typename TBase> class PolyManager final : public Internal::PolyManagerBase<TBase>
	{
		friend Internal::PolyManagerBase<TBase>;

		private:
			using ChunkDeleterType = Internal::ChunkDeleter<TBase>;

			template<typename T, typename... TArgs> inline T& createImpl(TArgs&&... mArgs)
			{
				SSVU_ASSERT_STATIC(isSame<TBase, T>() || isBaseOf<TBase, T>(), "PolyManager can only store types that belong to the same hierarchy");

				auto& chunk(this->storage.template getChunk<T>());
				auto result(chunk.template create<T>(std::forward<TArgs>(mArgs)...));
				this->toAdd.emplace_back(result, ChunkDeleterType{chunk});
				return *result;
			}
	};
}

#endif
