// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_PREPEND
#define SSVU_CORE_MPL_IMPL_PREPEND

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

namespace ssvu
{
	namespace MPL
	{
		namespace Impl
		{
			template<typename, typename> struct PrependHlpr;
			template<typename... Ts1, typename... Ts2> struct PrependHlpr<List<Ts1...>, List<Ts2...>>
			{
				using Type = List<Ts2..., Ts1...>;
			};
		}
	}
}

#endif
