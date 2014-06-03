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
		template<typename TDerived, typename TItem, typename TDeleter = std::default_delete<TItem>> class MemoryManagerBase : protected VecUptr<TItem, TDeleter>
		{
			template<typename T, typename TP> friend void ssvu::eraseRemoveIf(T&, const TP&);

			protected:
				using Container = VecUptr<TItem, TDeleter>;
				Container toAdd;

			public:
				using Container::begin;
				using Container::end;
				using Container::size;

				inline void clear()	noexcept { Container::clear(); toAdd.clear(); }
				inline void del(TItem& mItem) const noexcept { mItem.ssvu_mmAlive = false; }

				// Statically polymorphic methods
				inline void refresh() { reinterpret_cast<TDerived*>(this)->refreshImpl(); }
				template<typename TType = TItem, typename... TArgs> inline TType& create(TArgs&&... mArgs)
				{
					return reinterpret_cast<TDerived*>(this)->template createTImpl<TType, TArgs...>(std::forward<TArgs>(mArgs)...);
				}

				template<typename TType> inline static bool isAlive(const TType& mItem) noexcept	{ return mItem->ssvu_mmAlive; }
				template<typename TType> inline static bool isDead(const TType& mItem) noexcept		{ return !isAlive(mItem); }
		 };
	}

	template<typename T, typename TDeleter = std::default_delete<T>> class MemoryManager : public Internal::MemoryManagerBase<MemoryManager<T, TDeleter>, T, TDeleter>
	{
		public:
			inline void refreshImpl()
			{
				for(auto& i : this->toAdd) this->emplace_back(std::move(i)); this->toAdd.clear();
				eraseRemoveIf(*this, this->template isDead<Uptr<T, TDeleter>>);
			}
			template<typename TType = T, typename... TArgs> inline TType& createTImpl(TArgs&&... mArgs)
			{
				return getEmplaceUptr<TType>(this->toAdd, std::forward<TArgs>(mArgs)...);
			}
	};
}

#endif

// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_MEMORYMANAGER2REC
#define SSVU_MEMORYMANAGER2REC

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/RecPtr/RecPtr.hpp"

namespace ssvu
{
	template<typename> class MemoryManager2Rec;

	template<typename T, typename TMR> class MRItrIdxRec
	{
		public:
			using TUptr = UptrRec<T>;

		private:
			TMR& mr;
			std::size_t value;

		public:
			inline MRItrIdxRec(TMR& mMR, std::size_t mIdx) noexcept : mr(mMR), value(mIdx) { }

			inline MRItrIdxRec& operator++() noexcept				{ ++value;			return *this; }
			inline MRItrIdxRec& operator++(int) noexcept			{ ++value;			return *this; }
			inline MRItrIdxRec& operator--() noexcept				{ --value;			return *this; }
			inline MRItrIdxRec& operator--(int) noexcept			{ --value;			return *this; }
			inline MRItrIdxRec& operator+=(int mOffset) noexcept	{ value += mOffset;	return *this; }
			inline MRItrIdxRec& operator-=(int mOffset) noexcept	{ value -= mOffset;	return *this; }

			inline TUptr& operator*() noexcept				{ return mr.c[this->value]; }
			inline TUptr* operator->() noexcept				{ return &(mr.c[this->value]); }

			inline bool operator==(const MRItrIdxRec& mRhs) const noexcept	{ return value == mRhs.value; }
			inline bool operator!=(const MRItrIdxRec& mRhs) const noexcept	{ return value != mRhs.value; }
			inline bool operator<(const MRItrIdxRec& mRhs) const noexcept	{ return value < mRhs.value; }
			inline bool operator>(const MRItrIdxRec& mRhs) const noexcept	{ return value > mRhs.value; }
			inline bool operator<=(const MRItrIdxRec& mRhs) const noexcept	{ return value <= mRhs.value; }
			inline bool operator>=(const MRItrIdxRec& mRhs) const noexcept	{ return value >= mRhs.value; }
	};

