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
		template<typename T> inline void chainPush(Chain*& mChain, T mItem) noexcept
		{
			reinterpret_cast<Chain*>(mItem)->next = mChain;
			mChain = reinterpret_cast<Chain*>(mItem);
		}
		template<typename T> inline T chainPop(Chain*& mChain) noexcept
		{
			auto result(reinterpret_cast<T>(mChain));
			mChain = mChain->next;
			return result;
		}

		template<typename TBase, template<typename> class TLHelper> class Chunk
		{
			private:
				using LHelperType = TLHelper<TBase>;
				Chain* chain{nullptr};

			public:
				inline ~Chunk() noexcept
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
					char* result{chain == nullptr ? LHelperType::template allocate<T>() : chainPop<char*>(chain)};
					LHelperType::template construct<T>(result, std::forward<TArgs>(mArgs)...);
					return LHelperType::template getItem<T>(result);
				}

				inline void recycle(TBase* mBase) noexcept(noexcept(LHelperType::destroy(mBase)))
				{
					LHelperType::destroy(mBase);
					chainPush(chain, LHelperType::getByte(mBase));
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
				inline void operator()(TBase* mPtr) const noexcept(noexcept(chunk->recycle(mPtr))) { chunk->recycle(mPtr); }
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
		};
	}
}

#endif
