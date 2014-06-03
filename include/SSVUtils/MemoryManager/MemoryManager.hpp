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

#ifndef SSVU_MEMORYMANAGER2
#define SSVU_MEMORYMANAGER2

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	template<typename, typename> class MemoryManager2;

	namespace Internal
	{
		template<typename T, typename TItrValue, typename TDerived> class MRItrBase
		{
			protected:
				TItrValue value;

				inline TDerived& getThisDerived() noexcept { return *reinterpret_cast<TDerived*>(this); }
				template<typename TT> inline TT getImpl() noexcept { return getThisDerived().template getImpl<TT>(); }

			public:
				inline MRItrBase(TItrValue mValue) noexcept : value{mValue} { }

				inline TDerived& operator++() noexcept				{ ++value;			return getThisDerived(); }
				inline TDerived& operator++(int) noexcept			{ ++value;			return getThisDerived(); }
				inline TDerived& operator--() noexcept				{ --value;			return getThisDerived(); }
				inline TDerived& operator--(int) noexcept			{ --value;			return getThisDerived(); }
				inline TDerived& operator+=(int mOffset) noexcept	{ value += mOffset;	return getThisDerived(); }
				inline TDerived& operator-=(int mOffset) noexcept	{ value -= mOffset;	return getThisDerived(); }

				inline T& operator*() noexcept				{ return getImpl<T&>(); }
				inline const T& operator*() const noexcept	{ return getImpl<const T&>(); }
				inline T* operator->() noexcept				{ return &(getImpl<T&>()); }
				inline const T* operator->() const noexcept	{ return &(getImpl<const T&>()); }

				inline bool operator==(const TDerived& mRhs) const noexcept	{ return value == mRhs.value; }
				inline bool operator!=(const TDerived& mRhs) const noexcept	{ return value != mRhs.value; }
				inline bool operator<(const TDerived& mRhs) const noexcept	{ return value < mRhs.value; }
				inline bool operator>(const TDerived& mRhs) const noexcept	{ return value > mRhs.value; }
				inline bool operator<=(const TDerived& mRhs) const noexcept	{ return value <= mRhs.value; }
				inline bool operator>=(const TDerived& mRhs) const noexcept	{ return value >= mRhs.value; }
		};
	}

	template<typename T, typename TDeleter> class MRItrIdx final : public Internal::MRItrBase<T, std::size_t, MRItrIdx<T, TDeleter>>
	{
		template<typename, typename, typename> friend class Internal::MRItrBase;

		private:
			MemoryManager2<T, TDeleter>* mr;
			template<typename TT> inline TT getImpl() noexcept { return *mr->c[this->value]; }

		public:
			inline MRItrIdx(MemoryManager2<T, TDeleter>& mMR, std::size_t mIdx) noexcept
				: Internal::MRItrBase<T, std::size_t, MRItrIdx<T, TDeleter>>{mIdx}, mr(&mMR) { }
	};

	namespace Internal
	{
		template<typename TDerived, typename TItem, typename TDeleter = std::default_delete<TItem>> class MemoryManager2Base
		{
			template<typename T, typename TP> friend void ssvu::eraseRemoveIf(T&, const TP&);

			public:
				VecUptr<TItem, TDeleter> c;
				std::size_t aliveSize{0u};

				inline void clear()	noexcept { c.clear(); }
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

	template<typename T, typename TDeleter = std::default_delete<T>> class MemoryManager2 : public Internal::MemoryManager2Base<MemoryManager2<T, TDeleter>, T, TDeleter>
	{
		public:
			inline void refreshImpl()
			{
				eraseRemoveIf(this->c, this->template isDead<Uptr<T, TDeleter>>);
				this->aliveSize = this->c.size();
			}
			template<typename TType = T, typename... TArgs> inline TType& createTImpl(TArgs&&... mArgs)
			{
				return getEmplaceUptr<TType>(this->c, std::forward<TArgs>(mArgs)...);
			}

			inline MRItrIdx<T, TDeleter> begin() noexcept { return {*this, 0}; }
			inline MRItrIdx<T, TDeleter> end() noexcept { return {*this, this->aliveSize}; }

			inline MRItrIdx<T, TDeleter> begin() const noexcept { return {*this, 0}; }
			inline MRItrIdx<T, TDeleter> end() const noexcept { return {*this, this->aliveSize}; }
	};
}

#endif
