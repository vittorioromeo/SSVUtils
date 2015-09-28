// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"

#include "SSVUtils/Bimap/Bimap.hpp"

#include "./utils/test_utils.hpp"
int main()
{
    ssvu::Bimap<int, std::string> sb;

    TEST_ASSERT(sb.empty());
    TEST_ASSERT(!sb.has(10));
    TEST_ASSERT(!sb.has("banana"));
    TEST_ASSERT(sb.count(10) == 0);
    TEST_ASSERT(sb.count("banana") == 0);

    sb.emplace(10, "banana");

    TEST_ASSERT(!sb.empty());
    TEST_ASSERT(sb.size() == 1);
    TEST_ASSERT(sb.has(10));
    TEST_ASSERT(sb.has("banana"));
    TEST_ASSERT(sb.count(10) == 1);
    TEST_ASSERT(sb.count("banana") == 1);
    TEST_ASSERT(sb.at(10) == "banana");
    TEST_ASSERT(sb[10] == "banana");
    TEST_ASSERT(sb.at("banana") == 10);
    TEST_ASSERT(sb["banana"] == 10);

    sb["banana"] = 25;

    TEST_ASSERT(!sb.empty());
    TEST_ASSERT(sb.size() == 1);
    TEST_ASSERT(!sb.has(10));
    TEST_ASSERT(sb.has(25));
    TEST_ASSERT(sb.has("banana"));
    TEST_ASSERT(sb.count(10) == 0);
    TEST_ASSERT(sb.count(25) == 1);
    TEST_ASSERT(sb.count("banana") == 1);
    TEST_ASSERT(sb.at(25) == "banana");
    TEST_ASSERT(sb[25] == "banana");
    TEST_ASSERT(sb.at("banana") == 25);
    TEST_ASSERT(sb["banana"] == 25);

    sb["banana"] = 15;
    sb[15] = "melon";
    sb.emplace(10, "cucumber");

    TEST_ASSERT(!sb.empty());
    TEST_ASSERT(sb.size() == 2);
    TEST_ASSERT(sb.has(10));
    TEST_ASSERT(sb.has(15));
    TEST_ASSERT(!sb.has(25));
    TEST_ASSERT(sb.has("melon"));
    TEST_ASSERT(sb.has("cucumber"));
    TEST_ASSERT(!sb.has("banana"));
    TEST_ASSERT(sb.count(10) == 1);
    TEST_ASSERT(sb.count(15) == 1);
    TEST_ASSERT(sb.count(25) == 0);
    TEST_ASSERT(sb.count("melon") == 1);
    TEST_ASSERT(sb.count("cucumber") == 1);
    TEST_ASSERT(sb.count("banana") == 0);
    TEST_ASSERT(sb.at(10) == "cucumber");
    TEST_ASSERT(sb[10] == "cucumber");
    TEST_ASSERT(sb.at("cucumber") == 10);
    TEST_ASSERT(sb["cucumber"] == 10);
    TEST_ASSERT(sb.at(15) == "melon");
    TEST_ASSERT(sb[15] == "melon");
    TEST_ASSERT(sb.at("melon") == 15);
    TEST_ASSERT(sb["melon"] == 15);

    sb.clear();

    TEST_ASSERT(sb.empty());
    TEST_ASSERT(sb.size() == 0);
    TEST_ASSERT(!sb.has(10));
    TEST_ASSERT(!sb.has(15));
    TEST_ASSERT(!sb.has(25));
    TEST_ASSERT(!sb.has("melon"));
    TEST_ASSERT(!sb.has("cucumber"));
    TEST_ASSERT(!sb.has("banana"));

    sb["yolo"] = 15;

    TEST_ASSERT(!sb.empty());
    TEST_ASSERT(sb.size() == 1);
    TEST_ASSERT(!sb.has(10));
    TEST_ASSERT(sb.has(15));
    TEST_ASSERT(!sb.has("melon"));
    TEST_ASSERT(sb.has("yolo"));
    TEST_ASSERT(sb["yolo"] == 15);

    sb["yolo"] = 25;

    TEST_ASSERT(sb.has("yolo"));
    TEST_ASSERT(!sb.has(15));
    TEST_ASSERT(sb.has(25));
    TEST_ASSERT(sb["yolo"] == 25);

    sb.erase("yolo");

    TEST_ASSERT(!sb.has("yolo"));
    TEST_ASSERT(!sb.has(15));
    TEST_ASSERT(!sb.has(25));
    TEST_ASSERT(sb.empty());
}
