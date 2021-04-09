// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"

#include "./utils/test_utils.hpp"

#include <string>
#include <vector>

TEST_MAIN()
{
    using namespace std;
    using namespace ssvu;
    using namespace ssvu::MPL;

    struct PT0
    {
    };
    struct PT1
    {
    };
    struct PT2
    {
    };
    struct PT3
    {
    };
    struct PT4
    {
    };

    static_assert(getMin<int>(1, 5, 100, -20, -5, -100) == -100);
    static_assert(getMin<int>(0, 1) == 0);
    static_assert(getMin<int>(1) == 1);

    static_assert(getMax<int>(1, 5, 100, -20, -5, -100) == 100);
    static_assert(getMax<int>(0, 1) == 1);
    static_assert(getMax<int>(1) == 1);

    static_assert(
        std::is_same_v<MPL::Impl::VAHead<PT0, PT1, PT2>, PT0>);
    static_assert(
        std::is_same_v<MPL::Impl::VATail<PT0, PT1, PT2>, PT2>);

    static_assert(std::is_same_v<List<PT0, PT1, PT2>::Head, PT0>);
    static_assert(std::is_same_v<List<PT0, PT1, PT2>::Tail, PT2>);

    static_assert(std::is_same_v<List<>::AsTpl, std::tuple<>>);
    static_assert(std::is_same_v<List<PT0>::AsTpl, std::tuple<PT0>>);
    static_assert(
        std::is_same_v<List<PT0, PT1>::AsTpl, std::tuple<PT0, PT1>>);
    static_assert(
        std::is_same_v<List<PT0, PT1, PT0>::AsTpl, std::tuple<PT0, PT1, PT0>>);

    static_assert(std::is_same_v<List<PT0, PT1, PT2>::At<0>, PT0>);
    static_assert(std::is_same_v<List<PT0, PT1, PT2>::At<1>, PT1>);
    static_assert(std::is_same_v<List<PT0, PT1, PT2>::At<2>, PT2>);

    static_assert(std::is_same_v<List<>::PushBack<PT0>, List<PT0>>);
    static_assert(
        std::is_same_v<List<PT0>::PushBack<PT1>, List<PT0, PT1>>);
    static_assert(
        std::is_same_v<List<PT0, PT1>::PushBack<PT2>, List<PT0, PT1, PT2>>);

    static_assert(std::is_same_v<List<>::PushFront<PT0>, List<PT0>>);
    static_assert(
        std::is_same_v<List<PT0>::PushFront<PT1>, List<PT1, PT0>>);
    static_assert(
        std::is_same_v<List<PT1, PT0>::PushFront<PT2>, List<PT2, PT1, PT0>>);

    static_assert(
        std::is_same_v<List<PT0, PT1, PT2>::PopBack, List<PT0, PT1>>);
    static_assert(std::is_same_v<List<PT0, PT1>::PopBack, List<PT0>>);
    static_assert(std::is_same_v<List<PT0>::PopBack, List<>>);

    static_assert(
        std::is_same_v<List<PT0, PT1, PT2>::PopFront, List<PT1, PT2>>);
    static_assert(std::is_same_v<List<PT1, PT2>::PopFront, List<PT2>>);
    static_assert(std::is_same_v<List<PT2>::PopFront, List<>>);

    static_assert(
        std::is_same_v<List<PT0, PT1, PT2, PT1, PT0>::Slice<0, 0>, List<>>);
    static_assert(
        std::is_same_v<List<PT0, PT1, PT2, PT1, PT0>::Slice<0, 1>, List<PT0>>);
    static_assert(
        std::is_same<List<PT0, PT1, PT2, PT1, PT0>::Slice<0, 3>,
            List<PT0, PT1, PT2>>());
    static_assert(
        std::is_same_v<List<PT0, PT1, PT2, PT1, PT0>::Slice<2, 4>,
            List<PT2, PT1>>);

    // Will not compile
    // static_assert(std::is_same<List<PT0, PT1, PT2, PT1,
    // PT0>::Slice<0, 999>, List<PT0, PT1, PT2, PT1, PT0>>());

    static_assert(List<PT0, PT1, PT2, PT1, PT0>::has<PT0>());
    static_assert(List<PT0, PT1, PT2, PT1, PT0>::has<PT1>());
    static_assert(List<PT0, PT1, PT2, PT1, PT0>::has<PT2>());
    static_assert(!List<PT0, PT1, PT2, PT1, PT0>::has<PT3>());
    static_assert(!List<PT0, PT1, PT2, PT1, PT0>::has<PT4>());
    static_assert(!List<>::has<PT0>());
    static_assert(!List<>::has<PT1>());

    static_assert(std::is_same<List<PT0, PT1>::Append<List<PT0, PT1>>,
        List<PT0, PT1, PT0, PT1>>());
    static_assert(
        std::is_same_v<List<PT0, PT1>::Append<List<>>, List<PT0, PT1>>);
    static_assert(
        std::is_same_v<List<>::Append<List<PT0, PT1>>, List<PT0, PT1>>);

    static_assert(std::is_same<List<PT0, PT1>::Prepend<List<PT2, PT1>>,
        List<PT2, PT1, PT0, PT1>>());
    static_assert(
        std::is_same_v<List<PT0, PT1>::Prepend<List<>>, List<PT0, PT1>>);
    static_assert(
        std::is_same_v<List<>::Prepend<List<PT0, PT1>>, List<PT0, PT1>>);

    static_assert(
        List<PT0, PT1, PT2, PT1, PT0>::getCountOf<PT0>() == 2);
    static_assert(
        List<PT0, PT1, PT2, PT1, PT0>::getCountOf<PT1>() == 2);
    static_assert(
        List<PT0, PT1, PT2, PT1, PT0>::getCountOf<PT2>() == 1);
    static_assert(
        List<PT0, PT1, PT2, PT1, PT0>::getCountOf<PT3>() == 0);

    static_assert(!List<PT0, PT1, PT2, PT1, PT0>::unique);
    static_assert(!List<PT0, PT0>::unique);
    static_assert(List<PT0, PT1, PT2>::unique);
    static_assert(List<>::unique);

    static_assert(std::is_same_v<List<>::Unique, List<>>);
    static_assert(std::is_same_v<List<PT0>::Unique, List<PT0>>);
    static_assert(std::is_same_v<List<PT0, PT1, PT2, PT1, PT0>::Unique,
        List<PT0, PT1, PT2>>);
    static_assert(
        std::is_same<List<PT0, PT1, PT2, PT1, PT0, PT1, PT2, PT0>::Unique,
            List<PT0, PT1, PT2>>());

    static_assert(
        !std::is_same_v<List<PT2, PT1, PT0>::Unique, List<PT0, PT1, PT2>>);
    static_assert(
        std::is_same_v<List<PT2, PT1, PT0>::Unique, List<PT2, PT1, PT0>>);

    static_assert(
        !std::is_same_v<List<PT0, PT2, PT1, PT0>::Unique, List<PT0, PT1, PT2>>);
    static_assert(
        std::is_same_v<List<PT0, PT2, PT1, PT0>::Unique, List<PT0, PT2, PT1>>);

    static_assert(
        std::is_same_v<List<>::Apply<std::add_lvalue_reference_t>, List<>>);
    static_assert(
        std::is_same_v<List<PT0>::Apply<std::add_lvalue_reference_t>,
            List<PT0&>>);
    static_assert(
        std::is_same_v<List<PT0, PT0>::Apply<std::add_lvalue_reference_t>,
            List<PT0&, PT0&>>);
    static_assert(
        std::is_same_v<List<PT0, PT1, PT2>::Apply<std::add_lvalue_reference_t>,
            List<PT0&, PT1&, PT2&>>);

    static_assert(std::is_same_v<List<>::Filter<std::is_pod>, List<>>);
    static_assert(
        std::is_same<List<int, char, int>::Filter<std::is_pod>,
            List<int, char, int>>());
    static_assert(
        std::is_same<List<int, std::string, char, int,
                         std::vector<int>>::Filter<std::is_pod>,
            List<int, char, int>>());
    static_assert(
        std::is_same<List<int, std::string, char, int, std::vector<int>>::
                         Filter<SSVU_MPL_NEGATE(std::is_pod)>,
            List<std::string, std::vector<int>>>());

    static_assert(std::is_same<List<int, char, float>::Insert<int, 0>,
        List<int, int, char, float>>());

    static_assert(std::is_same<List<int, char, float>::Insert<int, 2>,
        List<int, char, int, float>>());

    static_assert(std::is_same_v<List<>::Insert<int, 0>, List<int>>);

    static_assert(
        std::is_same_v<List<int, char, float>::Remove<0>, List<char, float>>);

    static_assert(
        std::is_same_v<List<int, char, float>::Remove<1>, List<int, float>>);

    static_assert(
        std::is_same_v<List<int, char>::Remove<0>, List<char>>);

    static_assert(std::is_same_v<List<char>::Remove<0>, List<>>);

    static_assert(List<>::isEqualTo<List<>>());

    static_assert(!List<int>::isEqualTo<List<>>());

    static_assert(!List<>::isEqualTo<List<int>>());

    static_assert(List<int>::isEqualTo<List<int>>());

    static_assert(!List<int, char>::isEqualTo<List<int>>());

    static_assert(!List<int>::isEqualTo<List<int, char>>());

    static_assert(List<int, char>::isEqualTo<List<int, char>>());

    static_assert(!List<int, char>::isEqualTo<List<char, int>>());

    static_assert(!std::is_same_v<List<int, char>, List<char, int>>);

    static_assert(
        std::is_same_v<List<int>::IdxsOfSeq<List<>>, ListIC<int>>);

    static_assert(
        std::is_same_v<List<>::IdxsOfSeq<List<int>>, ListIC<int>>);

    static_assert(
        !std::is_same_v<List<>::IdxsOfSeq<List<>>, ListInt<0>>);

    static_assert(
        std::is_same_v<List<int>::IdxsOfSeq<List<int>>, ListInt<0>>);

    static_assert(
        std::is_same_v<List<int, int>::IdxsOfSeq<List<int>>, ListInt<0, 1>>);

    static_assert(
        std::is_same_v<List<int, char, int>::IdxsOfSeq<List<int>>,
            ListInt<0, 2>>);

    static_assert(
        std::is_same_v<List<int, char, int>::IdxsOfSeq<List<int, char>>,
            ListInt<0>>);

    static_assert(
        std::is_same<List<int, char, char>::IdxsOfSeq<List<int, char>>,
            ListInt<0>>());

    static_assert(
        std::is_same<List<int, char, int, char>::IdxsOfSeq<List<int, char>>,
            ListInt<0, 2>>());

    static_assert(std::is_same<
        List<int, char, char, int, char>::IdxsOfSeq<List<int, char>>,
        ListInt<0, 3>>());

    static_assert(
        std::is_same_v<List<int>::ReplaceAllOfSeq<List<int>, List<>>, List<>>);

    static_assert(
        std::is_same<List<int>::ReplaceAllOfSeq<List<int>, List<char>>,
            List<char>>());

    static_assert(
        std::is_same<List<int, float>::ReplaceAllOfSeq<List<int>, List<char>>,
            List<char, float>>());

    static_assert(std::is_same<
        List<int, float, int>::ReplaceAllOfSeq<List<int>, List<char>>,
        List<char, float, char>>());

    static_assert(std::is_same<
        List<int, float>::ReplaceAllOfSeq<List<int, float>, List<char>>,
        List<char>>());

    static_assert(
        std::is_same<List<int, float, int, int, int, float, int, float,
                         float>::ReplaceAllOfSeq<List<int, float>, List<char>>,
            List<char, int, int, char, char, float>>());

    static_assert(
        std::is_same<List<int, float, int, int, int, float, int, float,
                         float>::ReplaceAllOfSeq<List<int, float>, List<>>,
            List<int, int, float>>());

    static_assert(
        std::is_same<List<int, float, int, int, int, float, int, float, float>::
                         ReplaceAllOfSeq<List<int, float>, List<char, double>>,
            List<char, double, int, int, char, double, char, double, float>>());

    static_assert(std::is_same_v<List<>::Reverse, List<>>);

    static_assert(std::is_same_v<List<PT0>::Reverse, List<PT0>>);

    static_assert(
        std::is_same_v<List<PT0, PT1>::Reverse, List<PT1, PT0>>);

    static_assert(
        std::is_same_v<List<PT0, PT1, PT2>::Reverse, List<PT2, PT1, PT0>>);

    struct EvenFn
    {
        inline constexpr auto operator()(std::size_t mI) noexcept
        {
            return mI % 2 == 0;
        }
    };

    struct OddFn
    {
        inline constexpr auto operator()(std::size_t mI) noexcept
        {
            return mI % 2 == 1;
        }
    };

    static_assert(std::is_same<
        List<int, char, float, int, char, float>::FilterIdx<EvenFn>,
        List<int, float, char>>());

    static_assert(
        std::is_same<List<int, char, float, int, char, float>::FilterIdx<OddFn>,
            List<char, int, float>>());

    static_assert(List<int, char, char>::template getIdxOf<int>() == 0);
    static_assert(
        List<int, char, char>::template getIdxOf<char>() == 1);

    static_assert(
        List<double, char, float>::template getIdxOf<double>() == 0);
    static_assert(
        List<double, char, float>::template getIdxOf<float>() == 2);
}
