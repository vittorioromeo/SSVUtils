// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_BENCHMARK_INTERNAL_IMPL_DISABLED
#define SSVU_BENCHMARK_INTERNAL_IMPL_DISABLED

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	namespace Benchmark
	{
		namespace Internal
		{
			using TP = std::chrono::time_point<HRClock>;
			using Duration = std::chrono::milliseconds;

			struct Data
			{
				TP tp;
				std::string name;
			};

			inline void start(std::string)				{ }
			inline Internal::Data getEndData()			{ return Internal::Data{}; }
			inline Internal::Duration getEndDuration()	{ return Internal::Duration{0}; }
			inline std::string getEndString()			{ return ""; }
			inline void endLo()							{ }
		}
	}
}

#endif
