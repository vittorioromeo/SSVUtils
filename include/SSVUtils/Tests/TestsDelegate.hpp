// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSDELEGATE
#define SSVU_TESTS_TESTSDELEGATE

SSVUT_TEST(DelegateTests)
{
	using namespace std;
	using namespace ssvu;

	bool testState{false};
	Delegate<void()> del1;
	del1 += [&testState]{ testState = !testState; };

	del1();
	SSVUT_EXPECT(testState == true);
	del1();
	SSVUT_EXPECT(testState == false);

	Delegate<int(int)> del2;
	del2 += [](int x){ return x + x; };
	del2 += [](int x){ return x * x; };

	auto del2result(del2(3));
	SSVUT_EXPECT(del2result[0] == 6);
	SSVUT_EXPECT(del2result[1] == 9);
	SSVUT_EXPECT(del2result.size() == 2);

	Delegate<void()> del3;
	del3 += [&del1]{ del1(); };
	del3(); SSVUT_EXPECT(testState == true);

	Delegate<void(int&)> del4;
	del4 += [](int& i){ i += 1; };
	del4 += [](int& i){ i += 1; };

	int f = 0;
	del4(f);
	SSVUT_EXPECT_OP(f, ==, 2);
	del4(f);
	SSVUT_EXPECT_OP(f, ==, 4);
	del4(f);
	SSVUT_EXPECT_OP(f, ==, 6);
	del4(f);
	SSVUT_EXPECT_OP(f, ==, 8);

	int i = 0;
	Delegate<int()> del5;
	del5 += [i]() mutable { i += 1; return i; };
	SSVUT_EXPECT_OP(del5()[0], ==, 1);
	SSVUT_EXPECT_OP(del5()[0], ==, 2);
	SSVUT_EXPECT_OP(del5()[0], ==, 3);
	SSVUT_EXPECT_OP(del5()[0], ==, 4);
	SSVUT_EXPECT_OP(del5()[0], ==, 5);
	SSVUT_EXPECT_OP(del5()[0], ==, 6);
	SSVUT_EXPECT_OP(del5()[0], ==, 7);
}

#endif
