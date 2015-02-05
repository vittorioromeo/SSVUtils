// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_UNION
#define SSVU_IMPL_UNION

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Union/Inc/UnionBase.hpp"

namespace ssvu
{
	/// @brief Union variant class that can store one of any `TTs` types at one time.
	/// @details Intended for use with types that require construction and destruction.
	/// In debug mode, checks are performed to make sure the data was correctly constructed/destructed.
	template<typename... TTs> class Union : public Impl::UnionBase<TTs...>
	{
		private:
			// If debug mode is enabled, store and check a "clean" storage flag for additional safety and debugging ease
			#if defined(SSVU_DEBUG)
				bool clean{true};
				inline void setClean(bool mClean) noexcept { clean = mClean; }
				inline void assertClean() const noexcept { SSVU_ASSERT(clean, "Union needs to be clean"); }
				inline void assertNotClean() const noexcept { SSVU_ASSERT(!clean, "Union needs to be dirty"); }
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
				this->template initImpl<T>(FWD(mArgs)...);
			}

			/// @brief Destroys the internal `T` data.
			/// @details Asserts that any previous data was constructed.
			template<typename T> inline void deinit() noexcept(isNothrowDtor<T>())
			{
				assertNotClean(); setClean(true);
				this->template deinitImpl<T>();
			}

			// The Union should be cleaned before being destroyed
			inline ~Union() noexcept { assertClean(); }

			// Getters
			template<typename T> inline T& get() & noexcept				{ assertNotClean(); return this->template getImpl<T>(); }
			template<typename T> inline const T& get() const& noexcept	{ assertNotClean(); return this->template getImpl<T>(); }
			template<typename T> inline T get() && noexcept				{ assertNotClean(); return std::move(this->template getImpl<T>()); }
	};
}

#endif

// TODO: assert correct type is being used?
