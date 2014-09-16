// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_BENCHMARK
#define SSVU_BENCHMARK

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

			/// @brief Benchmark data structure storing a time point and a name.
			struct Data
			{
				TP tp;
				std::string name;

				inline Data() = default;
				inline Data(const TP& mTP, std::string mName) : tp{mTP}, name{std::move(mName)} { }
			};
		}
	}
}

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
		inline void start(std::string mTitle = "") { return Internal::start(std::move(mTitle)); }

		/// @brief Ends the current benchmark timer and returns the data.
		/// @return Returns the elapsed time as a std::chrono::milliseconds.
		inline auto getEndData() { return Internal::getEndData(); }

		/// @brief Ends the current benchmark timer and returns the elapsed time.
		/// @return Returns the elapsed time as a std::chrono::milliseconds.
		inline auto getEndDuration() { return Internal::getEndDuration(); }

		/// @brief Ends the current benchmark timer and returns the elapsed time.
		/// @return Returns the elapsed time as a string.
		inline auto getEndString() { return Internal::getEndString(); }

		/// @brief Ends the current benchmark timer and logs the elapsed time.
		inline void endLo() { Internal::endLo(); }

		namespace Internal
		{
			struct ScopeLogBenchmark
			{
				inline ScopeLogBenchmark(std::string mTitle = "") { start(std::move(mTitle)); }
				inline ~ScopeLogBenchmark() { endLo(); }
			};
		}
	}
}

/// @macro Instantiates a ScopeLogBenchmark temp-named object in the current scope.
/// @details The istantiated object will run a benchmark during its lifetime.
/// Pass the desired benchmark title as a parameter.
#define SSVU_BENCHMARK_SCOPELOGBENCHMARK(...) \
	::ssvu::Benchmark::Internal::ScopeLogBenchmark SSVPP_CAT(__scopeLogBenchmark, __LINE__){__VA_ARGS__}

#endif

// TODO: consider renaming scopelogbenchmark?
// TODO: grouped benchmarks (like separate code segments, accumulate the time, then display at the end)
// TODO: indentation options, display options (check map example
