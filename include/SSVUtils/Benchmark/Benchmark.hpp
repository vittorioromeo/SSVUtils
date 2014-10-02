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

			/// @brief CRTP struct for data classes' shared methods.
			template<typename TDerived> struct DataBase
			{
				/// @brief Returns the elapsed time as a string.
				inline std::string getString() const { return toStr(ssvu::castUp<TDerived>(this)->getDuration().count()) + " ms"; }
			};

			/// @brief Benchmark data structure storing a time point and a name.
			struct Data : public DataBase<Data>
			{
				TP tp;
				std::string name;

				inline Data() = default;
				inline Data(const TP& mTP, std::string mName) : tp{mTP}, name{std::move(mName)} { }

				/// @brief Returns the elapsed time as a std::chrono::milliseconds.
				inline Duration getDuration() const noexcept { return std::chrono::duration_cast<Duration>(HRClock::now() - tp); }
			};

			/// @brief Benchmark group data structure storing a time point and a duration.
			struct DataGroup : public DataBase<DataGroup>
			{
				TP tp;
				Duration duration;

				inline void reset() noexcept { tp = HRClock::now(); duration = Duration{0}; }
				inline void resume() noexcept { tp = HRClock::now(); }
				inline void pause() noexcept { duration += std::chrono::duration_cast<Duration>(HRClock::now() - tp); }

				/// @brief Returns the elapsed time as a std::chrono::milliseconds.
				inline Duration getDuration() const noexcept { return duration; }
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

		/// @brief Ends the current benchmark timer and logs the elapsed time.
		inline void endLo() { Internal::endLo(); }

		/// @brief Resets the accumulated time of the `mGroup` benchmark group.
		inline void groupReset(const std::string& mGroup) { Internal::groupReset(mGroup); }

		/// @brief Resumes accumulating time in the `mGroup` benchmark group.
		inline void groupResume(const std::string& mGroup) { Internal::groupResume(mGroup); }

		/// @brief Pauses accumulating time in the `mGroup` benchmark group.
		inline void groupPause(const std::string& mGroup) { Internal::groupPause(mGroup); }

		/// @brief Stops and logs on `ssvu::lo()` the accumulated time of the `mGroup` benchmark group.
		inline void groupEndLo(const std::string& mGroup) { Internal::groupEndLo(mGroup); }

		namespace Internal
		{
			/// @brief RAII struct used for LOG_SCOPE_EXIT benchmarks.
			struct LogScopeExit
			{
				inline LogScopeExit(std::string mTitle = "") { start(std::move(mTitle)); }
				inline ~LogScopeExit() { endLo(); }
			};

			/// @brief RAII struct used for RUN_GROUP_SCOPE_EXIT benchmarks.
			struct RunGroupScopeExit
			{
				std::string group;
				inline RunGroupScopeExit(std::string mGroup) : group{std::move(mGroup)} { groupResume(group); }
				inline ~RunGroupScopeExit() { groupPause(group); }
			};
		}
	}
}

/// @macro Instantiates a `LogScopeExit` temp-named object in the current scope.
/// @details The istantiated object will run a benchmark during its lifetime.
/// Pass the desired benchmark title as a parameter.
#define SSVU_BENCHMARK_LOG_SCOPE_EXIT(...) \
	::ssvu::Benchmark::Internal::LogScopeExit SSVPP_CAT(__logScopeExit, __LINE__){__VA_ARGS__}

/// @macro Instantiates a `RunGroupScopeExit` temp-named object in the current scope.
/// @details The istantiated object will resume and pause a group benchmark during its lifetime.
/// Pass the desired benchmark group as a parameter.
#define SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT(...) \
	::ssvu::Benchmark::Internal::RunGroupScopeExit SSVPP_CAT(__logGroupScopeExit, __LINE__){__VA_ARGS__}

#endif
