// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0


#include "SSVUtils/Core/Core.hpp"


#include "./utils/test_utils.hpp"
int main()
{
    using namespace std;
    using namespace ssvu;

    for(int i{0}; i < 100; ++i)
    {
        auto r(getRndI<int>(-10, 10));
        TEST_ASSERT(r >= -10 && r < 10);

        // getRnd is [a, b)
        auto ru(getRndI<unsigned int>(1, 10));
        TEST_ASSERT(ru > 0 && ru < 10);

        // getRndR is [a, b]
        auto rf(getRndR<float>(0.f, 2.f));
        TEST_ASSERT(rf >= 0.f && rf <= 2.f);

        auto rs(getRndSign());
        TEST_ASSERT(rs == 1 || rs == -1);
    }

    TEST_ASSERT(getSign(-1) == -1);
    TEST_ASSERT(getSign(1) == 1);
    TEST_ASSERT(getSign(0) == 0);

    TEST_ASSERT(getSign(-1u) == 1);
    TEST_ASSERT(getSign(1u) == 1);
    TEST_ASSERT(getSign(0u) == 0);

    TEST_ASSERT(getSign(-1.5f) == -1);
    TEST_ASSERT(getSign(1.5f) == 1);
    TEST_ASSERT(getSign(0.5f) == 1);
    TEST_ASSERT(getSign(0.f) == 0);

    TEST_ASSERT(getClampedMin(0, 100) == 100);
    TEST_ASSERT(getClampedMin(150, 100) == 150);
    TEST_ASSERT(getClampedMax(0, 100) == 0);
    TEST_ASSERT(getClampedMax(150, 100) == 100);
    TEST_ASSERT(getClamped(1000, 0, 500) == 500);
    TEST_ASSERT(getClamped(1000, 1500, 2500) == 1500);

    TEST_ASSERT(getWrapDeg(720) == 0);
    TEST_ASSERT(getWrapDeg(720 + 180) == 180);

    TEST_ASSERT(getMod(10, 9) == 1);
    TEST_ASSERT(getMod(-10, 9) == 8);
    TEST_ASSERT(getMod(2, 2) == 0);
    TEST_ASSERT(getMod(16000, 2) == 0);
    TEST_ASSERT(getMod(16001, 2) == 1);
    TEST_ASSERT(getMod(-16000, 2) == 0);
    TEST_ASSERT(getMod(-16001, 2) == 1);

    TEST_ASSERT(getMod(10, 5) == 0);
    TEST_ASSERT(getMod(10, 10) == 0);
    TEST_ASSERT(getMod(10, 15) == 10);
    TEST_ASSERT(getMod(4, 3) == 1);
    TEST_ASSERT(getMod(5, 3) == 2);
    TEST_ASSERT(getMod(6, 3) == 0);
    TEST_ASSERT(getMod(-1, 3) == 2);
    TEST_ASSERT(getMod(-2, 3) == 1);
    TEST_ASSERT(getMod(-3, 3) == 0);
    TEST_ASSERT(getMod(-4, 3) == 2);

    TEST_ASSERT(getMod(-2, -2, 2) == -2);
    TEST_ASSERT(getMod(-1, -2, 2) == -1);
    TEST_ASSERT(getMod(0, -2, 2) == 0);
    TEST_ASSERT(getMod(1, -2, 2) == 1);
    TEST_ASSERT(getMod(2, -2, 2) == -2);
    TEST_ASSERT(getMod(3, -2, 2) == -1);
    TEST_ASSERT(getMod(4, -2, 2) == 0);
    TEST_ASSERT(getMod(5, -2, 2) == 1);
    TEST_ASSERT(getMod(6, -2, 2) == -2);

    TEST_ASSERT(getMap(100, 0, 100, 0, 200) == 200);
    TEST_ASSERT(getMap(50, 0, 100, 0, 200) == 100);
    TEST_ASSERT(getMap(0, -100, 100, 0, 200) == 100);
    TEST_ASSERT(getMap(0, -100, 100, -200, 200) == 0);

    TEST_ASSERT(getLerp(0, -500, 500) == -500);
    TEST_ASSERT(getLerp(1, -500, 500) == 500);
    TEST_ASSERT(getLerp(0.5, -500, 500) == 0);
}
