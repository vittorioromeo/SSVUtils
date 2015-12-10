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
