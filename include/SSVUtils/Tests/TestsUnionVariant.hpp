// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSUNIONVARIANT
#define SSVU_TESTS_TESTSUNIONVARIANT

#define EXEC_TEST(mType, mX) \
	{ \
		u.init<mType>(mX); \
		SSVUT_EXPECT_OP(u.get<mType>(), ==, mX); \
		u.deinit<mType>(); \
	}

#define EXEC_TEST_POD(mType, mX) \
	{ \
		u.init<mType>(mX); \
		SSVUT_EXPECT_OP(u.get<mType>(), ==, mX); \
	}


SSVUT_TEST(UnionVariantTests)
{
	using namespace ssvu;

	{
		UnionVariant<char, short int, int, float, long int, double, std::string> u;
		EXEC_TEST(char, 'c')
		EXEC_TEST(short int, 152)
		EXEC_TEST(int, 152)
		EXEC_TEST(float, 152.5f)
		EXEC_TEST(long int, 152)
		EXEC_TEST(double, 33.21)
		EXEC_TEST(std::string, "hi")
	}

	{
		UnionVariantPOD<char, short int, int, float, long int, double> u;
		EXEC_TEST_POD(char, 'c')
		EXEC_TEST_POD(short int, 152)
		EXEC_TEST_POD(int, 152)
		EXEC_TEST_POD(float, 152.5f)
		EXEC_TEST_POD(long int, 152)
		EXEC_TEST_POD(double, 33.21)
	}
}

#undef EXEC_TEST
#undef EXEC_TEST_POD

#endif
