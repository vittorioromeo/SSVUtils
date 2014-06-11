// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER_INTERNAL_RECYCLERIMPL
#define SSVU_MEMORYMANAGER_INTERNAL_RECYCLERIMPL

namespace ssvu
{
	namespace Internal
	{
		template<typename TBase, template<typename> class TLHelper> using MonoRecyclerBase = BaseRecycler<TBase, TLHelper, MonoStorage, MonoRecyclerImpl>;
		template<typename TBase, template<typename> class TLHelper> using PolyRecyclerBase = BaseRecycler<TBase, TLHelper, PolyStorage, PolyRecyclerImpl>;

		template<typename TBase, template<typename> class TLHelper, template<typename, template<typename> class> class TStorage, template<typename, template<typename> class> class TDerived> class BaseRecycler
		{
			public:
				using LayoutType = TLHelper<TBase>;
				using ChunkType = Chunk<TBase, TLHelper>;
				using ChunkDeleterType = ChunkDeleter<TBase, TLHelper>;
				using StorageType = TStorage<TBase, TLHelper>;
				using PtrType = Uptr<TBase, ChunkDeleterType>;
				using DerivedType = TDerived<TBase, TLHelper>;

			protected:
				 StorageType storage;

			public:
				template<typename T = TBase, typename... TArgs> inline PtrType create(TArgs&&... mArgs)
				{
					SSVU_ASSERT_STATIC(sizeof(TBase) >= sizeof(char*), "sizeof(TBase) must be >= sizeof(char*)");
					return reinterpret_cast<DerivedType*>(this)->template createImpl<T>(std::forward<TArgs>(mArgs)...);
				}
		};

		template<typename TBase, template<typename> class TLHelper> struct MonoRecyclerImpl final : public MonoRecyclerBase<TBase, TLHelper>
		{
			using BaseType = MonoRecyclerBase<TBase, TLHelper>;
			using PtrType = typename BaseType::PtrType;
			using ChunkDeleterType = typename BaseType::ChunkDeleterType;

			template<typename T, typename... TArgs> inline PtrType createImpl(TArgs&&... mArgs)
			{
				SSVU_ASSERT_STATIC(isSame<TBase, T>(), "MonoRecyclerImpl can only allocate objects of the same type");
				auto result(this->storage.chunk.template create<T>(std::forward<TArgs>(mArgs)...));
				return PtrType{result, ChunkDeleterType{this->storage.chunk}};
			}
		};

		template<typename TBase, template<typename> class TLHelper> struct PolyRecyclerImpl final : public PolyRecyclerBase<TBase, TLHelper>
		{
			using BaseType = PolyRecyclerBase<TBase, TLHelper>;
			using PtrType = typename BaseType::PtrType;
			using ChunkDeleterType = typename BaseType::ChunkDeleterType;

			template<typename T, typename... TArgs> inline PtrType createImpl(TArgs&&... mArgs)
			{
				SSVU_ASSERT_STATIC(isSame<TBase, T>() || isBaseOf<TBase, T>(), "PolyRecyclerImpl can only allocate types that belong to the same hierarchy");
				auto& chunk(this->storage.template getChunk<T>());
				auto result(chunk.template create<T>(std::forward<TArgs>(mArgs)...));
				return PtrType{result, ChunkDeleterType{chunk}};
			}
		};
	}
}

#endif
