// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER
#define SSVU_MEMORYMANAGER

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	struct MemoryManageable { bool ssvu_mmAlive{true}; };

	template<typename> class PolyManager;
	template<typename> class MonoManager;
	template<typename, typename, typename> class BaseManager;

	namespace Internal
	{
		template<typename TBase> class Item
		{
			public:
				bool alive;
				TBase item;
		};

		template<typename TBase> inline constexpr Item<TBase>* getItemFromBase(const TBase* mBase) noexcept
		{
			SSVU_ASSERT_STATIC(isStandardLayout<Item<TBase>>(), "Item<TBase> must have standard layout");
			return reinterpret_cast<Item<TBase>*>(const_cast<char*>(reinterpret_cast<const char*>(mBase) - offsetof(Item<TBase>, item)));
		}

		template<typename TBase> inline constexpr TBase& getBaseFromItem(const Item<TBase>* mItem) noexcept { return mItem->base; }

		template<typename TBase> class Chunk
		{
			private:
				std::vector<TBase*> ptrs;

			public:
				inline ~Chunk() { for(auto p : ptrs) delete p; }

				inline void push(TBase* mPtr) noexcept(noexcept(ptrs.emplace_back(mPtr))) { ptrs.emplace_back(mPtr); }
				inline TBase* pop() noexcept(noexcept(ptrs.pop_back()))
				{
					auto result(ptrs.back());
					ptrs.pop_back();
					return result;
				}

				inline bool isEmpty() const noexcept { return ptrs.empty(); }
				inline void recycle(TBase* mPtr) { mPtr->TBase::~TBase(); push(mPtr); }
		};

		template<typename TBase> class ChunkDeleter
		{
			private:
				Chunk<TBase>* chunk;

			public:
				inline ChunkDeleter(Chunk<TBase>& mChunk) noexcept : chunk{&mChunk} { }
				inline void operator()(TBase* mPtr) const { chunk->recycle(mPtr); }
		};

		template<typename TBase> class PolyStorage
		{
			private:
				std::map<std::size_t, Chunk<TBase>> chunks;

			public:
				template<typename T> inline Chunk<TBase>& getChunk() { return chunks[sizeof(T)]; }

				template<typename T, typename... TArgs> inline T* create(Chunk<TBase>& mChunk, TArgs&&... mArgs)
				{
					std::allocator<T> alloc;

					auto result(mChunk.isEmpty() ? alloc.allocate(1) : mChunk.pop());
					//alloc.construct(result, std::forward<TArgs>(mArgs)...);
					new (result) T(std::forward<TArgs>(mArgs)...);

					return reinterpret_cast<T*>(result);
				}
		};

		template<typename TBase> class MonoStorage
		{
			public:
				Chunk<TBase> chunk;
				std::allocator<TBase> alloc;
		};

		template<typename TBase> using PolyManagerBase = BaseManager<TBase, PolyStorage<TBase>, PolyManager<TBase>>;
		template<typename TBase> using MonoManagerBase = BaseManager<TBase, MonoStorage<TBase>, MonoManager<TBase>>;
	}

	template<typename TBase, typename TStorage, typename TDerived> class BaseManager
	{
		template<typename T1, typename T2> friend void ssvu::eraseRemoveIf(T1&, const T2&);

		protected:
			using ChunkType = Internal::Chunk<TBase>;
			using ChunkDeleterType = Internal::ChunkDeleter<TBase>;
			using PtrType = Uptr<TBase, ChunkDeleterType>;
			using Container = std::vector<PtrType>;
			TStorage storage;
			Container items, toAdd;

		public:
			template<typename T = TBase, typename... TArgs> inline T& create(TArgs&&... mArgs)
			{
				return reinterpret_cast<TDerived*>(this)->template createImpl<T>(std::forward<TArgs>(mArgs)...);
			}

			inline void clear()	noexcept { items.clear(); toAdd.clear(); }
			inline void del(TBase& mItem) const noexcept { mItem.ssvu_mmAlive = false; }

			inline void refresh()
			{
				for(auto& i : this->toAdd) this->items.emplace_back(std::move(i)); this->toAdd.clear();
				eraseRemoveIf(this->items, this->template isDead<PtrType>);
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

	template<typename TBase> class PolyManager : public Internal::PolyManagerBase<TBase>
	{
		friend Internal::PolyManagerBase<TBase>;

		private:
			using ChunkDeleterType = Internal::ChunkDeleter<TBase>;

			template<typename T, typename... TArgs> inline T& createImpl(TArgs&&... mArgs)
			{
				auto& chunk(this->storage.template getChunk<T>());
				auto result(this->storage.template create<T>(chunk, std::forward<TArgs>(mArgs)...));
				this->toAdd.emplace_back(result, ChunkDeleterType{chunk});
				return *result;
			}
	 };

	template<typename TBase> class MonoManager : public Internal::MonoManagerBase<TBase>
	{
//		SSVU_ASSERT_STATIC(!isPolymorphic<T>(), "T must not be polymorphic");
		friend Internal::MonoManagerBase<TBase>;

		private:
			using ChunkDeleterType = Internal::ChunkDeleter<TBase>;

			template<typename T, typename... TArgs> inline T& createImpl(TArgs&&... mArgs)
			{
				auto result(this->storage.chunk.isEmpty() ? this->storage.alloc.allocate(1) : this->storage.chunk.pop());
				this->storage.alloc.construct(result, std::forward<TArgs>(mArgs)...);
				this->toAdd.emplace_back(result, ChunkDeleterType{this->storage.chunk});
				return *result;
			}
	 };
}

#endif

// TODO: BIMAP LIKE PAIR STORAGE?? (to avoid MemoryMangeable)
