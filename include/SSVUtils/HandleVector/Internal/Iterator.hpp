// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_HANDLEVECTOR_INTERNAL_ITERATOR
#define SSVU_HANDLEVECTOR_INTERNAL_ITERATOR

namespace ssvu
{
	namespace Internal
	{
		template<typename T, typename TItrValue, typename TImpl> class HVecItrBase
		{
			protected:
				TItrValue value;
				TImpl impl;

			public:
				template<typename... TArgs> inline HVecItrBase(TItrValue mValue, TArgs&&... mArgs) noexcept : value{mValue}, impl{fwd<TArgs>(mArgs)...} { }

				inline auto& operator++() noexcept				{ ++value;			return *this; }
				inline auto& operator++(int) noexcept			{ ++value;			return *this; }
				inline auto& operator--() noexcept				{ --value;			return *this; }
				inline auto& operator--(int) noexcept			{ --value;			return *this; }
				inline auto& operator+=(int mOffset) noexcept	{ value += mOffset;	return *this; }
				inline auto& operator-=(int mOffset) noexcept	{ value -= mOffset;	return *this; }

				inline decltype(auto) operator*() noexcept			{ return impl.template get<T&>(value); }
				inline decltype(auto) operator*() const noexcept	{ return impl.template get<const T&>(value); }
				inline decltype(auto) operator->() noexcept			{ return &impl.template get<T&>(value); }
				inline decltype(auto) operator->() const noexcept	{ return &impl.template get<const T&>(value); }

				inline bool operator==(const HVecItrBase& mRhs) const noexcept	{ return value == mRhs.value; }
				inline bool operator!=(const HVecItrBase& mRhs) const noexcept	{ return value != mRhs.value; }
				inline bool operator<(const HVecItrBase& mRhs) const noexcept	{ return value < mRhs.value; }
				inline bool operator>(const HVecItrBase& mRhs) const noexcept	{ return value > mRhs.value; }
				inline bool operator<=(const HVecItrBase& mRhs) const noexcept	{ return value <= mRhs.value; }
				inline bool operator>=(const HVecItrBase& mRhs) const noexcept	{ return value >= mRhs.value; }
		};

		struct HVecItrImplFast
		{
			template<typename TR, typename TV> inline TR get(const TV& mValue) noexcept { return mValue->getData(); }
		};

		template<typename T> struct HVecItrImplIdx
		{
			HandleVector<T>* hVec;
			inline HVecItrImplIdx(HandleVector<T>& mHVec) noexcept : hVec(&mHVec) { }
			template<typename TR, typename TV> inline TR get(const TV& mValue) noexcept { return hVec->getDataAt(mValue); }
		};
	}

	template<typename T> using HVecItrFast = Internal::HVecItrBase<T, Internal::Atom<T>*, Internal::HVecItrImplFast>;
	template<typename T> using HVecItrIdx = Internal::HVecItrBase<T, HIdx, Internal::HVecItrImplIdx<T>>;
}

#endif
