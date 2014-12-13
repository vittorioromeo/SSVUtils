// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_BENCHMARK
#define SSVU_BENCHMARK

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Benchmark/Inc/Benchmark.hpp"

#if defined(SSVU_API_HEADERONLY)
	#ifndef SSVU_BENCHMARK_DISABLE
		#include "SSVUtils/Benchmark/Src/ImplEnabled.cpp"
	#else
		#include "SSVUtils/Benchmark/Src/ImplDisabled.cpp"
	#endif

	#include "SSVUtils/Benchmark/Src/Benchmark.cpp"
	#include "SSVUtils/Benchmark/Src/Data.cpp"
	#include "SSVUtils/Benchmark/Src/DataGroup.cpp"
#endif

#endif
