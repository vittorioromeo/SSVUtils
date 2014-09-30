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

		namespace Impl
		{
			template<typename> struct AdaptorFromTag;
			template<> struct AdaptorFromTag<std::forward_iterator_tag>			{ template<typename TItr, typename TImpl> using Type = BaseAdaptorItrFwd<TItr, TImpl>; };
			template<> struct AdaptorFromTag<std::bidirectional_iterator_tag>	{ template<typename TItr, typename TImpl> using Type = BaseAdaptorItrBdr<TItr, TImpl>; };
			template<> struct AdaptorFromTag<std::random_access_iterator_tag>	{ template<typename TItr, typename TImpl> using Type = BaseAdaptorItrRnd<TItr, TImpl>; };
		}

		/// @typedef Adaptor type for a specific iterator tag type.
		template<typename T, typename TItr, typename TImpl> using AdaptorFromTag = typename Impl::AdaptorFromTag<T>::template Type<TItr, TImpl>;

		namespace Impl
		{
			template<typename TItr> struct AdaptorFromItr
			{
				using Tag = typename std::iterator_traits<TItr>::iterator_category;
				template<typename TImpl> using Type = Internal::AdaptorFromTag<Tag, TItr, TImpl>;
			};
		}

		/// @typedef Adaptor type for a specific iterator type.
		template<typename TItr, typename TImpl> using AdaptorFromItr = typename Impl::AdaptorFromItr<TItr>::template Type<TImpl>;

		namespace Impl
		{
			template<typename TC> struct AdaptorFromContainer
			{
				template<typename TImpl> using Type = Internal::AdaptorFromItr<decltype(std::begin(std::declval<TC>())), TImpl>;
			};
		}

		/// @typedef Adaptor type for a specific container type.
		template<typename TC, typename TImpl> using AdaptorFromContainer = typename Impl::AdaptorFromContainer<TC>::template Type<TImpl>;
	}
}

#endif
