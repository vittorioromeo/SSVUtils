// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_ELEMENTS_BASES_BASEARGPACK
#define SSVU_CMDLINE_ELEMENTS_BASES_BASEARGPACK

#include "SSVUtils/CmdLine/Elements/Bases/BaseElement.hpp"

#include <string>
#include <vector>

namespace ssvu
{
namespace CmdLine
{
namespace Impl
{
class BaseArgPack : public BaseElement
{
protected:
    std::size_t min, max;

public:
    inline BaseArgPack() noexcept : min{0}, max{0}
    {
    }
    inline BaseArgPack(std::size_t mMin, std::size_t mMax) noexcept
        : min{mMin}, max{mMax}
    {
    }

    virtual void set(const std::vector<std::string>&) = 0;

    inline auto isInfinite() const noexcept
    {
        return min == 0 && max == 0;
    }
    inline auto getMin() const noexcept
    {
        return min;
    }
    inline auto getMax() const noexcept
    {
        return max;
    }

    inline std::string getUsageStr() const override
    {
        return "(PACK " + getName() + " " + "[" + toStr(min) + "/" +
               (isInfinite() ? "..." : toStr(max)) + "])";
    }
};
} // namespace Impl
} // namespace CmdLine
} // namespace ssvu

#endif
