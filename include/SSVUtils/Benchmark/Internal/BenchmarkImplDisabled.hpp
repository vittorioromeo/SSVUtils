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
			inline auto getEndData()					{ return Internal::Data{}; }
			inline void endLo()							{ }
			inline void resetGroup(const std::string&)	{ }
			inline void resumeGroup(const std::string&)	{ }
			inline void pauseGroup(const std::string&)	{ }
			inline void endLoGroup(const std::string&)	{ }
		}
	}
}

#endif
