// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Benchmark/Inc/Benchmark.hpp"
#include "SSVUtils/Benchmark/Inc/Data.hpp"

#include <string>
#include <chrono>

namespace ssvu
{
namespace Benchmark
{
namespace Impl
{
SSVU_INLINE Data::Data(const TP& mTP, std::string mName)
    : tp{mTP}, name{std::move(mName)}
{
}

SSVU_INLINE Duration Data::getDuration() const noexcept
{
    return std::chrono::duration_cast<Duration>(
        std::chrono::high_resolution_clock::now() - tp);
}
} // namespace Impl
} // namespace Benchmark
} // namespace ssvu
