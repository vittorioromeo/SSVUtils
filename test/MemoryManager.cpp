// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0


#include "SSVUtils/Core/Core.hpp"

#include "SSVUtils/MemoryManager/MemoryManager.hpp"

#include "./utils/test_utils.hpp"
int main()
{
    {
        volatile int cc{0};
        volatile int dc{0};

        struct TMMItem
        {
            volatile int& dcRef;
            inline TMMItem(volatile int& mCC, volatile int& mDC) : dcRef(mDC)
            {
                ++mCC;
            }
            inline virtual ~TMMItem() { ++dcRef; }
        };

        struct TMMItemS : public TMMItem
        {
            char stuff[2];
            using TMMItem::TMMItem;
        };
        struct TMMItemB : public TMMItem
        {
            char stuff[20];
            using TMMItem::TMMItem;
        };

        TEST_ASSERT(cc == 0 && dc == 0);

        {
            ssvu::MonoRecVector<TMMItem> mrv;
            auto& i1 __attribute__((unused)) = mrv.create(cc, dc);
            auto& i2 __attribute__((unused)) = mrv.create(cc, dc);
        }

        {
            ssvu::PolyRecVector<TMMItem> prv;
            auto& i1 __attribute__((unused)) =
            prv.template create<TMMItemS>(cc, dc);
            auto& i2 __attribute__((unused)) =
            prv.template create<TMMItemB>(cc, dc);
        }

        {
            ssvu::PolyFixedRecVector<TMMItem, TMMItemS, TMMItemB> prvf;
            auto& i1 __attribute__((unused)) =
            prvf.template create<TMMItemS>(cc, dc);
            auto& i2 __attribute__((unused)) =
            prvf.template create<TMMItemB>(cc, dc);
        }

        TEST_ASSERT(cc > 0 && dc > 0);
    }

    {
        volatile int cc{0};
        volatile int dc{0};

        struct TMMItem
        {
            volatile int& dcRef;
            inline TMMItem(volatile int& mCC, volatile int& mDC) : dcRef(mDC)
            {
                ++mCC;
            }
            inline virtual ~TMMItem() { ++dcRef; }
        };

        struct TMMItemS : public TMMItem
        {
            char stuff[2];
            using TMMItem::TMMItem;
        };
        struct TMMItemB : public TMMItem
        {
            char stuff[20];
            using TMMItem::TMMItem;
        };

        {
            TEST_ASSERT(cc == 0 && dc == 0);

            ssvu::MonoRecycler<TMMItem> mr;

            {
                auto i1 = mr.create(cc, dc);
                auto i2 = mr.create(cc, dc);
                mr.create(cc, dc); // dies immediately

                TEST_ASSERT(cc == 3 && dc == 1);
            }

            TEST_ASSERT(cc == 3 && dc == 3);
        }

        cc = 0;
        dc = 0;

        {
            TEST_ASSERT(cc == 0 && dc == 0);

            ssvu::PolyRecycler<TMMItem> pr;

            {
                auto i1 = pr.create<TMMItemS>(cc, dc);
                auto i2 = pr.create<TMMItemB>(cc, dc);
                pr.create<TMMItemS>(cc, dc);
                pr.create<TMMItemB>(cc, dc);

                TEST_ASSERT(cc == 4 && dc == 2);
            }

            TEST_ASSERT(cc == 4 && dc == 4);

            {
                auto i1 = pr.create<TMMItemS>(cc, dc);
                auto i2 = pr.create<TMMItemB>(cc, dc);
                pr.create<TMMItemS>(cc, dc);
                pr.create<TMMItemB>(cc, dc);

                TEST_ASSERT(cc == 8 && dc == 6);
            }

            TEST_ASSERT(cc == 8 && dc == 8);
        }

        cc = 0;
        dc = 0;

        {
            TEST_ASSERT(cc == 0 && dc == 0);

            ssvu::MonoManager<TMMItem> mm;

            {
                auto& i1 __attribute__((unused)) = mm.create(cc, dc);
                TEST_ASSERT(cc == 1 && dc == 0);

                auto& i2 __attribute__((unused)) = mm.create(cc, dc);
                TEST_ASSERT(cc == 2 && dc == 0);

                mm.create(cc, dc);
                TEST_ASSERT(cc == 3 && dc == 0);
            }

            mm.refresh();
            TEST_ASSERT(cc == 3 && dc == 0);

            for(auto& i : mm) mm.del(*i);
            TEST_ASSERT(cc == 3 && dc == 0);

            mm.refresh();
            TEST_ASSERT(cc == 3 && dc == 3);
        }

        cc = 0;
        dc = 0;

        {
            TEST_ASSERT(cc == 0 && dc == 0);

            ssvu::PolyManager<TMMItem> mm;

            {
                auto& i1 __attribute__((unused)) = mm.create<TMMItemS>(cc, dc);
                TEST_ASSERT(cc == 1 && dc == 0);

                auto& i2 __attribute__((unused)) = mm.create<TMMItemB>(cc, dc);
                TEST_ASSERT(cc == 2 && dc == 0);

                mm.create<TMMItemS>(cc, dc);
                TEST_ASSERT(cc == 3 && dc == 0);

                mm.create<TMMItemB>(cc, dc);
                TEST_ASSERT(cc == 4 && dc == 0);
            }

            mm.refresh();
            TEST_ASSERT(cc == 4 && dc == 0);

            for(auto& i : mm) mm.del(*i);
            TEST_ASSERT(cc == 4 && dc == 0);

            mm.refresh();
            TEST_ASSERT(cc == 4 && dc == 4);
        }

        cc = dc = 0;

        {
            TEST_ASSERT_OP(cc, ==, 0);
            TEST_ASSERT_OP(dc, ==, 0);

            ssvu::MonoManager<TMMItem> mm;

            TEST_ASSERT_OP(cc, ==, 0);
            TEST_ASSERT_OP(dc, ==, 0);

            auto& i1 = mm.create(cc, dc);

            TEST_ASSERT_OP(cc, ==, 1);
            TEST_ASSERT_OP(dc, ==, 0);

            mm.refresh();

            TEST_ASSERT_OP(cc, ==, 1);
            TEST_ASSERT_OP(dc, ==, 0);

            mm.del(i1);

            TEST_ASSERT_OP(cc, ==, 1);
            TEST_ASSERT_OP(dc, ==, 0);

            mm.refresh();

            TEST_ASSERT_OP(cc, ==, 1);
            TEST_ASSERT_OP(dc, ==, 1);
        }

        cc = dc = 0;

        {
            TEST_ASSERT_OP(cc, ==, 0);
            TEST_ASSERT_OP(dc, ==, 0);

            ssvu::MonoManager<TMMItem> mm;

            TEST_ASSERT_OP(cc, ==, 0);
            TEST_ASSERT_OP(dc, ==, 0);

            auto& i1 __attribute__((unused)) = mm.create(cc, dc);

            TEST_ASSERT_OP(cc, ==, 1);
            TEST_ASSERT_OP(dc, ==, 0);

            mm.refresh();
        }

        TEST_ASSERT_OP(cc, ==, 1);
        TEST_ASSERT_OP(dc, ==, 1);

        cc = dc = 0;

        {
            TEST_ASSERT_OP(cc, ==, 0);
            TEST_ASSERT_OP(dc, ==, 0);

            ssvu::PolyManager<TMMItem> mm;

            {
                auto& i1 __attribute__((unused)) = mm.create<TMMItemS>(cc, dc);
                TEST_ASSERT_OP(cc, ==, 1);
                TEST_ASSERT_OP(dc, ==, 0);

                auto& i2 __attribute__((unused)) = mm.create<TMMItemB>(cc, dc);
                TEST_ASSERT_OP(cc, ==, 2);
                TEST_ASSERT_OP(dc, ==, 0);

                auto& i3 __attribute__((unused)) = mm.create<TMMItemB>(cc, dc);
                TEST_ASSERT_OP(cc, ==, 3);
                TEST_ASSERT_OP(dc, ==, 0);
            }

            TEST_ASSERT_OP(cc, ==, 3);
            TEST_ASSERT_OP(dc, ==, 0);
        }

        TEST_ASSERT_OP(cc, ==, 3);
        TEST_ASSERT_OP(dc, ==, 3);

        cc = dc = 0;

        {
            TEST_ASSERT_OP(cc, ==, 0);
            TEST_ASSERT_OP(dc, ==, 0);

            ssvu::PolyManager<TMMItem> mm;

            {
                auto& i1 __attribute__((unused)) = mm.create<TMMItemS>(cc, dc);
                TEST_ASSERT_OP(cc, ==, 1);
                TEST_ASSERT_OP(dc, ==, 0);

                auto& i2 __attribute__((unused)) = mm.create<TMMItemB>(cc, dc);
                TEST_ASSERT_OP(cc, ==, 2);
                TEST_ASSERT_OP(dc, ==, 0);

                auto& i3 __attribute__((unused)) = mm.create<TMMItemB>(cc, dc);
                TEST_ASSERT_OP(cc, ==, 3);
                TEST_ASSERT_OP(dc, ==, 0);
            }

            TEST_ASSERT_OP(cc, ==, 3);
            TEST_ASSERT_OP(dc, ==, 0);

            mm.refresh();
        }

        TEST_ASSERT_OP(cc, ==, 3);
        TEST_ASSERT_OP(dc, ==, 3);

        cc = dc = 0;

        {
            TEST_ASSERT_OP(cc, ==, 0);
            TEST_ASSERT_OP(dc, ==, 0);

            ssvu::PolyManager<TMMItem> mm;

            {
                auto& __attribute__((unused)) i1 __attribute__((unused)) =
                mm.create<TMMItemS>(cc, dc);
                TEST_ASSERT_OP(cc, ==, 1);
                TEST_ASSERT_OP(dc, ==, 0);

                auto& __attribute__((unused)) i2 __attribute__((unused)) =
                mm.create<TMMItemB>(cc, dc);
                TEST_ASSERT_OP(cc, ==, 2);
                TEST_ASSERT_OP(dc, ==, 0);

                auto& __attribute__((unused)) i3 __attribute__((unused)) =
                mm.create<TMMItemB>(cc, dc);
                TEST_ASSERT_OP(cc, ==, 3);
                TEST_ASSERT_OP(dc, ==, 0);
            }

            TEST_ASSERT_OP(cc, ==, 3);
            TEST_ASSERT_OP(dc, ==, 0);

            mm.refresh();
        }

        TEST_ASSERT_OP(cc, ==, 3);
        TEST_ASSERT_OP(dc, ==, 3);

        cc = dc = 0;

        {
            TEST_ASSERT_OP(cc, ==, 0);
            TEST_ASSERT_OP(dc, ==, 0);

            ssvu::PolyManager<TMMItem> mm;

            {
                auto& __attribute__((unused)) i1 __attribute__((unused)) =
                mm.create<TMMItemS>(cc, dc);
                TEST_ASSERT_OP(cc, ==, 1);
                TEST_ASSERT_OP(dc, ==, 0);

                auto& __attribute__((unused)) i2 __attribute__((unused)) =
                mm.create<TMMItemB>(cc, dc);
                TEST_ASSERT_OP(cc, ==, 2);
                TEST_ASSERT_OP(dc, ==, 0);

                auto& __attribute__((unused)) i3 __attribute__((unused)) =
                mm.create<TMMItemB>(cc, dc);
                TEST_ASSERT_OP(cc, ==, 3);
                TEST_ASSERT_OP(dc, ==, 0);
            }

            TEST_ASSERT_OP(cc, ==, 3);
            TEST_ASSERT_OP(dc, ==, 0);

            mm.refresh();

            for(auto& i : mm) mm.del(*i);

            TEST_ASSERT_OP(cc, ==, 3);
            TEST_ASSERT_OP(dc, ==, 0);

            mm.refresh();

            TEST_ASSERT_OP(cc, ==, 3);
            TEST_ASSERT_OP(dc, ==, 3);
        }

        TEST_ASSERT_OP(cc, ==, 3);
        TEST_ASSERT_OP(dc, ==, 3);

        cc = dc = 0;

        {
            TEST_ASSERT_OP(cc, ==, 0);
            TEST_ASSERT_OP(dc, ==, 0);

            ssvu::PolyManager<TMMItem> mm;

            auto& i1 = mm.create<TMMItemS>(cc, dc);
            TEST_ASSERT_OP(cc, ==, 1);
            TEST_ASSERT_OP(dc, ==, 0);
            TEST_ASSERT_OP(mm.size(), ==, 0);

            auto& i2 = mm.create<TMMItemB>(cc, dc);
            TEST_ASSERT_OP(cc, ==, 2);
            TEST_ASSERT_OP(dc, ==, 0);
            TEST_ASSERT_OP(mm.size(), ==, 0);

            auto& __attribute__((unused)) i3 __attribute__((unused)) =
            mm.create<TMMItemB>(cc, dc);
            TEST_ASSERT_OP(cc, ==, 3);
            TEST_ASSERT_OP(dc, ==, 0);
            TEST_ASSERT_OP(mm.size(), ==, 0);

            TEST_ASSERT_OP(cc, ==, 3);
            TEST_ASSERT_OP(dc, ==, 0);
            TEST_ASSERT_OP(mm.size(), ==, 0);

            mm.refresh();
            TEST_ASSERT_OP(mm.size(), ==, 3);

            mm.del(i2);

            TEST_ASSERT_OP(cc, ==, 3);
            TEST_ASSERT_OP(dc, ==, 0);
            TEST_ASSERT_OP(mm.size(), ==, 3);

            mm.refresh();

            TEST_ASSERT_OP(cc, ==, 3);
            TEST_ASSERT_OP(dc, ==, 1);
            TEST_ASSERT_OP(mm.size(), ==, 2);

            mm.refresh();

            TEST_ASSERT_OP(cc, ==, 3);
            TEST_ASSERT_OP(dc, ==, 1);
            TEST_ASSERT_OP(mm.size(), ==, 2);

            mm.del(i1);

            TEST_ASSERT_OP(cc, ==, 3);
            TEST_ASSERT_OP(dc, ==, 1);
            TEST_ASSERT_OP(mm.size(), ==, 2);

            mm.refresh();

            TEST_ASSERT_OP(cc, ==, 3);
            TEST_ASSERT_OP(dc, ==, 2);
            TEST_ASSERT_OP(mm.size(), ==, 1);

            mm.create<TMMItemS>(cc, dc);
            mm.create<TMMItemS>(cc, dc);
            auto& i4 = mm.create<TMMItemB>(cc, dc);
            mm.create<TMMItemB>(cc, dc);

            TEST_ASSERT_OP(cc, ==, 7);
            TEST_ASSERT_OP(dc, ==, 2);
            TEST_ASSERT_OP(mm.size(), ==, 1);

            mm.refresh();

            TEST_ASSERT_OP(cc, ==, 7);
            TEST_ASSERT_OP(dc, ==, 2);
            TEST_ASSERT_OP(mm.size(), ==, 5);

            mm.refresh();
            TEST_ASSERT_OP(mm.size(), ==, 5);

            mm.del(i4);
            TEST_ASSERT_OP(mm.size(), ==, 5);

            TEST_ASSERT_OP(cc, ==, 7);
            TEST_ASSERT_OP(dc, ==, 2);
            TEST_ASSERT_OP(mm.size(), ==, 5);

            mm.refresh();

            TEST_ASSERT_OP(cc, ==, 7);
            TEST_ASSERT_OP(dc, ==, 3);
            TEST_ASSERT_OP(mm.size(), ==, 4);
        }

        TEST_ASSERT_OP(cc, ==, 7);
        TEST_ASSERT_OP(dc, ==, 7);
    }
}