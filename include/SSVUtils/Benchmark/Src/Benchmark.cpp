// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Benchmark/Inc/Benchmark.hpp"
#include "SSVUtils/Benchmark/Inc/Data.hpp"

#ifndef SSVU_BENCHMARK_DISABLE
#include "SSVUtils/Benchmark/Inc/ImplEnabled.hpp"
#else
#include "SSVUtils/Benchmark/Inc/ImplDisabled.hpp"
#endif

namespace ssvu
{
    namespace Benchmark
    {
        SSVU_INLINE void start(std::string mTitle)
        {
            return Impl::start(mv(mTitle));
        }

        SSVU_INLINE Impl::Data getEndData() { return Impl::getEndData(); }

        SSVU_INLINE void endLo() { Impl::endLo(); }

        SSVU_INLINE void groupReset(const std::string& mGroup)
        {
            Impl::groupReset(mGroup);
        }
        SSVU_INLINE void groupResume(const std::string& mGroup)
        {
            Impl::groupResume(mGroup);
        }
        SSVU_INLINE void groupPause(const std::string& mGroup)
        {
            Impl::groupPause(mGroup);
        }
        SSVU_INLINE void groupEndLo(const std::string& mGroup)
        {
            Impl::groupEndLo(mGroup);
        }
    }
}
