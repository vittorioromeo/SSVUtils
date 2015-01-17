// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSUTILSTUPLE
#define SSVU_TESTS_TESTSUTILSTUPLE

#include "SSVUtils/Core/Core.hpp"

SSVUT_TEST(TestsUtilsTuple)
{
	using namespace std;
	using namespace ssvu;

	SSVU_ASSERT_STATIC_NM(isSame<TplRepeat<int, 0>, tuple<>>());
	SSVU_ASSERT_STATIC_NM(isSame<TplRepeat<int, 1>, tuple<int>>());
	SSVU_ASSERT_STATIC_NM(isSame<TplRepeat<int, 2>, tuple<int, int>>());
	SSVU_ASSERT_STATIC_NM(isSame<TplRepeat<int, 3>, tuple<int, int, int>>());

	SSVUT_EXPECT_OP((getTplIdxOf<int, TplRepeat<int, 1>>()), ==, 0);
	SSVUT_EXPECT_OP((getTplIdxOf<int, TplRepeat<int, 10>>()), ==, 0);

	// Will not compile:
	// SSVUT_EXPECT_OP((getTplIdxOf<int, TplRepeat<float, 0>>()), ==, 0);

	SSVUT_EXPECT_OP((getTplIdxOf<int, tuple<float, double, int, float, int>>()), ==, 2);

	auto lbd0 = [](int a, int b){ return a + b; };
	auto lbd1 = [](float a, int b){ return a + b; };

	tuple<int, int> t0{1, 3};
	tuple<float, int> t1{1.5f, 3};

	SSVUT_EXPECT_OP((explode(lbd0, t0)), ==, 4);
	SSVUT_EXPECT_OP((explode(lbd1, t1)), ==, (1.5f + 3));
	SSVUT_EXPECT_OP((explode(lbd1, t0)), ==, 4);
	SSVUT_EXPECT_OP((explode(lbd0, t1)), ==, (static_cast<int>(1.5f) + 3));

	double acc{0};
	tplFor([&acc](auto x)
	{
		acc += x;
	}, tuple<int, float, int, double>(1, 2.f, 3, 4.));

	SSVUT_EXPECT_OP(acc, ==, 1 + 2.f + 3 + 4.);

	acc = 0;
	tplFor([&acc](auto x, auto y)
	{
		acc += x;
		acc += y;
	}, tuple<int, float, int, double>(1, 2.f, 3, 4.), tuple<int, int>(1, 2));

	SSVUT_EXPECT_OP(acc, ==, 1 + 2.f + 1 + 2);

	acc = 0;
	tplForIdx([&acc](auto i, auto x, auto y)
	{
		acc += x;
		acc += y;
		acc += i;
	}, tuple<int, float, int, double>(1, 2.f, 3, 4.), tuple<int, int>(1, 2));

	SSVUT_EXPECT_OP(acc, ==, 1 + 2.f + 1 + 2 + 0 + 1);
}

#endif
