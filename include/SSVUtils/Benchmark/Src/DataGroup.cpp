// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Benchmark/Inc/DataGroup.hpp"

#include <chrono>
namespace ssvu
{
namespace Benchmark
{
namespace Impl
{
SSVU_INLINE void DataGroup::reset() noexcept
{
    tp = std::chrono::high_resolution_clock::now();
    duration = Duration{0};
}

SSVU_INLINE void DataGroup::resume() noexcept
{
    tp = std::chrono::high_resolution_clock::now();
}

SSVU_INLINE void DataGroup::pause() noexcept
{
    duration += std::chrono::duration_cast<Duration>(
        std::chrono::high_resolution_clock::now() - tp);
}

SSVU_INLINE Duration DataGroup::getDuration() const noexcept
{
    return duration;
}
} // namespace Impl
} // namespace Benchmark
} // namespace ssvu
