// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_REVERSE
#define SSVU_CORE_MPL_IMPL_REVERSE

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

namespace ssvu
{
    namespace MPL
    {
        namespace Impl
        {
            template <typename, typename>
            struct ReverseHlpr;
            template <typename T, typename... Ts, typename TR>
            struct ReverseHlpr<List<T, Ts...>, TR>
            {
                using Type = typename ReverseHlpr<
                    List<Ts...>, typename TR::template PushFront<T>>::Type;
            };
            template <typename TR>
            struct ReverseHlpr<List<>, TR>
            {
                using Type = TR;
            };

            template <typename... Ts>
            using Reverse = typename ReverseHlpr<List<Ts...>, List<>>::Type;
        }
    }
}

#endif
