// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CMDLINE_INTERNAL_ETYPE
#define SSVU_CMDLINE_INTERNAL_ETYPE

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
namespace CmdLine
{
    enum class EType : SizeT
    {
        Arg,
        ArgOpt,
        ArgPack,
        Flag,
        FlagValue,
        FlagValueOpt
    };

    namespace Impl
    {
        template <EType TET>
        struct ETypeInfo
        {
            inline static constexpr EType getEType() noexcept { return TET; }
        };
    }
}
}

#endif
