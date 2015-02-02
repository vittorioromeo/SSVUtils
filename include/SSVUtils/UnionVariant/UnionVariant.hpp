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
		/// @brief Base implementation for `UnionVariant` types.
		template<typename... TTs> class UnionVariantBase
		{
			protected:
				/// @brief Storage for the data, using max align and max size of types.
				AlignedStorage<getCTMaxSize<TTs...>(), getCTMaxAlign<TTs...>()> data;

				/// @brief Constructs and sets the internal data to `T`.
				template<typename T, typename... TArgs> inline void initImpl(TArgs&&... mArgs) noexcept(isNothrowCtor<T, TArgs...>())
				{
					SSVU_ASSERT_STATIC_NM(CTHas<T, TTs...>());
					new (&data) T(SSVU_FWD(mArgs)...);
				}

				/// @brief Destructs the internal `T` data.
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

	/// @brief Union variant class that can store one of any `TTs` types at one time.
	/// @details Intended for use with types that require construction and destruction.
	/// In debug mode, checks are performed to make sure the data was correctly constructed/destructed.
	template<typename... TTs> class UnionVariant : public Internal::UnionVariantBase<TTs...>
	{
		private:
			// If debug mode is enabled, store and check a "clean" storage flag for additional safety and debugging ease
			#if defined(SSVU_DEBUG)
				bool clean{true};
				inline void setClean(bool mClean) noexcept { clean = mClean; }
				inline void assertClean() const noexcept { SSVU_ASSERT(clean, "UnionVariant needs to be clean"); }
				inline void assertNotClean() const noexcept { SSVU_ASSERT(!clean, "UnionVariant needs to be dirty"); }
			#else
				inline void setClean(bool) noexcept { }
				inline void assertClean() const noexcept { }
				inline void assertNotClean() const noexcept { }
			#endif

		public:
			/// @brief Constructs and sets the internal data to `T`.
			/// @details Asserts that any previous data was destroyed.
			template<typename T, typename... TArgs> inline void init(TArgs&&... mArgs) noexcept(isNothrowCtor<T, TArgs...>())
			{
				assertClean(); setClean(false);
				this->template initImpl<T>(SSVU_FWD(mArgs)...);
			}

			/// @brief Destroys the internal `T` data.
			/// @details Asserts that any previous data was constructed.
			template<typename T> inline void deinit() noexcept(isNothrowDtor<T>())
			{
				assertNotClean(); setClean(true);
				this->template deinitImpl<T>();
			}

			// The UnionVariant should be cleaned before being destroyed
			inline ~UnionVariant() noexcept { assertClean(); }

			// Getters
			template<typename T> inline T& get() & noexcept				{ assertNotClean(); return this->template getImpl<T>(); }
			template<typename T> inline const T& get() const& noexcept	{ assertNotClean(); return this->template getImpl<T>(); }
			template<typename T> inline T get() && noexcept				{ assertNotClean(); return std::move(this->template getImpl<T>()); }
	};

	/// @brief Union variant class that can store one of any `TTs` POD types at one time.
	/// @details Intended for use only with POD types. Data cannot be deinitialized.
	/// No checks are performed on construction/destruction of the data.
	template<typename... TTs> class UnionVariantPOD : public Internal::UnionVariantBase<TTs...>
	{
		SSVU_ASSERT_STATIC_NM(Internal::PODChecker<TTs...>());

		public:
			/// @brief Constructs and sets the internal data to `T`.
			template<typename T, typename... TArgs> inline void init(TArgs&&... mArgs) noexcept(isNothrowCtor<T, TArgs...>())
			{
				this->template initImpl<T>(SSVU_FWD(mArgs)...);
			}

			// Getters
			template<typename T> inline T& get() & noexcept				{ return this->template getImpl<T>(); }
			template<typename T> inline const T& get() const& noexcept	{ return this->template getImpl<T>(); }
			template<typename T> inline T get() && noexcept				{ return std::move(this->template getImpl<T>());}
	};
}

#endif

// TODO: rename to something else? Add Experiments/Random/maybe.cpp?
