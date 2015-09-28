// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"

#include "SSVUtils/HandleVector/HandleVector.hpp"

#include "./utils/test_utils.hpp"
int main()
{
    using namespace ssvu;
    using namespace std;

    int cc{0}, dd{0};

    struct OTest
    {
        std::string s;
        int& rCC;
        int& rDD;

        OTest(int& mRCC, int& mRDD) : rCC(mRCC), rDD(mRDD) { ++rCC; }
        ~OTest() { ++rDD; }
    };

    // Mixed elements
    {
        HandleVector<OTest> mgr;
        for(int k = 0; k < 2; ++k) {
            cc = dd = 0;

            auto a0(mgr.create(cc, dd));
            auto a1(mgr.create(cc, dd));
            auto a2(mgr.create(cc, dd));
            auto a3(mgr.create(cc, dd));
            auto a4(mgr.create(cc, dd));
            auto a5(mgr.create(cc, dd));
            auto a6(mgr.create(cc, dd));

            TEST_ASSERT(cc == 7);
            TEST_ASSERT(dd == 0);
            TEST_ASSERT(mgr.getSize() == 0);
            TEST_ASSERT(mgr.getSizeNext() == 7);

            mgr.refresh();

            TEST_ASSERT(cc == 7);
            TEST_ASSERT(dd == 0);
            TEST_ASSERT(mgr.getSize() == 7);
            TEST_ASSERT(mgr.getSizeNext() == 7);

            a0->s = "hi";
            a4->s = "ciao";
            a6->s = "bye";

            a2.destroy();
            a3.destroy();
            a5.destroy();

            TEST_ASSERT(cc == 7);
            TEST_ASSERT(dd == 0);
            TEST_ASSERT(mgr.getSize() == 7);
            TEST_ASSERT(mgr.getSizeNext() == 7);

            mgr.refresh();

            TEST_ASSERT_OP(cc, ==, 7);
            TEST_ASSERT_OP(dd, ==, 3);
            TEST_ASSERT(mgr.getSize() == 4);
            TEST_ASSERT(mgr.getSizeNext() == 4);

            TEST_ASSERT(a0->s == "hi");
            TEST_ASSERT(a4->s == "ciao");
            TEST_ASSERT(a6->s == "bye");

            TEST_ASSERT(a0.isAlive());
            TEST_ASSERT(a1.isAlive());
            TEST_ASSERT(!a2.isAlive());
            TEST_ASSERT(!a3.isAlive());
            TEST_ASSERT(a4.isAlive());
            TEST_ASSERT(!a5.isAlive());
            TEST_ASSERT(a6.isAlive());

            mgr.forEach([](OTest& mA)
                        {
                            mA.s += "bb";
                        });

            TEST_ASSERT(a0->s == "hibb");
            TEST_ASSERT(a4->s == "ciaobb");
            TEST_ASSERT(a6->s == "byebb");

            TEST_ASSERT(a0.isAlive());
            TEST_ASSERT(a1.isAlive());
            TEST_ASSERT(!a2.isAlive());
            TEST_ASSERT(!a3.isAlive());
            TEST_ASSERT(a4.isAlive());
            TEST_ASSERT(!a5.isAlive());
            TEST_ASSERT(a6.isAlive());

            auto aNew(mgr.create(cc, dd));
            aNew->s = "hehe";

            TEST_ASSERT(cc == 8);
            TEST_ASSERT(dd == 3);
            TEST_ASSERT(mgr.getSize() == 4);
            TEST_ASSERT(mgr.getSizeNext() == 5);

            mgr.refresh();

            TEST_ASSERT(cc == 8);
            TEST_ASSERT(dd == 3);
            TEST_ASSERT(mgr.getSize() == 5);
            TEST_ASSERT(mgr.getSizeNext() == 5);

            TEST_ASSERT(a0.isAlive());
            TEST_ASSERT(a1.isAlive());
            TEST_ASSERT(!a2.isAlive());
            TEST_ASSERT(!a3.isAlive());
            TEST_ASSERT(a4.isAlive());
            TEST_ASSERT(!a5.isAlive());
            TEST_ASSERT(a6.isAlive());
            TEST_ASSERT(aNew.isAlive());

            TEST_ASSERT(aNew->s == "hehe");

            a0.destroy();
            mgr.refresh();

            TEST_ASSERT(!a0.isAlive());
            TEST_ASSERT(a1.isAlive());
            TEST_ASSERT(!a2.isAlive());
            TEST_ASSERT(!a3.isAlive());
            TEST_ASSERT(a4.isAlive());
            TEST_ASSERT(!a5.isAlive());
            TEST_ASSERT(a6.isAlive());
            TEST_ASSERT(aNew.isAlive());

            TEST_ASSERT(cc == 8);
            TEST_ASSERT(dd == 4);
            TEST_ASSERT(mgr.getSize() == 4);
            TEST_ASSERT(mgr.getSizeNext() == 4);

            auto aSuicide(mgr.create(cc, dd));

            TEST_ASSERT(cc == 9);
            TEST_ASSERT(dd == 4);
            TEST_ASSERT(mgr.getSize() == 4);
            TEST_ASSERT(mgr.getSizeNext() == 5);

            aSuicide.destroy();

            TEST_ASSERT(cc == 9);
            TEST_ASSERT(dd == 4);
            TEST_ASSERT(mgr.getSize() == 4);
            TEST_ASSERT(mgr.getSizeNext() == 5);

            mgr.refresh();

            TEST_ASSERT(cc == 9);
            TEST_ASSERT(dd == 5);
            TEST_ASSERT(mgr.getSize() == 4);
            TEST_ASSERT(mgr.getSizeNext() == 4);

            mgr.clear();

            TEST_ASSERT(cc == 9);
            TEST_ASSERT(dd == 9);
        }
    }

    // All alive -> all dead -> all alive
    {
        HandleVector<OTest> mgr;
        for(int k = 0; k < 2; ++k) {
            cc = dd = 0;

            auto a0(mgr.create(cc, dd));
            auto a1(mgr.create(cc, dd));
            auto a2(mgr.create(cc, dd));
            auto a3(mgr.create(cc, dd));

            TEST_ASSERT(cc == 4);
            TEST_ASSERT(dd == 0);
            TEST_ASSERT(mgr.getSize() == 0);
            TEST_ASSERT(mgr.getSizeNext() == 4);

            TEST_ASSERT(a0.isAlive());
            TEST_ASSERT(a1.isAlive());
            TEST_ASSERT(a2.isAlive());
            TEST_ASSERT(a3.isAlive());

            mgr.refresh();

            TEST_ASSERT(cc == 4);
            TEST_ASSERT(dd == 0);
            TEST_ASSERT(mgr.getSize() == 4);
            TEST_ASSERT(mgr.getSizeNext() == 4);

            TEST_ASSERT(a0.isAlive());
            TEST_ASSERT(a1.isAlive());
            TEST_ASSERT(a2.isAlive());
            TEST_ASSERT(a3.isAlive());

            a0.destroy();
            a1.destroy();
            a2.destroy();
            a3.destroy();

            TEST_ASSERT(a0.isAlive());
            TEST_ASSERT(a1.isAlive());
            TEST_ASSERT(a2.isAlive());
            TEST_ASSERT(a3.isAlive());

            mgr.refresh();

            TEST_ASSERT(cc == 4);
            TEST_ASSERT(dd == 4);
            TEST_ASSERT(mgr.getSize() == 0);
            TEST_ASSERT(mgr.getSizeNext() == 0);

            TEST_ASSERT(!a0.isAlive());
            TEST_ASSERT(!a1.isAlive());
            TEST_ASSERT(!a2.isAlive());
            TEST_ASSERT(!a3.isAlive());

            mgr.refresh();

            TEST_ASSERT(cc == 4);
            TEST_ASSERT(dd == 4);
            TEST_ASSERT(mgr.getSize() == 0);
            TEST_ASSERT(mgr.getSizeNext() == 0);

            a0 = mgr.create(cc, dd);
            a1 = mgr.create(cc, dd);
            a2 = mgr.create(cc, dd);
            a3 = mgr.create(cc, dd);

            TEST_ASSERT(a0.isAlive());
            TEST_ASSERT(a1.isAlive());
            TEST_ASSERT(a2.isAlive());
            TEST_ASSERT(a3.isAlive());

            TEST_ASSERT(cc == 8);
            TEST_ASSERT(dd == 4);
            TEST_ASSERT(mgr.getSize() == 0);
            TEST_ASSERT(mgr.getSizeNext() == 4);

            mgr.refresh();

            TEST_ASSERT(a0.isAlive());
            TEST_ASSERT(a1.isAlive());
            TEST_ASSERT(a2.isAlive());
            TEST_ASSERT(a3.isAlive());

            TEST_ASSERT(cc == 8);
            TEST_ASSERT(dd == 4);
            TEST_ASSERT(mgr.getSize() == 4);
            TEST_ASSERT(mgr.getSizeNext() == 4);

            mgr.clear();

            TEST_ASSERT(!a0.isAlive());
            TEST_ASSERT(!a1.isAlive());
            TEST_ASSERT(!a2.isAlive());
            TEST_ASSERT(!a3.isAlive());

            TEST_ASSERT(cc == 8);
            TEST_ASSERT(dd == 8);
            TEST_ASSERT(mgr.getSize() == 0);
            TEST_ASSERT(mgr.getSizeNext() == 0);
        }
    }

    // Empty, one element
    {
        HandleVector<OTest> mgr;
        for(int k = 0; k < 2; ++k) {
            cc = dd = 0;

            TEST_ASSERT(cc == 0);
            TEST_ASSERT(dd == 0);
            TEST_ASSERT(mgr.getSize() == 0);
            TEST_ASSERT(mgr.getSizeNext() == 0);

            mgr.refresh();

            TEST_ASSERT(cc == 0);
            TEST_ASSERT(dd == 0);
            TEST_ASSERT(mgr.getSize() == 0);
            TEST_ASSERT(mgr.getSizeNext() == 0);

            auto a0(mgr.create(cc, dd));

            TEST_ASSERT(a0.isAlive());

            TEST_ASSERT(cc == 1);
            TEST_ASSERT(dd == 0);
            TEST_ASSERT(mgr.getSize() == 0);
            TEST_ASSERT(mgr.getSizeNext() == 1);

            mgr.refresh();

            TEST_ASSERT(a0.isAlive());

            TEST_ASSERT(cc == 1);
            TEST_ASSERT(dd == 0);
            TEST_ASSERT(mgr.getSize() == 1);
            TEST_ASSERT(mgr.getSizeNext() == 1);

            a0.destroy();

            TEST_ASSERT(a0.isAlive());

            TEST_ASSERT(cc == 1);
            TEST_ASSERT(dd == 0);
            TEST_ASSERT(mgr.getSize() == 1);
            TEST_ASSERT(mgr.getSizeNext() == 1);

            mgr.refresh();

            TEST_ASSERT(!a0.isAlive());

            TEST_ASSERT(cc == 1);
            TEST_ASSERT(dd == 1);
            TEST_ASSERT(mgr.getSize() == 0);
            TEST_ASSERT(mgr.getSizeNext() == 0);

            mgr.clear();

            TEST_ASSERT(!a0.isAlive());

            TEST_ASSERT(cc == 1);
            TEST_ASSERT(dd == 1);
            TEST_ASSERT(mgr.getSize() == 0);
            TEST_ASSERT(mgr.getSizeNext() == 0);
        }
    }

    // Iterator tests
    {
        HandleVector<OTest> mgr;
        auto a0 = mgr.create(cc, dd);
        a0->s = "a";
        auto a1 = mgr.create(cc, dd);
        a1->s = "b";
        auto a2 = mgr.create(cc, dd);
        a2->s = "c";

        {
            std::string temp;
            for(auto a : mgr) temp += a.s;
            TEST_ASSERT(temp == "");
        }
        {
            std::string temp;
            for(auto& a : mgr) temp += a.s;
            TEST_ASSERT(temp == "");
        }
        {
            std::string temp;
            for(const auto& a : mgr) temp += a.s;
            TEST_ASSERT(temp == "");
        }
        {
            std::string temp;
            for(auto itr(mgr.beginIdx()); itr != mgr.endIdx(); ++itr)
                temp += itr->s;
            TEST_ASSERT(temp == "");
        }
        {
            std::string temp;
            for(auto itr(mgr.begin()); itr != mgr.endNext(); ++itr)
                temp += itr->s;
            TEST_ASSERT(temp == "abc");
        }
        {
            std::string temp;
            for(auto itr(mgr.beginIdx()); itr != mgr.endIdxNext(); ++itr)
                temp += itr->s;
            TEST_ASSERT(temp == "abc");
        }

        mgr.refresh();

        {
            std::string temp;
            for(auto a : mgr) temp += a.s;
            TEST_ASSERT(temp == "abc");
        }
        {
            std::string temp;
            for(auto& a : mgr) temp += a.s;
            TEST_ASSERT(temp == "abc");
        }
        {
            std::string temp;
            for(const auto& a : mgr) temp += a.s;
            TEST_ASSERT(temp == "abc");
        }
        {
            std::string temp;
            for(auto itr(mgr.beginIdx()); itr != mgr.endIdx(); ++itr)
                temp += itr->s;
            TEST_ASSERT(temp == "abc");
        }
        {
            std::string temp;
            for(auto itr(mgr.begin()); itr != mgr.endNext(); ++itr)
                temp += itr->s;
            TEST_ASSERT(temp == "abc");
        }
        {
            std::string temp;
            for(auto itr(mgr.beginIdx()); itr != mgr.endIdxNext(); ++itr)
                temp += itr->s;
            TEST_ASSERT(temp == "abc");
        }

        auto a3 = mgr.create(cc, dd);
        a3->s = "d";

        {
            std::string temp;
            for(auto a : mgr) temp += a.s;
            TEST_ASSERT(temp == "abc");
        }
        {
            std::string temp;
            for(auto& a : mgr) temp += a.s;
            TEST_ASSERT(temp == "abc");
        }
        {
            std::string temp;
            for(const auto& a : mgr) temp += a.s;
            TEST_ASSERT(temp == "abc");
        }
        {
            std::string temp;
            for(auto itr(mgr.beginIdx()); itr != mgr.endIdx(); ++itr)
                temp += itr->s;
            TEST_ASSERT(temp == "abc");
        }
        {
            std::string temp;
            for(auto itr(mgr.begin()); itr != mgr.endNext(); ++itr)
                temp += itr->s;
            TEST_ASSERT(temp == "abcd");
        }
        {
            std::string temp;
            for(auto itr(mgr.beginIdx()); itr != mgr.endIdxNext(); ++itr)
                temp += itr->s;
            TEST_ASSERT(temp == "abcd");
        }

        mgr.refresh();

        {
            std::string temp;
            for(auto a : mgr) temp += a.s;
            TEST_ASSERT(temp == "abcd");
        }
        {
            std::string temp;
            for(auto& a : mgr) temp += a.s;
            TEST_ASSERT(temp == "abcd");
        }
        {
            std::string temp;
            for(const auto& a : mgr) temp += a.s;
            TEST_ASSERT(temp == "abcd");
        }
        {
            std::string temp;
            for(auto itr(mgr.beginIdx()); itr != mgr.endIdx(); ++itr)
                temp += itr->s;
            TEST_ASSERT(temp == "abcd");
        }
        {
            std::string temp;
            for(auto itr(mgr.begin()); itr != mgr.endNext(); ++itr)
                temp += itr->s;
            TEST_ASSERT(temp == "abcd");
        }
        {
            std::string temp;
            for(auto itr(mgr.beginIdx()); itr != mgr.endIdxNext(); ++itr)
                temp += itr->s;
            TEST_ASSERT(temp == "abcd");
        }
    }

    // AtomFromData tests
    {
        HandleVector<OTest> mgr;
        auto a0 = mgr.create(cc, dd);
        a0->s = "a";
        auto a1 = mgr.create(cc, dd);
        a1->s = "b";
        auto a2 = mgr.create(cc, dd);
        a2->s = "c";
        auto& d0 = *a0;
        auto& d1 = *a1;
        auto& d2 = *a2;

        TEST_ASSERT(mgr.getAtomFromData(d0).getData().s == "a");
        TEST_ASSERT(mgr.getAtomFromData(d1).getData().s == "b");
        TEST_ASSERT(mgr.getAtomFromData(d2).getData().s == "c");

        mgr.refresh();

        TEST_ASSERT(mgr.getAtomFromData(d0).getData().s == "a");
        TEST_ASSERT(mgr.getAtomFromData(d1).getData().s == "b");
        TEST_ASSERT(mgr.getAtomFromData(d2).getData().s == "c");

        mgr.reserve(100);
        // After reserving pointers may now be invalid, we need to use handles

        TEST_ASSERT(mgr.getAtomFromHandle(a0).getData().s == "a");
        TEST_ASSERT(mgr.getAtomFromHandle(a1).getData().s == "b");
        TEST_ASSERT(mgr.getAtomFromHandle(a2).getData().s == "c");
    }
}
