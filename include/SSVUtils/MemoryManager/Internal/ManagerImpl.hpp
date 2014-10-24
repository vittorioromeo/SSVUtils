// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER_INTERNAL_MANAGERIMPL
#define SSVU_MEMORYMANAGER_INTERNAL_MANAGERIMPL

namespace ssvu
{
	namespace Internal
	{
		template<typename T, typename TItrValue, typename TImpl> class MMItrBase : public BaseAdaptorItrRnd<TItrValue, TImpl>
		{
			protected:
				TImpl impl;

			public:
				template<typename... TArgs> inline MMItrBase(TItrValue mValue, TArgs&&... mArgs) noexcept
					: BaseAdaptorItrRnd<TItrValue, TImpl>{mValue}, impl{fwd<TArgs>(mArgs)...} { }

				inline decltype(auto) operator*() noexcept			{ return impl.get(this->itr); }
				inline decltype(auto) operator*() const noexcept	{ return impl.get(this->itr); }
				inline decltype(auto) operator->() noexcept			{ return &impl.get(this->itr); }
				inline decltype(auto) operator->() const noexcept	{ return &impl.get(this->itr); }
		};

		template<typename TBM> struct MMItrImplIdx
		{
			TBM* bm;
			template<typename TV> inline decltype(auto) get(const TV& mValue) noexcept { return bm->getDataAt(mValue); }
		};

		template<typename TBase, typename TBM> using MMItrIdx = MMItrBase<TBase, SizeT, MMItrImplIdx<TBM>>;

		/// @brief Base memory recycler manager class.
		/// @tparam TBase Base type of manager objects.
		/// @tparam TRecycler Internal recycler type. (MonoRecycler? PolyRecycler?)
		template<typename TBase, typename TRecycler> class BaseManager
		{
			public:
				using LayoutType = LayoutImpl::LHelperBool<TBase>;
				using ChunkType = Chunk<TBase, LayoutImpl::LHelperBool>;
				using ChunkDeleterType = ChunkDeleter<TBase, LayoutImpl::LHelperBool>;
				using PtrType = UPtr<TBase, ChunkDeleterType>;
				using RecyclerType = TRecycler;
				using Container = GrowableArray<PtrType>;
				using ItrIdx = MMItrIdx<PtrType, BaseManager<TBase, TRecycler>>;
				using ItrIdxC = MMItrIdx<PtrType, const BaseManager<TBase, TRecycler>>;

			private:
				RecyclerType recycler;
				Container items;
				SizeT msize{0u}, sizeNext{0u}, capacity{0u};

				inline bool isAliveAt(SizeT mI) const noexcept	{ return isAlive(items[mI].get()); }
				inline bool isDeadAt(SizeT mI) const noexcept	{ return isDead(items[mI].get()); }

			public:
				inline BaseManager() { reserve(25); }
				inline ~BaseManager() { clear(); }

				inline auto& getDataAt(SizeT mI) noexcept { return items[mI]; }
				inline const auto& getDataAt(SizeT mI) const noexcept { return items[mI]; }

				template<typename T = TBase, typename... TArgs> inline T& create(TArgs&&... mArgs)
				{
					auto uPtr(recycler.template create<T>(fwd<TArgs>(mArgs)...));

					if(capacity <= sizeNext) reserve(capacity * 3);

					items.initAt(sizeNext, std::move(uPtr));
					return ssvu::castUp<T>(*items[sizeNext++]);
				}

				inline void clear()	noexcept
				{
					for(auto i(0u); i < sizeNext; ++i) items.deinitAt(i);
					msize = sizeNext = 0;
				}
				inline void del(TBase& mBase) noexcept { LayoutType::setBool(&mBase, false); }

				inline void reserve(SizeT mCapacityNew)
				{
					SSVU_ASSERT(capacity < mCapacityNew);
					items.grow(capacity, mCapacityNew);
					capacity = mCapacityNew;
				}

				inline void refresh() noexcept
				{
					Internal::refreshImpl(msize, sizeNext,
						[this](SizeT mI){ return isAliveAt(mI); },
						[this](SizeT mD, SizeT mA){ std::swap(items[mD], items[mA]); },
						[this](SizeT mD){ items.deinitAt(mD); });
				}

				inline static bool isAlive(const TBase* mBase) noexcept	{ return LayoutType::getBool(mBase); }
				inline static bool isDead(const TBase* mBase) noexcept	{ return !isAlive(mBase); }

				inline auto size()		const noexcept	{ return msize; }
				inline auto begin()		noexcept		{ return ItrIdx{0, this}; }
				inline auto end()		noexcept		{ return ItrIdx{msize, this}; }
				inline auto begin()		const noexcept	{ return ItrIdxC{0, this}; }
				inline auto end()		const noexcept	{ return ItrIdxC{msize, this}; }
				/*inline auto cbegin()	const noexcept	{ return std::cbegin(items); }
				inline auto cend()		const noexcept	{ return std::cend(items); }
				inline auto rbegin()	noexcept		{ return std::rbegin(items); }
				inline auto rend()		noexcept		{ return std::rend(items); }
				inline auto crbegin()	const noexcept	{ return std::crbegin(items); }
				inline auto crend()		const noexcept	{ return std::crend(items); }*/
		};
	}
}

#endif
