// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"


#include "./utils/test_utils.hpp"
int main()
{
    using namespace std;
    using namespace ssvu;

    SSVU_ASSERT_STATIC_NM(isSame<TplRepeat<int, 0>, tuple<>>());
    SSVU_ASSERT_STATIC_NM(isSame<TplRepeat<int, 1>, tuple<int>>());
    SSVU_ASSERT_STATIC_NM(isSame<TplRepeat<int, 2>, tuple<int, int>>());
    SSVU_ASSERT_STATIC_NM(isSame<TplRepeat<int, 3>, tuple<int, int, int>>());

    TEST_ASSERT_OP((getTplIdxOf<int, TplRepeat<int, 1>>()), ==, 0);
    TEST_ASSERT_OP((getTplIdxOf<int, TplRepeat<int, 10>>()), ==, 0);

    // Will not compile:
    // TEST_ASSERT_OP((getTplIdxOf<int, TplRepeat<float, 0>>()), ==, 0);

    TEST_ASSERT_OP(
        (getTplIdxOf<int, tuple<float, double, int, float, int>>()), ==, 2);

    auto lbd0 = [](int a, int b)
    {
        return a + b;
    };
    auto lbd1 = [](float a, int b)
    {
        return a + b;
    };

    tuple<int, int> t0{1, 3};
    tuple<float, int> t1{1.5f, 3};

    TEST_ASSERT_OP((explode(lbd0, t0)), ==, 4);
    TEST_ASSERT_OP((explode(lbd1, t1)), ==, (1.5f + 3));
    TEST_ASSERT_OP((explode(lbd1, t0)), ==, 4);
    TEST_ASSERT_OP((explode(lbd0, t1)), ==, (ssvu::toInt(1.5f) + 3));

    double acc{0};
    tplFor(
        [&acc](auto x)
        {
            acc += x;
        },
        tuple<int, float, int, double>(1, 2.f, 3, 4.));

    TEST_ASSERT_OP(acc, ==, 1 + 2.f + 3 + 4.);

    acc = 0;
    tplFor(
        [&acc](auto x, auto y)
        {
            acc += x;
            acc += y;
        },
        tuple<int, float, int, double>(1, 2.f, 3, 4.), tuple<int, int>(1, 2));

    TEST_ASSERT_OP(acc, ==, 1 + 2.f + 1 + 2);

    acc = 0;
    tplForData(
        [&acc](auto data, auto x, auto y)
        {
            acc += x;
            acc += y;
            acc += data.getIdx();
        },
        tuple<int, float, int, double>(1, 2.f, 3, 4.), tuple<int, int>(1, 2));

    TEST_ASSERT_OP(acc, ==, 1 + 2.f + 1 + 2 + 0 + 1);

#if defined(__GXX_RTTI)
    {
        ssvu::Tpl<int, char, float> tt1{1, 'c', 3.4f};
        ssvu::Tpl<double, std::string> tt2{10.55, "banana"};

        ssvu::tplForData(
            [&](auto mD, const auto& mA, const auto& mB)
            {
                using ts = typename decltype(mD)::Types;

                TEST_ASSERT((typeid(typename ts::template At<0>)) ==
                            (typeid(ssvu::TplElem<mD.getIdx(),
                                ssvu::Tpl<int, char, float>>)));
                TEST_ASSERT((typeid(typename ts::template At<1>)) ==
                            (typeid(ssvu::TplElem<mD.getIdx(),
                                ssvu::Tpl<double, std::string>>)));
                TEST_ASSERT(
                    (typeid(typename ts::template At<0>)) == (typeid(mA)));
                TEST_ASSERT(
                    (typeid(typename ts::template At<1>)) == (typeid(mB)));
            },
            tt1, tt2);
    }
#endif
}
