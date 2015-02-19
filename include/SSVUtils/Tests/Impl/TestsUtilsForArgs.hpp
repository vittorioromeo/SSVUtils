// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSUTILSFORARGS
#define SSVU_TESTS_TESTSUTILSFORARGS

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Test/Test.hpp"

SSVUT_TEST(UtilsForArgs)
{
	using namespace std;
	using namespace ssvu;

	{
		int acc{0};
		forArgs([&acc](auto x){ acc += x; }, 0, 1, 2, 3, 4, 5);
		SSVUT_EXPECT_OP(acc, ==, (0+1+2+3+4+5));
	}

	{
		int acc{0};
		forArgs([&acc](auto&& x){ acc += x; }, 0, 1, 2, 3, 4, 5);
		SSVUT_EXPECT_OP(acc, ==, (0+1+2+3+4+5));
	}

	{
		int steps = 0;
		int ctrl = 0;

		auto a = [&steps, &ctrl]{ ++steps; ctrl = 1; return 1; };
		auto b = [&steps, &ctrl]{ ++steps; ctrl = 2; return 2; };
		auto c = [&steps, &ctrl]{ ++steps; ctrl = 3; return 3; };
		auto d = [&steps, &ctrl]{ ++steps; ctrl = 4; return 4; };
		auto e = [&steps, &ctrl]{ ++steps; ctrl = 5; return 5; };

		int acc{0};
		forArgs([this, &ctrl, &steps, &acc](auto x)
		{
			auto val(x());
			acc += val;

			if(steps == 1) SSVUT_EXPECT_OP(ctrl, ==, 2);
			if(steps == 2) SSVUT_EXPECT_OP(ctrl, ==, 1);
			if(steps == 3) SSVUT_EXPECT_OP(ctrl, ==, 4);
			if(steps == 4) SSVUT_EXPECT_OP(ctrl, ==, 3);
			if(steps == 5) SSVUT_EXPECT_OP(ctrl, ==, 5);
		}, b, a, d, c, e);
		SSVUT_EXPECT_OP(acc, ==, (0+1+2+3+4+5));
	}
}

#endif
