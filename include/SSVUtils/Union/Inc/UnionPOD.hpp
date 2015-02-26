// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_UNIONPOD
#define SSVU_IMPL_UNIONPOD

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Union/Inc/UnionBase.hpp"
#include "SSVUtils/Union/Inc/Union.hpp"

namespace ssvu
{
	/// @brief Union variant class that can store one of any `Ts` POD types at one time.
	/// @details Intended for use only with POD types. Data cannot be deinitialized.
	/// No checks are performed on construction/destruction of the data.
	template<typename... Ts> class UnionPOD : public Impl::UnionBase<Ts...>
	{
		SSVU_ASSERT_STATIC(MPL::all<IsPod, Ts...>(), "All types must be POD");

		public:
			/// @brief Constructs and sets the internal data to `T`.
			template<typename T, typename... TArgs> inline void init(TArgs&&... mArgs) noexcept(isNothrowCtor<T, TArgs...>())
			{
				this->template initImpl<T>(FWD(mArgs)...);
			}

			// Getters
			template<typename T> inline T& get() & noexcept				{ return this->template getImpl<T>(); }
			template<typename T> inline const T& get() const& noexcept	{ return this->template getImpl<T>(); }
			template<typename T> inline T get() && noexcept				{ return move(this->template getImpl<T>());}
	};
}

#endif
