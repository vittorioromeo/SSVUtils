// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_BENCHMARK_TYPEDEFS
#define SSVU_IMPL_BENCHMARK_TYPEDEFS

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	namespace Benchmark
	{
		namespace Internal
		{
			/// @typedef Shortcut typedef for `std::chrono::timepoint<HRClock>`.
			using TP = std::chrono::time_point<HRClock>;

			/// @typedef Shortcut typedef for `std::chrono::milliseconds`.
			using Duration = std::chrono::milliseconds;
		}
	}
}

#endif
