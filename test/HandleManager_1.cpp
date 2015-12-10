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

  
    // All alive -> all dead -> all alive
    {
        HandleVector<OTest> mgr;
        for(int k = 0; k < 2; ++k)
        {
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

}
