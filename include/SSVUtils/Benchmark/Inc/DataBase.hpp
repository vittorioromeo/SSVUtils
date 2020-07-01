// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_BENCHMARK_DATABASE
#define SSVU_IMPL_BENCHMARK_DATABASE

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
namespace Benchmark
{
namespace Impl
{
/// @brief CRTP struct for data classes' shared methods.
template <typename TDerived>
struct DataBase
{
    /// @brief Returns the elapsed time as a string.
    inline std::string getString() const
    {
        auto value(ssvu::castUp<TDerived>(this)->getDuration().count());
        auto ms(ssvu::toNum<double>(value) / 1000000.0);
        return toStr(ms) + " ms";
    }
};
} // namespace Impl
} // namespace Benchmark
} // namespace ssvu

#endif
