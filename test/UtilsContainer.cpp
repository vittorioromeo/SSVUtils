// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"


#include "./utils/test_utils.hpp"

int main()
{
    {
        using namespace std;
        using namespace ssvu;

        std::vector<string> vec{"abc", "bcd", "efg", "ghi"};
        TEST_ASSERT(ssvu::find(vec, "abc") == std::begin(vec));
        TEST_ASSERT(ssvu::find(vec, "wut") == std::end(vec));
        TEST_ASSERT(ssvu::idxOf(vec, "efg") == 2);
        TEST_ASSERT(ssvu::findIf(vec, [](const string& s)
                                 {
                                     return beginsWith(s, "gh");
                                 }) == std::begin(vec) + 3);
        TEST_ASSERT(ssvu::contains(vec, "bcd") == true);
        TEST_ASSERT(ssvu::contains(vec, "bcdnbdf") == false);
        TEST_ASSERT(ssvu::containsAnyIf(vec, [](const string& s)
                                        {
                                            return beginsWith(s, "gh");
                                        }) == true);

        ssvu::eraseRemoveIf(vec, [](const string& s)
                            {
                                return beginsWith(s, "gh");
                            });
        TEST_ASSERT(vec.size() == 3);

        std::map<string, int> m{{"abc", 0}, {"bcd", 1}, {"def", 2}, {"efg", 3}};
        TEST_ASSERT(ssvu::getKeys(m)[2] == "def");
    }

    {
        using namespace ssvu;

        {
            auto vecInt(mkVector(0, 1, 2, 3, 4, 5));
            SSVU_ASSERT_STATIC_NM(isSame<decltype(vecInt), std::vector<int>>());
            TEST_ASSERT_OP(vecInt.size(), ==, 6);
        }

        {
            auto vecFloat(mkVector(0, 1, 2, 3, 4, 5.f));
            SSVU_ASSERT_STATIC_NM(
                isSame<decltype(vecFloat), std::vector<float>>());
            TEST_ASSERT_OP(vecFloat.size(), ==, 6);
        }

        {
            auto vecDouble(mkVector(0., 1, 2.f, 3, 4, 5));
            SSVU_ASSERT_STATIC_NM(
                isSame<decltype(vecDouble), std::vector<double>>());
            TEST_ASSERT_OP(vecDouble.size(), ==, 6);
        }

        {
            auto umIntInt(mkUnorderedMap(0, 0, 1, 1, 2, 2));
            SSVU_ASSERT_STATIC_NM(
                isSame<decltype(umIntInt), std::unordered_map<int, int>>());
            TEST_ASSERT_OP(umIntInt.size(), ==, 3);
        }

        {
            auto umIntFloat(mkUnorderedMap(0, 0, 1, 1, 2, 2.f));
            SSVU_ASSERT_STATIC_NM(
                isSame<decltype(umIntFloat), std::unordered_map<int, float>>());
            TEST_ASSERT_OP(umIntFloat.size(), ==, 3);
        }

        {
            auto umStrInt(mkUnorderedMap("0", 0, "1", 1, std::string{"2"}, 2));
            SSVU_ASSERT_STATIC_NM(
                isSame<decltype(umStrInt),
                       std::unordered_map<std::string, int>>());
            TEST_ASSERT_OP(umStrInt.size(), ==, 3);
        }
    }
}