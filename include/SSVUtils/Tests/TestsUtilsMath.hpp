// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSUTILSMATH
#define SSVU_TESTS_TESTSUTILSMATH

SSVUT_TEST(UtilsMathTests)
{
	using namespace std;
	using namespace ssvu;

	for(int i{0}; i < 100; ++i)
	{
		auto r(getRnd<int>(-10, 10));
		SSVUT_EXPECT(r >= -10 && r < 10);

		// getRnd is [a, b)
		auto ru(getRnd<unsigned int>(1, 10));
		SSVUT_EXPECT(ru > 0 && ru < 10);

		// getRndR is [a, b]
		auto rf(getRndR<float>(0.f, 2.f));
		SSVUT_EXPECT(rf >= 0.f && rf <= 2.f);

		auto rs(getRndSign());
		SSVUT_EXPECT(rs == 1 || rs == -1);
	}

	SSVUT_EXPECT(getSign(-1) == -1);
	SSVUT_EXPECT(getSign(1) == 1);
	SSVUT_EXPECT(getSign(0) == 0);

	SSVUT_EXPECT(getSign(-1u) == 1);
	SSVUT_EXPECT(getSign(1u) == 1);
	SSVUT_EXPECT(getSign(0u) == 0);

	SSVUT_EXPECT(getSign(-1.5f) == -1);
	SSVUT_EXPECT(getSign(1.5f) == 1);
	SSVUT_EXPECT(getSign(0.5f) == 1);
	SSVUT_EXPECT(getSign(0.f) == 0);

	SSVUT_EXPECT(getClampedMin(0, 100) == 100);
	SSVUT_EXPECT(getClampedMin(150, 100) == 150);
	SSVUT_EXPECT(getClampedMax(0, 100) == 0);
	SSVUT_EXPECT(getClampedMax(150, 100) == 100);
	SSVUT_EXPECT(getClamped(1000, 0, 500) == 500);
	SSVUT_EXPECT(getClamped(1000, 1500, 2500) == 1500);

	SSVUT_EXPECT(wrapDeg(720) == 0);
	SSVUT_EXPECT(wrapDeg(720 + 180) == 180);

	SSVUT_EXPECT(getMod(10, 9) == 1);
	SSVUT_EXPECT(getMod(-10, 9) == 8);
	SSVUT_EXPECT(getMod(2, 2) == 0);
	SSVUT_EXPECT(getMod(16000, 2) == 0);
	SSVUT_EXPECT(getMod(16001, 2) == 1);
	SSVUT_EXPECT(getMod(-16000, 2) == 0);
	SSVUT_EXPECT(getMod(-16001, 2) == 1);

	SSVUT_EXPECT(getMod(10, 5) == 0);
	SSVUT_EXPECT(getMod(10, 10) == 0);
	SSVUT_EXPECT(getMod(10, 15) == 10);
	SSVUT_EXPECT(getMod(4, 3) == 1);
	SSVUT_EXPECT(getMod(5, 3) == 2);
	SSVUT_EXPECT(getMod(6, 3) == 0);
	SSVUT_EXPECT(getMod(-1, 3) == 2);
	SSVUT_EXPECT(getMod(-2, 3) == 1);
	SSVUT_EXPECT(getMod(-3, 3) == 0);
	SSVUT_EXPECT(getMod(-4, 3) == 2);

	SSVUT_EXPECT(getMod(-2, -2, 2) == -2);
	SSVUT_EXPECT(getMod(-1, -2, 2) == -1);
	SSVUT_EXPECT(getMod(0, -2, 2) == 0);
	SSVUT_EXPECT(getMod(1, -2, 2) == 1);
	SSVUT_EXPECT(getMod(2, -2, 2) == -2);
	SSVUT_EXPECT(getMod(3, -2, 2) == -1);
	SSVUT_EXPECT(getMod(4, -2, 2) == 0);
	SSVUT_EXPECT(getMod(5, -2, 2) == 1);
	SSVUT_EXPECT(getMod(6, -2, 2) == -2);

	SSVUT_EXPECT(getMap(100, 0, 100, 0, 200) == 200);
	SSVUT_EXPECT(getMap(50, 0, 100, 0, 200) == 100);
	SSVUT_EXPECT(getMap(0, -100, 100, 0, 200) == 100);
	SSVUT_EXPECT(getMap(0, -100, 100, -200, 200) == 0);

	SSVUT_EXPECT(getLerp(0, -500, 500) == -500);
	SSVUT_EXPECT(getLerp(1, -500, 500) == 500);
	SSVUT_EXPECT(getLerp(0.5, -500, 500) == 0);
}

#endif
