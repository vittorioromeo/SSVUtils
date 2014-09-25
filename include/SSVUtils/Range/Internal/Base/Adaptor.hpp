// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_RANGE_INTERNAL_BASE_ADAPTOR
#define SSVU_RANGE_INTERNAL_BASE_ADAPTOR

namespace ssvu
{
	namespace Internal
	{
		/// @brief Iterator adaptor for forward iterators. Implementation of the dereference is used via `TImpl::get`.
		template<typename TItr, typename TImpl> class BaseAdaptorItrFwd
		{
			protected:
				TItr itr;

			public:
				inline BaseAdaptorItrFwd(const TItr& mItr) noexcept : itr{mItr} { }

				inline decltype(auto) operator*() noexcept			{ return TImpl::get(itr); }
				inline decltype(auto) operator*() const noexcept	{ return TImpl::get(itr); }
				inline decltype(auto) operator->() noexcept			{ return &TImpl::get(itr); }
				inline decltype(auto) operator->() const noexcept	{ return &TImpl::get(itr); }

				inline auto& operator++() noexcept		{ ++this->itr; return *this; }
				inline auto& operator++(int) noexcept	{ ++this->itr; return *this; }

				inline bool operator==(const BaseAdaptorItrFwd& mRhs) const noexcept	{ return this->itr == mRhs.itr; }
				inline bool operator!=(const BaseAdaptorItrFwd& mRhs) const noexcept	{ return this->itr != mRhs.itr; }
		};

		/// @brief Iterator adaptor for bidirectional iterators.
		template<typename TItr, typename TImpl> struct BaseAdaptorItrBdr : public BaseAdaptorItrFwd<TItr, TImpl>
		{
			inline BaseAdaptorItrBdr(const TItr& mItr) noexcept : BaseAdaptorItrFwd<TItr, TImpl>{mItr} { }

			inline auto& operator--() noexcept		{ --this->itr; return *this; }
			inline auto& operator--(int) noexcept	{ --this->itr; return *this; }
		};

		/// @brief Iterator adaptor for random iterators.
		template<typename TItr, typename TImpl> struct BaseAdaptorItrRnd : public BaseAdaptorItrBdr<TItr, TImpl>
		{
			inline BaseAdaptorItrRnd(const TItr& mItr) noexcept : BaseAdaptorItrBdr<TItr, TImpl>{mItr} { }

			inline auto& operator+=(int mOffset) noexcept	{ this->itr += mOffset;	return *this; }
			inline auto& operator-=(int mOffset) noexcept	{ this->itr -= mOffset;	return *this; }

			inline bool operator<(const BaseAdaptorItrRnd& mRhs) const noexcept		{ return this->itr < mRhs.itr; }
			inline bool operator>(const BaseAdaptorItrRnd& mRhs) const noexcept		{ return this->itr > mRhs.itr; }
			inline bool operator<=(const BaseAdaptorItrRnd& mRhs) const noexcept	{ return this->itr <= mRhs.itr; }
			inline bool operator>=(const BaseAdaptorItrRnd& mRhs) const noexcept	{ return this->itr >= mRhs.itr; }
		};
	}
}

#endif
