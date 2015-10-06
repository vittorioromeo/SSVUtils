// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"


#include "./utils/test_utils.hpp"
int main()
{
    using namespace std;
    using namespace ssvu;
    string s{"test abc string abc"};

    TEST_ASSERT(toStr(100) == "100");
    TEST_ASSERT(toStr(100.f) == "100");
    TEST_ASSERT(getReplaced(s, "test", "banana") == "banana abc string abc");
    TEST_ASSERT(getReplacedAll(s, "abc", "cba") == "test cba string cba");
    TEST_ASSERT(beginsWith(s, "test") == true);
    TEST_ASSERT(beginsWith(s, 't') == true);
    TEST_ASSERT(beginsWith(s, "testa") == false);
    TEST_ASSERT(beginsWith(s, 'k') == false);
    TEST_ASSERT(endsWith(s, "abc") == true);
    TEST_ASSERT(endsWith(s, 'c') == true);
    TEST_ASSERT(endsWith(s, "cba") == false);
    TEST_ASSERT(endsWith(s, 'a') == false);
    TEST_ASSERT(toLower("AAA") == "aaa");
    TEST_ASSERT(getCharCount("AAA", 'A') == 3);
    TEST_ASSERT(getDistLevenshtein("AAA", "AAB") == 1);
    TEST_ASSERT(getDistLevenshtein("AAA", "ABB") == 2);
    TEST_ASSERT(getDistLevenshtein("AAA", "BBB") == 3);
    TEST_ASSERT(getDistLevenshtein("AAA", "BBBB") == 4);
    TEST_ASSERT(getDistLevenshtein("AAA", "BBC") == 3);

    TEST_ASSERT(getTrimStrL("   AAA") == "AAA");
    TEST_ASSERT(getTrimStrL("   AA  A") == "AA  A");
    TEST_ASSERT(getTrimStrL("A A A") == "A A A");
    TEST_ASSERT(getTrimStrL("  A A A") == "A A A");

    TEST_ASSERT(getTrimStrR("AAA   ") == "AAA");
    TEST_ASSERT(getTrimStrR("AA  A   ") == "AA  A");
    TEST_ASSERT(getTrimStrR("A A A") == "A A A");
    TEST_ASSERT(getTrimStrR("A A A  ") == "A A A");

    TEST_ASSERT(getTrimStrLR("   A A A  ") == "A A A");
    TEST_ASSERT(getTrimStrLR("      AaA  ") == "AaA");

    TEST_ASSERT(isDigit('0'));
    TEST_ASSERT(isDigit('1'));
    TEST_ASSERT(isDigit('2'));
    TEST_ASSERT(isDigit('3'));
    TEST_ASSERT(isDigit('4'));
    TEST_ASSERT(isDigit('5'));
    TEST_ASSERT(isDigit('6'));
    TEST_ASSERT(isDigit('7'));
    TEST_ASSERT(isDigit('8'));
    TEST_ASSERT(isDigit('9'));

    TEST_ASSERT(isUppercase('A'));
    TEST_ASSERT(isUppercase('B'));
    TEST_ASSERT(isUppercase('C'));

    TEST_ASSERT(isLowercase('a'));
    TEST_ASSERT(isLowercase('b'));
    TEST_ASSERT(isLowercase('c'));

    TEST_ASSERT(isControl('\n'));
    TEST_ASSERT(isBlank(' '));
    TEST_ASSERT(isSpace(' '));
    TEST_ASSERT(isSpace('\t'));

    TEST_ASSERT(isPunctuation('.'));
    TEST_ASSERT(isPunctuation(','));
    TEST_ASSERT(isPunctuation('!'));

    // Fast integer tests
    for(auto i(0u); i < 100; ++i)
    {
        auto is = ssvu::getRndI<int, int>(
            NumLimits<int>::min(), NumLimits<int>::max());
        auto iu = ssvu::getRndI<unsigned int, unsigned int>(
            NumLimits<unsigned int>::min(), NumLimits<unsigned int>::max());

        TEST_ASSERT_OP(ssvu::toStr(is), ==, std::to_string(is));
        TEST_ASSERT_OP(ssvu::toStr(iu), ==, std::to_string(iu));
    }
}
