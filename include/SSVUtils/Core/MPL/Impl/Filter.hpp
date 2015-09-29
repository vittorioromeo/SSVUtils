// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_MPL_IMPL_FILTER
#define SSVU_CORE_MPL_IMPL_FILTER

#include "SSVUtils/Core/MPL/Impl/BaseTypes.hpp"

namespace ssvu
{
    namespace MPL
    {
        namespace Impl
        {
            template <template <typename> class, typename, typename TResult>
            struct FilterHlpr
            {
                using Type = TResult;
            };
            template <template <typename> class TFilter, typename T,
                      typename... T1s, typename TResult>
            struct FilterHlpr<TFilter, List<T, T1s...>, TResult>
            {
                using CurrentList =
                    typename FilterHlpr<TFilter, List<T1s...>, TResult>::Type;
                using AddedList = typename FilterHlpr<
                    TFilter, List<T1s...>,
                    typename TResult::template PushBack<T>>::Type;

                using Type =
                    Conditional<!TFilter<T>{}(), CurrentList, AddedList>;
            };

            template <template <typename> class TFilter, typename... Ts>
            using Filter =
                typename FilterHlpr<TFilter, List<Ts...>, List<>>::Type;



            template <typename, SizeT, typename, typename TResult>
            struct FilterIdxHlpr
            {
                using Type = TResult;
            };
            template <typename TFilter, SizeT TI, typename T, typename... T1s,
                      typename TResult>
            struct FilterIdxHlpr<TFilter, TI, List<T, T1s...>, TResult>
            {
                using CurrentList =
                    typename FilterIdxHlpr<TFilter, TI + 1, List<T1s...>,
                                           TResult>::Type;
                using AddedList = typename FilterIdxHlpr<
                    TFilter, TI + 1, List<T1s...>,
                    typename TResult::template PushBack<T>>::Type;

                using Type =
                    Conditional<!TFilter{}(TI), CurrentList, AddedList>;
            };

            template <typename TFilter, typename... Ts>
            using FilterIdx =
                typename FilterIdxHlpr<TFilter, 0, List<Ts...>, List<>>::Type;
        }
    }
}

#endif
