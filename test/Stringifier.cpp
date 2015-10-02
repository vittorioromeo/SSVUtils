// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <list>
#include <forward_list>
#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Bimap/Bimap.hpp"


#include "./utils/test_utils.hpp"
int main()
{
    auto nothing([]
    {
    });
    int testArray[]{1, 2, 3};
    std::ostringstream trash;
#define SSVUT_STRINGIFY_TEST(mValue)      \
    {                                     \
        auto k(mValue);                   \
        ssvu::stringify<true>(trash, k);  \
        ssvu::stringify<false>(trash, k); \
    }                                     \
    nothing()

    SSVUT_STRINGIFY_TEST(0);
    SSVUT_STRINGIFY_TEST(0l);
    SSVUT_STRINGIFY_TEST(0u);
    SSVUT_STRINGIFY_TEST(0ul);
    SSVUT_STRINGIFY_TEST(0.f);
    SSVUT_STRINGIFY_TEST(0.);
    SSVUT_STRINGIFY_TEST(&trash);
    SSVUT_STRINGIFY_TEST("abc");
    SSVUT_STRINGIFY_TEST("abc"s);
    ssvu::stringify<true>(trash, testArray);
    ssvu::stringify<false>(trash, testArray);
    SSVUT_STRINGIFY_TEST(__R(std::array<int, 3>{{1, 2, 3}}));
    SSVUT_STRINGIFY_TEST(__R(std::vector<int>{1, 2, 3}));
    SSVUT_STRINGIFY_TEST(__R(std::list<int>{1, 2, 3}));
    SSVUT_STRINGIFY_TEST(__R(std::forward_list<int>{1, 2, 3}));
    SSVUT_STRINGIFY_TEST(
    __R(std::map<int, std::string>{{1, "aa"}, {2, "bb"}, {3, "cc"}}));
    SSVUT_STRINGIFY_TEST(
    __R(std::unordered_map<int, std::string>{{1, "aa"}, {2, "bb"}, {3, "cc"}}));
    SSVUT_STRINGIFY_TEST(
    __R(ssvu::Bimap<int, std::string>{{1, "aa"}, {2, "bb"}, {3, "cc"}}));
    SSVUT_STRINGIFY_TEST(__R(ssvu::Tpl<int, int, int>{1, 2, 3}));
    SSVUT_STRINGIFY_TEST(__R(std::pair<int, int>{2, 3}));

#undef SSVUT_STRINGIFY_TEST
}
