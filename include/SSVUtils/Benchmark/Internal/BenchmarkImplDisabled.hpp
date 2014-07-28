// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_BENCHMARK_INTERNAL_IMPL_DISABLED
#define SSVU_BENCHMARK_INTERNAL_IMPL_DISABLED

namespace ssvu
{
	namespace Benchmark
	{
		namespace Internal
		{
			inline void start(std::string)				{ }
			inline Internal::Data getEndData()			{ return Internal::Data{}; }
			inline Internal::Duration getEndDuration()	{ return Internal::Duration{0}; }
			inline std::string getEndString()			{ return ""; }
			inline void endLo()							{ }
		}
	}
}

#endif
