// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_NEGATE
#define SSVU_CORE_MPL_IMPL_NEGATE

#include "SSVUtils/Core/Common/Common.hpp"

namespace ssvu
{
    namespace MPL
    {
        namespace Impl
        {
            template <template <typename...> class TP>
            struct Negate
            {
                template <typename T>
                using Type = CTBool<!TP<T>{}()>;
            };
        }
    }
}

#define SSVU_MPL_NEGATE(...) \
    ::ssvu::MPL::Impl::Negate<__VA_ARGS__>::template Type

#endif
