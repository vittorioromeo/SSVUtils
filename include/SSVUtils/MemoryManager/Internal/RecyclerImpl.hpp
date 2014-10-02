// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER_INTERNAL_RECYCLERIMPL
#define SSVU_MEMORYMANAGER_INTERNAL_RECYCLERIMPL

namespace ssvu
{
	namespace Internal
	{
		template<typename TBase, template<typename> class TLHelper>						using MonoRecyclerBase = BaseRecycler<TBase, TLHelper, MonoStorage<TBase, TLHelper>, MonoRecyclerImpl<TBase, TLHelper>>;
		template<typename TBase, template<typename> class TLHelper, typename TStorage>	using PolyRecyclerBase = BaseRecycler<TBase, TLHelper, TStorage, PolyRecyclerImpl<TBase, TLHelper, TStorage>>;

		/// @brief Base CRTP recycler implementation.
		/// @tparam TBase Base type of stored objects.
		/// @tparam TLHelper Type of layout helper. (bool or no bool?)
		/// @tparam TStorage Type of storage. (MonoStorage? PolyStorage?)
		/// @tparam TDerived Type of derived class. (CRTP)
		template<typename TBase, template<typename> class TLHelper, typename TStorage, typename TDerived> class BaseRecycler
		{
			public:
				using LayoutType = TLHelper<TBase>;
				using ChunkType = Chunk<TBase, TLHelper>;
				using ChunkDeleterType = ChunkDeleter<TBase, TLHelper>;
				using StorageType = TStorage;
				using PtrType = UPtr<TBase, ChunkDeleterType>;
				using DerivedType = TDerived;

			protected:
				StorageType storage;

			private:
				inline auto& getTD() noexcept { return castUp<DerivedType>(*this); }

			public:
				/// @brief Creates a `T` instance and returns a `PtrType` to it.
				template<typename T = TBase, typename... TArgs> inline auto create(TArgs&&... mArgs)
				{
					return getTD().template createImpl<T>(fwd<TArgs>(mArgs)...);
				}

				/// @brief Creates a `T` instance and emplaces its `PtrType` back into `mContainer`. Returns a reference to the instance.
				/// @param mContainer Container where the created `PtrType` will be emplaced back.
				template<typename T = TBase, typename TContainer, typename... TArgs> inline T& getCreateEmplace(TContainer& mContainer, TArgs&&... mArgs)
				{
					mContainer.emplace_back(create<T>(fwd<TArgs>(mArgs)...));
					return castUp<T>(*mContainer.back());
				}
		};

		/// @brief CRTP implementation for `MonoRecycler`.
		template<typename TBase, template<typename> class TLHelper> struct MonoRecyclerImpl final : public MonoRecyclerBase<TBase, TLHelper>
		{
			using BaseType = MonoRecyclerBase<TBase, TLHelper>;
			using PtrType = typename BaseType::PtrType;
			using ChunkDeleterType = typename BaseType::ChunkDeleterType;

			template<typename T, typename... TArgs> inline auto createImpl(TArgs&&... mArgs)
			{
				SSVU_ASSERT_STATIC(isSame<TBase, T>(), "MonoRecyclerImpl can only allocate objects of the same type");
				return PtrType{this->storage.chunk.template create<T>(fwd<TArgs>(mArgs)...), ChunkDeleterType{this->storage.chunk}};
			}
		};

		/// @brief CRTP implementation for `PolyRecycler`.
		template<typename TBase, template<typename> class TLHelper, typename TStorage> struct PolyRecyclerImpl final : public PolyRecyclerBase<TBase, TLHelper, TStorage>
		{
			using BaseType = PolyRecyclerBase<TBase, TLHelper, TStorage>;
			using PtrType = typename BaseType::PtrType;
			using ChunkDeleterType = typename BaseType::ChunkDeleterType;

			template<typename T, typename... TArgs> inline auto createImpl(TArgs&&... mArgs)
			{
				SSVU_ASSERT_STATIC(isSameOrBaseOf<TBase, T>(), "PolyRecyclerImpl can only allocate types that belong to the same hierarchy");
				auto& chunk(this->storage.template getChunk<T>());
				return PtrType{chunk.template create<T>(fwd<TArgs>(mArgs)...), ChunkDeleterType{chunk}};
			}
		};
	}
}

#endif
