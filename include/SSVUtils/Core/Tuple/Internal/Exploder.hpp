// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_TUPLE_INTERNAL_EXPLODER
#define SSVU_CORE_TUPLE_INTERNAL_EXPLODER

#include <tuple>
#include "SSVUtils/Core/Common/Common.hpp"

namespace ssvu
{
	namespace Internal
	{
		template<SizeT TN> struct Exploder
		{
			template<typename TF, typename T, typename... TArgs> inline static auto explode(TF&& mF, T&& mT, TArgs&&... mArgs) noexcept
			{
				return Exploder<TN - 1>::explode(fwd<TF>(mF), fwd<T>(mT), ::std::get<TN - 1>(fwd<T>(mT)), fwd<TArgs>(mArgs)...);
			}
		};

		template<> struct Exploder<0>
		{
			template<typename TF, typename T, typename... TArgs> inline static auto explode(TF&& mF, T&&, TArgs&&... mArgs)
				noexcept(noexcept(fwd<TF>(mF)(fwd<TArgs>(mArgs)...)))
			{
				return fwd<TF>(mF)(fwd<TArgs>(mArgs)...);
			}
		};
	}
}

#endif
