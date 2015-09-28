// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"

#include "SSVUtils/Benchmark/Benchmark.hpp"

#include "./utils/test_utils.hpp"
int main()
{
    ssvu::Benchmark::start();
    {
        ssvu::Benchmark::start();
        {
        }
        ssvu::Benchmark::getEndData().getString();
    }
    ssvu::Benchmark::getEndData().getDuration();

    ssvu::setLogSuppressed(true);

    {
        SSVU_BENCHMARK_LOG_SCOPE_EXIT();
        SSVU_BENCHMARK_LOG_SCOPE_EXIT("test0");

        {
            SSVU_BENCHMARK_LOG_SCOPE_EXIT();
            SSVU_BENCHMARK_LOG_SCOPE_EXIT("test1");
        }
    }

    {
        ssvu::Benchmark::groupReset("__testGroup1");
        ssvu::Benchmark::groupReset("__testGroup2");
        SSVU_BENCHMARK_INIT_GROUP_SCOPE_EXIT("__testGroup3");

        ssvu::Benchmark::groupResume("__testGroup1");
        ssvu::Benchmark::groupPause("__testGroup1");

        ssvu::Benchmark::groupResume("__testGroup2");
        ssvu::Benchmark::groupPause("__testGroup2");

        ssvu::Benchmark::groupResume("__testGroup3");
        ssvu::Benchmark::groupPause("__testGroup3");

        ssvu::Benchmark::groupEndLo("__testGroup1");
        ssvu::Benchmark::groupEndLo("__testGroup2");
    }

    {
        ssvu::Benchmark::groupReset("__testGroup1");
        ssvu::Benchmark::groupReset("__testGroup2");

        {
            SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("__testGroup1");
        }
        {
            SSVU_BENCHMARK_RUN_GROUP_SCOPE_EXIT("__testGroup2");
        }

        ssvu::Benchmark::groupEndLo("__testGroup1");
        ssvu::Benchmark::groupEndLo("__testGroup2");
    }

    ssvu::setLogSuppressed(false);
}
