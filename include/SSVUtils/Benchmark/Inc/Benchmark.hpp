// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_BENCHMARK
#define SSVU_IMPL_BENCHMARK

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Benchmark/Inc/Macros.hpp"
#include "SSVUtils/Benchmark/Inc/Typedefs.hpp"
#include "SSVUtils/Benchmark/Inc/DataBase.hpp"
#include "SSVUtils/Benchmark/Inc/Data.hpp"
#include "SSVUtils/Benchmark/Inc/DataGroup.hpp"

namespace ssvu
{
namespace Benchmark
{
    /// @brief Starts the benchmark timer.
    void start(std::string mTitle = "");

    /// @brief Ends the current benchmark timer and returns the data.
    /// @return Returns the elapsed time as a std::chrono::milliseconds.
    Impl::Data getEndData();

    /// @brief Ends the current benchmark timer and logs the elapsed time.
    void endLo();

    /// @brief Resets the accumulated time of the `mGroup` benchmark group.
    void groupReset(const std::string& mGroup);

    /// @brief Resumes accumulating time in the `mGroup` benchmark group.
    void groupResume(const std::string& mGroup);

    /// @brief Pauses accumulating time in the `mGroup` benchmark group.
    void groupPause(const std::string& mGroup);

    /// @brief Stops and logs on `ssvu::lo()` the accumulated time of the
    /// `mGroup` benchmark group.
    void groupEndLo(const std::string& mGroup);

    namespace Impl
    {
        /// @brief RAII struct used for `SSVU_BENCHMARK_LOG_SCOPE_EXIT`
        /// benchmarks.
        struct LogScopeExit
        {
            inline LogScopeExit(std::string mTitle = "") { start(mv(mTitle)); }
            inline ~LogScopeExit() { endLo(); }
        };

        /// @brief RAII struct used for
        /// `SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT` benchmarks.
        struct RunGroupScopeExit
        {
            std::string group;
            inline RunGroupScopeExit(std::string mGroup) : group{mv(mGroup)}
            {
                groupResume(group);
            }
            inline ~RunGroupScopeExit() { groupPause(group); }
        };

        /// @brief RAII struct used for
        /// `SSVU_BENCHMARK_INIT_GROUP_SCOPE_EXIT` benchmarks.
        struct InitGroupScopeExit
        {
            std::string group;
            inline InitGroupScopeExit(std::string mGroup) : group{mv(mGroup)}
            {
                groupReset(group);
            }
            inline ~InitGroupScopeExit() { groupEndLo(group); }
        };
    }
}
}

#endif
