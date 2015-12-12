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

        OTest(const OTest&) = default;
        OTest& operator=(const OTest&) = default;
    };

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
}
