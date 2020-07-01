// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_ELEMENTS_FLAGVALUE
#define SSVU_CMDLINE_ELEMENTS_FLAGVALUE

#include "SSVUtils/CmdLine/Elements/Bases/Bases.hpp"
#include "SSVUtils/CmdLine/Internal/Internal.hpp"

#include <string>

namespace ssvu
{
namespace CmdLine
{
namespace Impl
{
template <typename T>
class FlagValueImpl : public BaseFlagValue
{
private:
    T value;

public:
    inline FlagValueImpl(
        const std::string& mNameShort, const std::string& mNameLong) noexcept
        : BaseFlagValue{mNameShort, mNameLong}
    {
    }

    inline void set(const std::string& mValue) override
    {
        value = Parser<T>::parse(mValue);
    }
    inline auto get() const noexcept
    {
        return value;
    }

    inline std::string getUsageStr() const override
    {
        return "[" + getNameShort() + "=(...) || " + getNameLong() + "=(...)]";
    }
};
} // namespace Impl

template <typename T>
class FlagValue final : public Impl::FlagValueImpl<T>,
                        public Impl::ETypeInfo<EType::FlagValue>
{
public:
    inline FlagValue(
        const std::string& mNameShort, const std::string& mNameLong) noexcept
        : Impl::FlagValueImpl<T>{mNameShort, mNameLong}
    {
    }
};
} // namespace CmdLine
} // namespace ssvu

#endif
