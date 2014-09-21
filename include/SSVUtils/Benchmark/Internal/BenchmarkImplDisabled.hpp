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
			inline void groupReset(const std::string&)	{ }
			inline void groupResume(const std::string&)	{ }
			inline void groupPause(const std::string&)	{ }
			inline void groupEndLo(const std::string&)	{ }
		}
	}
}

#endif
