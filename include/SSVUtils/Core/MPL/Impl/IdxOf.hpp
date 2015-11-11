// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_IDXOF
#define SSVU_CORE_MPL_IMPL_IDXOF

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

namespace ssvu
{
    namespace MPL
    {
        namespace Impl
        {
            template <typename, typename...>
            struct IdxOf;

            // IndexOf base case: found the type we're looking for.
            template <typename T, typename... Ts>
            struct IdxOf<T, T, Ts...> : std::integral_constant<std::size_t, 0>
            {
            };

            // IndexOf recursive case: 1 + IndexOf the rest of the types.
            template <typename T, typename TOther, typename... Ts>
            struct IdxOf<T, TOther, Ts...>
                : std::integral_constant<std::size_t, 1 + IdxOf<T, Ts...>{}>
            {
            };
        }
    }
}

#endif
