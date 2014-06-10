// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER_INTERNAL_IMPL
#define SSVU_MEMORYMANAGER_INTERNAL_IMPL

namespace ssvu
{
	namespace Internal
	{
		struct Chain { Chain* next; };

		class Chunk
		{
			private:
				Chain* chain{nullptr};

			public:
				inline ~Chunk()
				{
					Chain* temp;
					while(chain != nullptr)
					{
						temp = chain;
						chain = chain->next;
						ItemStorage::deallocate(reinterpret_cast<char*>(temp));
					}
				}

				template<typename T, typename... TArgs> inline T* create(TArgs&&... mArgs)
				{
					char* result;

					if(chain == nullptr) result = ItemStorage::allocate<T>();
					else
					{
						result = reinterpret_cast<char*>(chain);
						chain = chain->next;
					}

					ItemStorage::construct<T>(result, std::forward<TArgs>(mArgs)...);

					return ItemStorage::getBase<T>(result);
				}

				template<typename TBase> inline void recycle(TBase* mBase)
				{
					ItemStorage::destroy(mBase);

					auto newHead(reinterpret_cast<Chain*>(ItemStorage::getByte(mBase)));
					newHead->next = chain;
					chain = newHead;
				}
		};

		template<typename TBase> class ChunkDeleter
		{
			private:
				Chunk* chunk;

			public:
				inline ChunkDeleter(Chunk& mChunk) noexcept : chunk{&mChunk} { }
				inline void operator()(TBase* mPtr) const { chunk->recycle<TBase>(mPtr); }
		};

		struct MonoStorage { Chunk chunk; };

		class PolyStorage
		{
			private:
				std::map<std::size_t, Chunk> chunks;

			public:
				template<typename T> inline Chunk& getChunk() { return chunks[sizeof(T)]; }
				template<typename T, typename... TArgs> inline T* create(Chunk& mChunk, TArgs&&... mArgs)
				{
					return mChunk.create<T>(std::forward<TArgs>(mArgs)...);
				}
		};

		template<typename TBase> using MonoManagerBase = BaseManager<TBase, MonoStorage, MonoManager<TBase>>;
		template<typename TBase> using PolyManagerBase = BaseManager<TBase, PolyStorage, PolyManager<TBase>>;
	}
}

#endif
