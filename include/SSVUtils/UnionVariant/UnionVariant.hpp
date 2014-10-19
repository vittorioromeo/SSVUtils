// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UNIONVARIANT
#define SSVU_UNIONVARIANT

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/UnionVariant/Internal/CTUtils.hpp"

namespace ssvu
{
	namespace Internal
	{
		template<typename... TTs> class UnionVariantBase
		{
			protected:
				AlignedStorage<getCTMaxSize<TTs...>(), getCTMaxAlign<TTs...>()> data;

				template<typename T, typename... TArgs> inline void initImpl(TArgs&&... mArgs) noexcept(isNothrowCtor<T, TArgs...>())
				{
					SSVU_ASSERT_STATIC_NM(CTHas<T, TTs...>());
					new (&data) T(fwd<TArgs>(mArgs)...);
				}
				template<typename T> inline void deinitImpl() noexcept(isNothrowDtor<T>())
				{
					SSVU_ASSERT_STATIC_NM(CTHas<T, TTs...>());
					getImpl<T>().~T();
				}

				template<typename T> inline T& getImpl() & noexcept				{ SSVU_ASSERT_STATIC_NM(CTHas<T, TTs...>()); return castStorage<T>(data); }
				template<typename T> inline const T& getImpl() const& noexcept	{ SSVU_ASSERT_STATIC_NM(CTHas<T, TTs...>()); return castStorage<T>(data); }
				template<typename T> inline T getImpl() && noexcept				{ SSVU_ASSERT_STATIC_NM(CTHas<T, TTs...>()); return std::move(castStorage<T>(data)); }
		};
	}

	template<typename... TTs> class UnionVariant : public Internal::UnionVariantBase<TTs...>
	{
		private:
			// If debug mode is enabled, store and check a "clean" storage flag for additional safety and debugging ease
			#if SSVU_DEBUG
				bool clean{true};
				inline void setClean(bool mClean) noexcept { clean = mClean; }
				inline bool isClean() const noexcept { return clean; }
			#else
				inline void setClean(bool) noexcept { }
				inline bool isClean() const noexcept { return true; }
			#endif

		public:
			template<typename T, typename... TArgs> inline void init(TArgs&&... mArgs) noexcept(isNothrowCtor<T, TArgs...>())
			{
				SSVU_ASSERT(isClean()); setClean(false);
				this->template initImpl<T>(fwd<TArgs>(mArgs)...);
			}
			template<typename T> inline void deinit() noexcept(isNothrowDtor<T>())
			{
				SSVU_ASSERT(!isClean()); setClean(true);
				this->template deinitImpl<T>();
			}

			template<typename T> inline T& get() & noexcept				{ SSVU_ASSERT(!isClean()); return this->template getImpl<T>(); }
			template<typename T> inline const T& get() const& noexcept	{ SSVU_ASSERT(!isClean()); return this->template getImpl<T>(); }
			template<typename T> inline T get() && noexcept				{ SSVU_ASSERT(!isClean()); return std::move(this->template getImpl<T>()); }
	};

	template<typename... TTs> class UnionVariantPOD : public Internal::UnionVariantBase<TTs...>
	{
		SSVU_ASSERT_STATIC_NM(Internal::PODChecker<TTs...>());

		public:
			template<typename T, typename... TArgs> inline void init(TArgs&&... mArgs) noexcept(isNothrowCtor<T, TArgs...>())
			{
				this->template initImpl<T>(fwd<TArgs>(mArgs)...);
			}
			template<typename T> inline void deinit() noexcept(isNothrowDtor<T>()) { this->template deinitImpl<T>(); }

			template<typename T> inline T& get() & noexcept				{ return this->template getImpl<T>(); }
			template<typename T> inline const T& get() const& noexcept	{ return this->template getImpl<T>(); }
			template<typename T> inline T get() && noexcept				{ return std::move(this->template getImpl<T>());}
	};
}

#endif

// TODO: docs
