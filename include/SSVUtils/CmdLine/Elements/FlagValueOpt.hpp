// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_ELEMENTS_FLAGVALUEOPT
#define SSVU_CMDLINE_ELEMENTS_FLAGVALUEOPT

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/CmdLine/Internal/Internal.hpp"
#include "SSVUtils/CmdLine/Elements/Bases/Bases.hpp"
#include "SSVUtils/CmdLine/Elements/FlagValue.hpp"

namespace ssvu
{
    namespace CmdLine
    {
        template <typename T>
        class FlagValueOpt final : public Impl::FlagValueImpl<T>,
                                   public Impl::ETypeInfo<EType::FlagValueOpt>
        {
        private:
            T valueDefault;
            bool active{false};

        public:
            inline FlagValueOpt(const std::string& mNameShort,
                                const std::string& mNameLong,
                                const T& mValueDefault) noexcept
                : Impl::FlagValueImpl<T>{mNameShort, mNameLong},
                  valueDefault{mValueDefault}
            {
            }

            inline void set(const std::string& mValue) override
            {
                FlagValue<T>::set(mValue);
                active = true;
            }
            inline auto get() const noexcept
            {
                return active ? FlagValue<T>::get() : valueDefault;
            }

            inline std::string getUsageStr() const override
            {
                return "[OPT " + Impl::BaseFlag::getNameShort() +
                       "=(...) || OPT " + Impl::BaseFlag::getNameLong() +
                       "=(...)]";
            }
        };
    }
}

#endif
