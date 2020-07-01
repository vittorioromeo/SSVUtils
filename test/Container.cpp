// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"

#include "SSVUtils/Container/Container.hpp"

#include "./utils/test_utils.hpp"

#include <vector>

int main()
{
    {
        using namespace ssvu;

        VecMap<int, int> tm;

        TEST_ASSERT(tm.empty());
        TEST_ASSERT(tm.size() == 0);

        tm.reserve(3);

        TEST_ASSERT(tm.empty());
        TEST_ASSERT(tm.size() == 0);

        tm[0] = 0;
        tm[1] = 2;
        tm[2] = 4;

        TEST_ASSERT(!tm.empty());
        TEST_ASSERT(tm.size() == 3);

        TEST_ASSERT(tm[0] == 0 && tm.at(0) == 0);
        TEST_ASSERT(tm[1] == 2 && tm.at(1) == 2);
        TEST_ASSERT(tm[2] == 4 && tm.at(2) == 4);

        auto tmc(tm);

        TEST_ASSERT(tm == tmc);

        VecMap<int, int> tmil{{0, 0}, {1, 2}, {2, 4}};

        TEST_ASSERT(tm == tmil);

        auto tm2(std::move(tm));

        TEST_ASSERT(tm2[0] == 0 && tm2.at(0) == 0);
        TEST_ASSERT(tm2[1] == 2 && tm2.at(1) == 2);
        TEST_ASSERT(tm2[2] == 4 && tm2.at(2) == 4);

        tm2[0] = 5;
        TEST_ASSERT(tm2[0] == 5 && tm2.at(0) == 5);
    }
    {
        using namespace ssvu;

        VecMap<std::string, std::string> tm;

        TEST_ASSERT(tm.empty());
        TEST_ASSERT(tm.size() == 0);

        tm.reserve(3);

        TEST_ASSERT(tm.empty());
        TEST_ASSERT(tm.size() == 0);

        std::vector<std::string> words{
            "a", "klab", "eacbds", "haadfopja", "bdasaa", "aasdfpoasfas"};

        for(const auto& w : words) tm[w] = w + "val";

        TEST_ASSERT(!tm.empty());
        TEST_ASSERT(tm.size() == words.size());

        for(const auto& w : words)
        {
            TEST_ASSERT_OP(tm[w], ==, w + "val");
        }

        TEST_ASSERT_OP(tm.atOrDefault("banana"), ==, "");

        const auto& tv(tm["a"]);
        TEST_ASSERT(tv == "aval");

        tm.clear();

        TEST_ASSERT(tm.empty());
        TEST_ASSERT(tm.size() == 0);
    }
    {
        using namespace ssvu;

        VecSorted<std::string> vs;

        TEST_ASSERT(vs.empty());
        TEST_ASSERT(vs.size() == 0);

        vs.reserve(5);

        TEST_ASSERT(vs.empty());
        TEST_ASSERT(vs.size() == 0);
        {
            std::vector<std::string> words{
                "a", "klab", "eacbds", "haadfopja", "bdasaa", "aasdfpoasfas"};

            for(const auto& w : words) vs.insert(w);

            sort(words);

            for(auto i(0u); i < words.size(); ++i)
            {
                TEST_ASSERT_OP(words[i], ==, vs[i]);
            }

            vs.clear();

            TEST_ASSERT(vs.empty());
            TEST_ASSERT(vs.size() == 0);
        }
    }
}
