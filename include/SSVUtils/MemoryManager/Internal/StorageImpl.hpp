// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER_INTERNAL_STORAGEIMPL
#define SSVU_MEMORYMANAGER_INTERNAL_STORAGEIMPL

namespace ssvu
{
	namespace Internal
	{
		/// @brief Internal pointer chain data structure.
		/// @details Stored pointers contain a link to the next one in the chain when unused,
		/// otherwise they point to an allocated space that can be used for recycling.
		/// The memory pointed to from the stored pointers gets deallocated on destruction.
		template<typename TBase, template<typename> class TLHelper> class PtrChain
		{
			private:
				using LHelperType = TLHelper<TBase>;

				struct Link { Link* next; };
				Link* base{nullptr};

			public:
				inline PtrChain() noexcept
				{
					SSVU_ASSERT_STATIC(sizeof(TBase) >= sizeof(char*), "sizeof(TBase) must be >= sizeof(char*)");
				}

				inline PtrChain(const PtrChain&) = delete;
				inline PtrChain(PtrChain&& mPC) noexcept
				{
					base = mPC.base; mPC.base = nullptr;
				}

				inline auto& operator=(const PtrChain&) = delete;
				inline auto& operator=(PtrChain&& mPC) noexcept
				{
					base = mPC.base; mPC.base = nullptr;
					return *this;
				}

				inline ~PtrChain() noexcept
				{
					Link* temp;
					while(base != nullptr)
					{
						temp = base;
						base = base->next;
						LHelperType::deallocate(reinterpret_cast<char*>(temp));
					}
				}

				/// @brief Push a pointer in the chain. Assumes the contents of the pointer were destroyed.
				template<typename T> inline void push(T* mItem) noexcept
				{
					reinterpret_cast<Link*>(mItem)->next = base;
					base = reinterpret_cast<Link*>(mItem);
				}

				/// @brief Pops and returns a pointer from the chain.
				template<typename T> inline T* pop() noexcept
				{
					auto result(reinterpret_cast<char*>(base));
					base = base->next;
					return reinterpret_cast<T*>(result);
				}

				/// @brief Returns true if the pointer chain is empty.
				inline bool isEmpty() const noexcept { return base == nullptr; }
		};

		/// @brief Memory "chunk" storage structure for a certain object type.
		template<typename TBase, template<typename> class TLHelper> class Chunk
		{
			private:
				using LHelperType = TLHelper<TBase>;
				template<typename T> using Lyt = typename LHelperType::template Lyt<T>;

				PtrChain<TBase, TLHelper> ptrChain;

			public:
				inline Chunk() noexcept = default;

				inline Chunk(const Chunk&) = delete;
				inline Chunk(Chunk&& mC) noexcept
				{
					ptrChain = std::move(mC.ptrChain);
				}

				inline auto& operator=(const Chunk&) = delete;
				inline auto& operator=(Chunk&& mC) noexcept
				{
					ptrChain = std::move(mC.ptrChain);
					return *this;
				}

				/// @brief Creates and constructs a `T` instance.
				/// @details Uses one of the recyclable pointers if available, otherwise allocates new memory.
				template<typename T, typename... TArgs> inline T* create(TArgs&&... mArgs)
				{
					auto result(SSVU_UNLIKELY(ptrChain.isEmpty()) ? LHelperType::template allocate<T>() : ptrChain.template pop<Lyt<T>>());
					LHelperType::template construct<T>(result, fwd<TArgs>(mArgs)...);
					return castStorage<T>(&result->storageItem);
				}

				/// @brief Destroys a pointer that is in use. Memory does not get allocated - it gets recycled instead.
				inline void recycle(TBase* mBase) noexcept(noexcept(LHelperType::destroy(mBase)))
				{
					LHelperType::destroy(mBase);
					ptrChain.push(LHelperType::getLayout(mBase));
				}
		};

		/// @brief Deleter functor used for the recycled smart pointers.
		template<typename TBase, template<typename> class TLHelper> class ChunkDeleter
		{
			public:
				using ChunkType = Chunk<TBase, TLHelper>;

			private:
				ChunkType* chunk{nullptr};

			public:
				inline ChunkDeleter() noexcept = default;
				inline ChunkDeleter(ChunkType& mChunk) noexcept : chunk{&mChunk} { }
				inline void operator()(TBase* mPtr) const noexcept(noexcept(chunk->recycle(mPtr)))
				{
					SSVU_ASSERT(chunk != nullptr);
					chunk->recycle(mPtr);
				}
		};

		/// @brief Storage data structure for a single type - uses a single `Chunk`.
		template<typename TBase, template<typename> class TLHelper> struct MonoStorage
		{
			using ChunkType = Chunk<TBase, TLHelper>;
			ChunkType chunk;
		};

		/// @brief Storage data structure for multiple types - uses a map of `Chunk` objects.
		template<typename TBase, template<typename> class TLHelper> class PolyStorage
		{
			public:
				using ChunkType = Chunk<TBase, TLHelper>;

			private:
				std::unordered_map<SizeT, ChunkType> chunks;

			public:
				template<typename T> inline auto& getChunk() { return chunks[sizeof(T)]; }
		};

		namespace FixedStorageImpl
		{
			/// @typedef Type of size index.
			using SizeIdx = SizeT;

			/// @brief Returns a new unique idx.
			inline auto getLastSizeIdx() noexcept
			{
				static SizeIdx lastSizeIdx{0}; return lastSizeIdx++;
			}

			// Stores a specific index for a size
			template<SizeT TS> struct SizeIdxInfo { static SizeIdx idx; };
			template<SizeT TS> SizeIdx SizeIdxInfo<TS>::idx{getLastSizeIdx()};

			/// @brief Returns an unique idx attached to a specific object size.
			template<SizeT TS> inline auto getSizeIdx() noexcept { return SizeIdxInfo<TS>::idx; }
		}

		/// @brief Storage data structure for multiple types - uses a map of `Chunk` objects. Supports a limited number of object sizes.
		template<typename TBase, template<typename> class TLHelper, SizeT TMaxChunks> class PolyFixedStorage
		{
			public:
				using ChunkType = Chunk<TBase, TLHelper>;

			private:
				std::array<ChunkType, TMaxChunks> chunks;

			public:
				template<typename T> inline auto& getChunk() noexcept
				{
					SSVU_ASSERT(FixedStorageImpl::getSizeIdx<sizeof(T)>() < TMaxChunks);
					return chunks[FixedStorageImpl::getSizeIdx<sizeof(T)>()];
				}
		};
	}
}

#endif
