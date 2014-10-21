// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSGROWABLEARRAY
#define SSVU_TESTS_TESTSGROWABLEARRAY

SSVUT_TEST(GrowableArrayTests)
{
	volatile int cc{0};
	volatile int dc{0};

	struct TestItem
	{
		volatile int& mycc;
		volatile int& mydc;
		int k;
		TestItem(volatile int& mCC, volatile int& mDC, int mk) : mycc(mCC), mydc(mDC), k(mk)
		{
			++mycc;
		}
		~TestItem()
		{
			++mydc;
		}
	};

	using namespace ssvu;
	using namespace ssvu::Internal;

	SSVUT_EXPECT_OP(cc, ==, 0);
	SSVUT_EXPECT_OP(dc, ==, 0);

	{
		GrowableArray<TestItem> temp;
	}

	SSVUT_EXPECT_OP(cc, ==, 0);
	SSVUT_EXPECT_OP(dc, ==, 0);

	{
		GrowableArray<TestItem> g;
		g.grow(0, 4);
		g.initAt(0, cc, dc, 0);
		g.initAt(1, cc, dc, 1);
		g.initAt(2, cc, dc, 2);
		g.initAt(3, cc, dc, 3);

		SSVUT_EXPECT_OP(cc, ==, 4);
		SSVUT_EXPECT_OP(dc, ==, 0);

		SSVUT_EXPECT_OP(g[0].k, ==, 0);
		SSVUT_EXPECT_OP(g[1].k, ==, 1);
		SSVUT_EXPECT_OP(g[2].k, ==, 2);
		SSVUT_EXPECT_OP(g[3].k, ==, 3);

		g.grow(4, 8);

		SSVUT_EXPECT_OP(cc, ==, 4);
		SSVUT_EXPECT_OP(dc, ==, 0);

		SSVUT_EXPECT_OP(g[0].k, ==, 0);
		SSVUT_EXPECT_OP(g[1].k, ==, 1);
		SSVUT_EXPECT_OP(g[2].k, ==, 2);
		SSVUT_EXPECT_OP(g[3].k, ==, 3);

		g.deinitAt(0);
		g.deinitAt(1);
		g.deinitAt(2);
		g.deinitAt(3);

		SSVUT_EXPECT_OP(cc, ==, 4);
		SSVUT_EXPECT_OP(dc, ==, 4);
	}

	SSVUT_EXPECT_OP(cc, ==, 4);
	SSVUT_EXPECT_OP(dc, ==, 4);

	cc = dc = 0;

	{
		GrowableArray<UPtr<TestItem>> g;
		g.grow(0, 4);
		g.initAt(0, makeUPtr<TestItem>(cc, dc, 0));
		g.initAt(1, makeUPtr<TestItem>(cc, dc, 1));
		g.initAt(2, makeUPtr<TestItem>(cc, dc, 2));
		g.initAt(3, makeUPtr<TestItem>(cc, dc, 3));

		SSVUT_EXPECT_OP(cc, ==, 4);
		SSVUT_EXPECT_OP(dc, ==, 0);

		SSVUT_EXPECT_OP(g[0]->k, ==, 0);
		SSVUT_EXPECT_OP(g[1]->k, ==, 1);
		SSVUT_EXPECT_OP(g[2]->k, ==, 2);
		SSVUT_EXPECT_OP(g[3]->k, ==, 3);

		g.grow(4, 8);

		SSVUT_EXPECT_OP(cc, ==, 4);
		SSVUT_EXPECT_OP(dc, ==, 0);

		SSVUT_EXPECT_OP(g[0]->k, ==, 0);
		SSVUT_EXPECT_OP(g[1]->k, ==, 1);
		SSVUT_EXPECT_OP(g[2]->k, ==, 2);
		SSVUT_EXPECT_OP(g[3]->k, ==, 3);

		g.grow(8, 1000);

		SSVUT_EXPECT_OP(cc, ==, 4);
		SSVUT_EXPECT_OP(dc, ==, 0);

		SSVUT_EXPECT_OP(g[0]->k, ==, 0);
		SSVUT_EXPECT_OP(g[1]->k, ==, 1);
		SSVUT_EXPECT_OP(g[2]->k, ==, 2);
		SSVUT_EXPECT_OP(g[3]->k, ==, 3);

		g.deinitAt(0);
		g.deinitAt(1);
		g.deinitAt(2);
		g.deinitAt(3);

		SSVUT_EXPECT_OP(cc, ==, 4);
		SSVUT_EXPECT_OP(dc, ==, 4);
	}

	cc = dc = 0;

	{
		GrowableArray<TestItem> g;
		SSVUT_EXPECT_OP(cc, ==, 0);
		SSVUT_EXPECT_OP(dc, ==, 0);
		g.grow(0, 1);
		g.initAt(0, cc, dc, 0);
		SSVUT_EXPECT_OP(cc, ==, 1);
		SSVUT_EXPECT_OP(dc, ==, 0);
		SSVUT_EXPECT_OP(g[0].k, ==, 0);
		g.deinitAt(0);
		SSVUT_EXPECT_OP(cc, ==, 1);
		SSVUT_EXPECT_OP(dc, ==, 1);
	}
}

#endif
