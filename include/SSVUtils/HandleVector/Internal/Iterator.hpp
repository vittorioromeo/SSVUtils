// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_HANDLEVECTOR_INTERNAL_ITERATOR
#define SSVU_HANDLEVECTOR_INTERNAL_ITERATOR

namespace ssvu
{
	namespace Internal
	{
		template<typename T, typename TItrValue, typename TImpl> class HVecItrBase : public BaseAdaptorItrRnd<TItrValue, TImpl>
		{
			protected:
				TImpl impl;

			public:
				template<typename... TArgs> inline HVecItrBase(TItrValue mValue, TArgs&&... mArgs) noexcept : BaseAdaptorItrRnd<TItrValue, TImpl>{mValue}, impl{fwd<TArgs>(mArgs)...} { }

				inline decltype(auto) operator*() noexcept			{ return impl.template get<T&>(this->itr); }
				inline decltype(auto) operator*() const noexcept	{ return impl.template get<const T&>(this->itr); }
				inline decltype(auto) operator->() noexcept			{ return &impl.template get<T&>(this->itr); }
				inline decltype(auto) operator->() const noexcept	{ return &impl.template get<const T&>(this->itr); }
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

		template<typename T> struct HVecItrImplAtom
		{
			HandleVector<T>* hVec;
			inline HVecItrImplAtom(HandleVector<T>& mHVec) noexcept : hVec(&mHVec) { }
			template<typename TR, typename TV> inline TR get(const TV& mValue) noexcept { return hVec->getAtomAt(mValue); }
		};

		template<typename T> using HVecItrFast = HVecItrBase<T, Atom<T>*, HVecItrImplFast>;
		template<typename T> using HVecItrIdx = HVecItrBase<T, HIdx, HVecItrImplIdx<T>>;
		template<typename T> using HVecItrAtom = HVecItrBase<T, HIdx, HVecItrImplAtom<Atom<T>>>;
	}
}

#endif
