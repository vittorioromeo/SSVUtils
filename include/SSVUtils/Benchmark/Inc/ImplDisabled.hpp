// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_BENCHMARK_INTERNAL_IMPL_DISABLED
#define SSVU_IMPL_BENCHMARK_INTERNAL_IMPL_DISABLED

#include "SSVUtils/Benchmark/Inc/Data.hpp"

#include <string>

namespace ssvu
{
namespace Benchmark
{
namespace Impl
{
void start(std::string);
Data getEndData();
void endLo();
void groupReset(const std::string&);
void groupResume(const std::string&);
void groupPause(const std::string&);
void groupEndLo(const std::string&);
} // namespace Impl
} // namespace Benchmark
} // namespace ssvu

#endif