	template<typename T, typename TMR> class MRItrIdxRecC
	{
		public:
			using TUptr = UptrRec<T>;

		private:
			TMR& mr;
			std::size_t value;

		public:
			inline MRItrIdxRecC(TMR& mMR, std::size_t mIdx) noexcept : mr(mMR), value(mIdx) { }

			inline MRItrIdxRecC& operator++() noexcept				{ ++value;			return *this; }
			inline MRItrIdxRecC& operator++(int) noexcept			{ ++value;			return *this; }
			inline MRItrIdxRecC& operator--() noexcept				{ --value;			return *this; }
			inline MRItrIdxRecC& operator--(int) noexcept			{ --value;			return *this; }
			inline MRItrIdxRecC& operator+=(int mOffset) noexcept	{ value += mOffset;	return *this; }
			inline MRItrIdxRecC& operator-=(int mOffset) noexcept	{ value -= mOffset;	return *this; }

			inline const TUptr& operator*() const noexcept	{ return mr.c.at(this->value); }
			inline const TUptr* operator->() const noexcept	{ return &(mr.c.at(this->value)); }

			inline bool operator==(const MRItrIdxRecC& mRhs) const noexcept	{ return value == mRhs.value; }
			inline bool operator!=(const MRItrIdxRecC& mRhs) const noexcept	{ return value != mRhs.value; }
			inline bool operator<(const MRItrIdxRecC& mRhs) const noexcept	{ return value < mRhs.value; }
			inline bool operator>(const MRItrIdxRecC& mRhs) const noexcept	{ return value > mRhs.value; }
			inline bool operator<=(const MRItrIdxRecC& mRhs) const noexcept	{ return value <= mRhs.value; }
			inline bool operator>=(const MRItrIdxRecC& mRhs) const noexcept	{ return value >= mRhs.value; }
	};

	namespace Internal
	{
		template<typename TDerived, typename TItem> class MemoryManager2RecBase
		{
			template<typename T, typename TP> friend void ssvu::eraseRemoveIf(T&, const TP&);

			public:
				VecUptrRec<TItem> c;
				std::size_t aliveSize{0u};

				inline void clear()	noexcept { c.clear(); aliveSize = 0u; }
				inline void del(TItem& mItem) const noexcept { mItem.ssvu_mmAlive = false; }

				// Statically polymorphic methods
				inline void refresh() { reinterpret_cast<TDerived*>(this)->refreshImpl(); }
				template<typename TType = TItem, typename... TArgs> inline TType& create(TArgs&&... mArgs)
				{
					return reinterpret_cast<TDerived*>(this)->template createTImpl<TType, TArgs...>(std::forward<TArgs>(mArgs)...);
				}

				template<typename TType> inline static bool isAlive(const TType& mItem) noexcept	{ return mItem->ssvu_mmAlive; }
				template<typename TType> inline static bool isDead(const TType& mItem) noexcept		{ return !isAlive(mItem); }
		 };
	}

	template<typename T> class MemoryManager2Rec : public Internal::MemoryManager2RecBase<MemoryManager2Rec<T>, T>
	{
		public:
			inline void refreshImpl()
			{
				int iDead{0};
				const int intSizeNext(this->c.size());

				while(iDead < intSizeNext && this->template isAlive(this->c[iDead])) ++iDead;
				int iAlive{iDead - 1};

				for(int iD{iDead}; iD < intSizeNext; ++iD)
				{
					if(this->template isAlive(this->c[iD])) continue;

					for(int iA{iDead + 1}; true; ++iA)
					{
						if(iA == intSizeNext) goto finishRefresh;

						if(!this->template isAlive(this->c[iA])) continue;

						std::swap(this->c[iA], this->c[iD]);
						iAlive = iD; iDead = iA;

						break;
					}
				}

				finishRefresh:

				this->c.erase(std::begin(this->c) + iAlive + 1, std::end(this->c));
				this->aliveSize = this->c.size();
			}
			template<typename TType = T, typename... TArgs> inline TType& createTImpl(TArgs&&... mArgs)
			{
				auto uptr(makeUptrRecPoly<TType, T>(std::forward<TArgs>(mArgs)...));
				auto result(uptr.get());
				this->c.emplace_back(std::move(uptr));
				return *result;
			}

			inline std::size_t size() const noexcept { return this->c.size(); }

			inline MRItrIdxRec<T, MemoryManager2Rec<T>> begin() noexcept { return {*this, 0}; }
			inline MRItrIdxRec<T, MemoryManager2Rec<T>> end() noexcept { return {*this, this->aliveSize}; }

			inline const MRItrIdxRecC<T, const MemoryManager2Rec<T>> begin() const noexcept { return MRItrIdxRecC<T, const MemoryManager2Rec<T>>{*this, 0}; }
			inline const MRItrIdxRecC<T, const MemoryManager2Rec<T>> end() const noexcept { return MRItrIdxRecC<T, const MemoryManager2Rec<T>>{*this, this->aliveSize}; }
	};
}

#endif
