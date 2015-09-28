// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"

#include "SSVUtils/GrowableArray/GrowableArray.hpp"

#include "./utils/test_utils.hpp"
int main()
{
    using namespace ssvu;

    {
        GrowableArray<int> gab;
        gab.grow(0, 10);

        for(int i = 0; i < 10; ++i) gab[i] = i;
        for(int i = 0; i < 10; ++i) {
            TEST_ASSERT_OP(gab[i], ==, i);
        }

        gab.grow(10, 20);
        for(int i = 0; i < 10; ++i) {
            TEST_ASSERT_OP(gab[i], ==, i);
        }
    }

    volatile int cc{0};
    volatile int dc{0};

    struct TestItem
    {
        volatile int& mycc;
        volatile int& mydc;
        int k;
        TestItem(volatile int& mCC, volatile int& mDC, int mk)
            : mycc(mCC), mydc(mDC), k(mk)
        {
            ++mycc;
        }
        ~TestItem() { ++mydc; }
    };

    using namespace ssvu;
    using namespace ssvu::Impl;

    TEST_ASSERT_OP(cc, ==, 0);
    TEST_ASSERT_OP(dc, ==, 0);

    {
        GrowableArrayAS<TestItem> temp;
    }

    TEST_ASSERT_OP(cc, ==, 0);
    TEST_ASSERT_OP(dc, ==, 0);

    {
        GrowableArrayAS<TestItem> g;
        g.grow(0, 4);
        g.initAt(0, cc, dc, 0);
        g.initAt(1, cc, dc, 1);
        g.initAt(2, cc, dc, 2);
        g.initAt(3, cc, dc, 3);

        TEST_ASSERT_OP(cc, ==, 4);
        TEST_ASSERT_OP(dc, ==, 0);

        TEST_ASSERT_OP(g[0].k, ==, 0);
        TEST_ASSERT_OP(g[1].k, ==, 1);
        TEST_ASSERT_OP(g[2].k, ==, 2);
        TEST_ASSERT_OP(g[3].k, ==, 3);

        g.grow(4, 8);

        TEST_ASSERT_OP(cc, ==, 4);
        TEST_ASSERT_OP(dc, ==, 0);

        TEST_ASSERT_OP(g[0].k, ==, 0);
        TEST_ASSERT_OP(g[1].k, ==, 1);
        TEST_ASSERT_OP(g[2].k, ==, 2);
        TEST_ASSERT_OP(g[3].k, ==, 3);

        g.deinitAt(0);
        g.deinitAt(1);
        g.deinitAt(2);
        g.deinitAt(3);

        TEST_ASSERT_OP(cc, ==, 4);
        TEST_ASSERT_OP(dc, ==, 4);
    }

    TEST_ASSERT_OP(cc, ==, 4);
    TEST_ASSERT_OP(dc, ==, 4);

    cc = dc = 0;

    {
        GrowableArrayAS<UPtr<TestItem>> g;
        g.grow(0, 4);
        g.initAt(0, mkUPtr<TestItem>(cc, dc, 0));
        g.initAt(1, mkUPtr<TestItem>(cc, dc, 1));
        g.initAt(2, mkUPtr<TestItem>(cc, dc, 2));
        g.initAt(3, mkUPtr<TestItem>(cc, dc, 3));

        TEST_ASSERT_OP(cc, ==, 4);
        TEST_ASSERT_OP(dc, ==, 0);

        TEST_ASSERT_OP(g[0]->k, ==, 0);
        TEST_ASSERT_OP(g[1]->k, ==, 1);
        TEST_ASSERT_OP(g[2]->k, ==, 2);
        TEST_ASSERT_OP(g[3]->k, ==, 3);

        g.grow(4, 8);

        TEST_ASSERT_OP(cc, ==, 4);
        TEST_ASSERT_OP(dc, ==, 0);

        TEST_ASSERT_OP(g[0]->k, ==, 0);
        TEST_ASSERT_OP(g[1]->k, ==, 1);
        TEST_ASSERT_OP(g[2]->k, ==, 2);
        TEST_ASSERT_OP(g[3]->k, ==, 3);

        g.grow(8, 1000);

        TEST_ASSERT_OP(cc, ==, 4);
        TEST_ASSERT_OP(dc, ==, 0);

        TEST_ASSERT_OP(g[0]->k, ==, 0);
        TEST_ASSERT_OP(g[1]->k, ==, 1);
        TEST_ASSERT_OP(g[2]->k, ==, 2);
        TEST_ASSERT_OP(g[3]->k, ==, 3);

        g.deinitAt(0);
        g.deinitAt(1);
        g.deinitAt(2);
        g.deinitAt(3);

        TEST_ASSERT_OP(cc, ==, 4);
        TEST_ASSERT_OP(dc, ==, 4);
    }

    cc = dc = 0;

    {
        GrowableArrayAS<TestItem> g;
        TEST_ASSERT_OP(cc, ==, 0);
        TEST_ASSERT_OP(dc, ==, 0);
        g.grow(0, 1);
        g.initAt(0, cc, dc, 0);
        TEST_ASSERT_OP(cc, ==, 1);
        TEST_ASSERT_OP(dc, ==, 0);
        TEST_ASSERT_OP(g[0].k, ==, 0);
        g.deinitAt(0);
        TEST_ASSERT_OP(cc, ==, 1);
        TEST_ASSERT_OP(dc, ==, 1);
    }
}
