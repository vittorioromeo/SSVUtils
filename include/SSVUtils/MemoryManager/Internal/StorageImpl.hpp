// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER_INTERNAL_STORAGEIMPL
#define SSVU_MEMORYMANAGER_INTERNAL_STORAGEIMPL

namespace ssvu
{
	namespace Internal
	{
		struct Chain { Chain* next; };

		template<typename TBase, template<typename> class TLHelper> class Chunk
		{
			private:
				using LHelperType = TLHelper<TBase>;
				Chain* chain{nullptr};

			public:
				inline ~Chunk()
				{
					Chain* temp;
					while(chain != nullptr)
					{
						temp = chain;
						chain = chain->next;
						LHelperType::deallocate(reinterpret_cast<char*>(temp));
					}
				}

				template<typename T, typename... TArgs> inline T* create(TArgs&&... mArgs)
				{
					char* result;

					if(chain == nullptr) result = LHelperType::template allocate<T>();
					else
					{
						result = reinterpret_cast<char*>(chain);
						chain = chain->next;
					}

					LHelperType::template construct<T>(result, std::forward<TArgs>(mArgs)...);

					return LHelperType::template getBase<T>(result);
				}

				inline void recycle(TBase* mBase)
				{
					LHelperType::destroy(mBase);

					auto newHead(reinterpret_cast<Chain*>(LHelperType::getByte(mBase)));
					newHead->next = chain;
					chain = newHead;
				}
		};

		template<typename TBase, template<typename> class TLHelper> class ChunkDeleter
		{
			public:
				using ChunkType = Chunk<TBase, TLHelper>;

			private:
				ChunkType* chunk;

			public:
				inline ChunkDeleter(ChunkType& mChunk) noexcept : chunk{&mChunk} { }
				inline void operator()(TBase* mPtr) const { chunk->recycle(mPtr); }
		};

		template<typename TBase, template<typename> class TLHelper> struct MonoStorage
		{
			using ChunkType = Chunk<TBase, TLHelper>;
			ChunkType chunk;
		};

		template<typename TBase, template<typename> class TLHelper> class PolyStorage
		{
			public:
				using ChunkType = Chunk<TBase, TLHelper>;

			private:
				std::map<std::size_t, ChunkType> chunks;

			public:
				template<typename T> inline ChunkType& getChunk() { return chunks[sizeof(T)]; }
				template<typename T, typename... TArgs> inline T* create(ChunkType mChunk, TArgs&&... mArgs)
				{
					return mChunk.template create<T>(std::forward<TArgs>(mArgs)...);
				}
		};
	}
}

#endif
