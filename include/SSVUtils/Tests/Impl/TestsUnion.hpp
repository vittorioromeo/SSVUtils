// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSUNION
#define SSVU_TESTS_TESTSUNION

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Test/Test.hpp"
#include "SSVUtils/Union/Union.hpp"

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

#define EXEC_TEST_POD_CPMV(mType, mX) \
	{ \
		u.init<mType>(mX); \
		SSVUT_EXPECT_OP(u.get<mType>(), ==, mX); \
		UnionPOD<char, short int, int, float, long int, double> j; \
		j = u; \
		SSVUT_EXPECT_OP(j.get<mType>(), ==, mX); \
		UnionPOD<char, short int, int, float, long int, double> k; \
		k = mv(j); \
		SSVUT_EXPECT_OP(k.get<mType>(), ==, mX); \
	}


SSVUT_TEST(UnionTests)
{
	using namespace ssvu;

	{
		Union<char, short int, int, float, long int, double, std::string> u;
		EXEC_TEST(char, 'c')
		EXEC_TEST(short int, 152)
		EXEC_TEST(int, 152)
		EXEC_TEST(float, 152.5f)
		EXEC_TEST(long int, 152)
		EXEC_TEST(double, 33.21)
		EXEC_TEST(std::string, "hi")
	}

	{
		UnionPOD<char, short int, int, float, long int, double> u;
		EXEC_TEST_POD(char, 'c')
		EXEC_TEST_POD(short int, 152)
		EXEC_TEST_POD(int, 152)
		EXEC_TEST_POD(float, 152.5f)
		EXEC_TEST_POD(long int, 152)
		EXEC_TEST_POD(double, 33.21)
	}

	Maybe<int> mbInt;
	Maybe<std::vector<int>> mbVecInt;

	mbInt.init(3);
	mbInt.deinit();

	MaybePOD<int> mbpInt;
	mbpInt.init(5);

	{
		UnionPOD<char, short int, int, float, long int, double> u;
		EXEC_TEST_POD_CPMV(char, 'c')
		EXEC_TEST_POD_CPMV(short int, 152)
		EXEC_TEST_POD_CPMV(int, 152)
		EXEC_TEST_POD_CPMV(float, 152.5f)
		EXEC_TEST_POD_CPMV(long int, 152)
		EXEC_TEST_POD_CPMV(double, 33.21)
	}
}

#undef EXEC_TEST
#undef EXEC_TEST_POD

#endif
