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

    // Empty, one element
    {
        HandleVector<OTest> mgr;
        for(int k = 0; k < 2; ++k)
        {
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
}
