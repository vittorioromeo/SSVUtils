// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_ELEMENTS_ARGOPT
#define SSVU_CMDLINE_ELEMENTS_ARGOPT

#include "SSVUtils/CmdLine/Elements/Arg.hpp"
#include "SSVUtils/CmdLine/Internal/Internal.hpp"

#include <string>

namespace ssvu
{
namespace CmdLine
{
template <typename T>
class ArgOpt final : public Impl::ArgImpl<T>,
                     public Impl::ETypeInfo<EType::ArgOpt>
{
private:
    T valueDefault;
    bool active{false};

public:
    SSVU_DEFINE_SINK_CTOR_SIMPLE_1(ArgOpt, valueDefault)

    inline operator bool() const noexcept
    {
        return active;
    }

    inline void set(const std::string& mValue) override
    {
        Impl::ArgImpl<T>::set(mValue);
        active = true;
    }
    inline auto get() const
    {
        return active ? Impl::ArgImpl<T>::get() : valueDefault;
    }

    inline std::string getUsageStr() const override
    {
        return "(ARGOPT " + Impl::BaseElement::getName() + ")";
    }
};
} // namespace CmdLine
} // namespace ssvu

#endif
