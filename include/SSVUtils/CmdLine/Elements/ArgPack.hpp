// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_ELEMENTS_ARGPACK
#define SSVU_CMDLINE_ELEMENTS_ARGPACK

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/CmdLine/Elements/Arg.hpp"
#include "SSVUtils/CmdLine/Internal/Internal.hpp"

namespace ssvu
{
namespace CmdLine
{
template <typename T>
class ArgPack final : public Impl::BaseArgPack,
                      public Impl::ETypeInfo<EType::ArgPack>
{
private:
    std::vector<T> values;

public:
    inline ArgPack() = default;
    inline ArgPack(std::size_t mMin, std::size_t mMax) noexcept
        : BaseArgPack{mMin, mMax}
    {
    }

    inline void set(const std::vector<std::string>& mValues) override
    {
        for(const auto& v : mValues)
            values.emplace_back(Impl::Parser<T>::parse(v));
    }

    inline auto begin() noexcept
    {
        return values.begin();
    }
    inline auto end() noexcept
    {
        return values.end();
    }
    inline auto begin() const noexcept
    {
        return values.begin();
    }
    inline auto end() const noexcept
    {
        return values.end();
    }
    inline auto cbegin() const noexcept
    {
        return values.cbegin();
    }
    inline auto cend() const noexcept
    {
        return values.cend();
    }
};
} // namespace CmdLine
} // namespace ssvu

#endif
