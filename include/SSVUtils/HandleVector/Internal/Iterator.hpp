// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_HANDLEVECTOR_INTERNAL_ITERATOR
#define SSVU_HANDLEVECTOR_INTERNAL_ITERATOR

namespace ssvu
{
	namespace Internal
	{
		template<typename T, typename TItrValue, typename TDerived> class HVecItrBase
		{
			protected:
				TItrValue value;

				inline TDerived& getThisDerived() noexcept { return *reinterpret_cast<TDerived*>(this); }
				template<typename TT> inline TT getImpl() noexcept { return getThisDerived().template getImpl<TT>(); }

			public:
				inline HVecItrBase(TItrValue mValue) noexcept : value{mValue} { }

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

	template<typename T> class HVecItrFast final : public Internal::HVecItrBase<T, Internal::Atom<T>*, HVecItrFast<T>>
	{
		template<typename, typename, typename> friend class Internal::HVecItrBase;

		private:
			template<typename TT> inline TT getImpl() noexcept { return this->value->getData(); }

		public:
			inline HVecItrFast(Internal::Atom<T>* mAtomPtr) noexcept: Internal::HVecItrBase<T, Internal::Atom<T>*, HVecItrFast<T>>{mAtomPtr} { }
	};

	template<typename T> class HVecItrIdx final : public Internal::HVecItrBase<T, HIdx, HVecItrIdx<T>>
	{
		template<typename, typename, typename> friend class Internal::HVecItrBase;

		private:
			HandleVector<T>* hVec;
			template<typename TT> inline TT getImpl() noexcept { return hVec->getDataAt(this->value); }

		public:
			inline HVecItrIdx(HandleVector<T>& mHVec, HIdx mIdx) noexcept : Internal::HVecItrBase<T, HIdx, HVecItrIdx<T>>{mIdx}, hVec(&mHVec) { }
	};
}

#endif
