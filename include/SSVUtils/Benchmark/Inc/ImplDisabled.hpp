// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_BENCHMARK_INTERNAL_IMPL_DISABLED
#define SSVU_BENCHMARK_INTERNAL_IMPL_DISABLED

#include <string>
#include "SSVUtils/Benchmark/Inc/Data.hpp"

namespace ssvu
{
	namespace Benchmark
	{
		namespace Internal
		{
			void start(std::string);
			Data getEndData();
			void endLo();
			void groupReset(const std::string&);
			void groupResume(const std::string&);
			void groupPause(const std::string&);
			void groupEndLo(const std::string&);
		}
	}
}

#endif
