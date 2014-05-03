// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_BENCHMARK
#define SSVU_BENCHMARK

#include "SSVUtils/Core/Core.hpp"

#ifndef SSVU_BENCHMARK_DISABLE
	#include "SSVUtils/Benchmark/Internal/BenchmarkImplEnabled.hpp"
#else
	#include "SSVUtils/Benchmark/Internal/BenchmarkImplDisabled.hpp"
#endif

namespace ssvu
{
	namespace Benchmark
	{
		/// @brief Starts the benchmark timer.
		inline void start(std::string mTitle = "") { return Internal::start(mTitle); }

		/// @brief Ends the current benchmark timer and returns the data.
		/// @return Returns the elapsed time as a std::chrono::milliseconds.
		inline Internal::Data getEndData() { return Internal::getEndData(); }

		/// @brief Ends the current benchmark timer and returns the elapsed time.
		/// @return Returns the elapsed time as a std::chrono::milliseconds.
		inline Internal::Duration getEndDuration() { return Internal::getEndDuration(); }

		/// @brief Ends the current benchmark timer and returns the elapsed time.
		/// @return Returns the elapsed time as a string.
		inline std::string getEndString() { return Internal::getEndString(); }

		/// @brief Ends the current benchmark timer and logs the elapsed time.
		inline void endLo() { Internal::endLo(); }
	}
}

#endif
