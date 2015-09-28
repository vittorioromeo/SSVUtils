// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"

#include "SSVUtils/Delegate/Delegate.hpp"

#include "./utils/test_utils.hpp"
int main()
{
    using namespace std;
    using namespace ssvu;

    bool testState{false};
    Delegate<void()> del1;
    del1 += [&testState]
    {
        testState = !testState;
    };

    del1();
    TEST_ASSERT(testState == true);
    del1();
    TEST_ASSERT(testState == false);

    Delegate<int(int)> del2;
    del2 += [](int x)
    {
        return x + x;
    };
    del2 += [](int x)
    {
        return x * x;
    };

    auto del2result(del2(3));
    TEST_ASSERT(del2result[0] == 6);
    TEST_ASSERT(del2result[1] == 9);
    TEST_ASSERT(del2result.size() == 2);

    Delegate<void()> del3;
    del3 += [&del1]
    {
        del1();
    };
    del3();
    TEST_ASSERT(testState == true);

    Delegate<void(int&)> del4;
    del4 += [](int& i)
    {
        i += 1;
    };
    del4 += [](int& i)
    {
        i += 1;
    };

    int f = 0;
    del4(f);
    TEST_ASSERT_OP(f, ==, 2);
    del4(f);
    TEST_ASSERT_OP(f, ==, 4);
    del4(f);
    TEST_ASSERT_OP(f, ==, 6);
    del4(f);
    TEST_ASSERT_OP(f, ==, 8);

    {
        int i = 0;
        Delegate<int()> del5;
        del5 += [i]() mutable
        {
            i += 1;
            return i;
        };
        TEST_ASSERT_OP(del5()[0], ==, 1);
        TEST_ASSERT_OP(del5()[0], ==, 2);
        TEST_ASSERT_OP(del5()[0], ==, 3);
        TEST_ASSERT_OP(del5()[0], ==, 4);
        TEST_ASSERT_OP(del5()[0], ==, 5);
        TEST_ASSERT_OP(del5()[0], ==, 6);
        TEST_ASSERT_OP(del5()[0], ==, 7);
    }
}
