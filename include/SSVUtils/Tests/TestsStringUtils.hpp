// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSSTRINGUTILS
#define SSVU_TESTS_TESTSSTRINGUTILS

SSVUT_TEST(StringUtilsTests)
{
	using namespace std;
	using namespace ssvu;
	string s{"test abc string abc"};

	SSVUT_EXPECT(toStr(100) == "100");
	SSVUT_EXPECT(toStr(100.f) == "100");
	SSVUT_EXPECT(getReplaced(s, "test", "banana") == "banana abc string abc");
	SSVUT_EXPECT(getReplacedAll(s, "abc", "cba") == "test cba string cba");
	SSVUT_EXPECT(beginsWith(s, "test") == true);
	SSVUT_EXPECT(beginsWith(s, 't') == true);
	SSVUT_EXPECT(beginsWith(s, "testa") == false);
	SSVUT_EXPECT(beginsWith(s, 'k') == false);
	SSVUT_EXPECT(endsWith(s, "abc") == true);
	SSVUT_EXPECT(endsWith(s, 'c') == true);
	SSVUT_EXPECT(endsWith(s, "cba") == false);
	SSVUT_EXPECT(endsWith(s, 'a') == false);
	SSVUT_EXPECT(toLower("AAA") == "aaa");
	SSVUT_EXPECT(getCharCount("AAA", 'A') == 3);
	SSVUT_EXPECT(getDistLevenshtein("AAA", "AAB") == 1);
	SSVUT_EXPECT(getDistLevenshtein("AAA", "ABB") == 2);
	SSVUT_EXPECT(getDistLevenshtein("AAA", "BBB") == 3);
	SSVUT_EXPECT(getDistLevenshtein("AAA", "BBBB") == 4);
	SSVUT_EXPECT(getDistLevenshtein("AAA", "BBC") == 3);

	SSVUT_EXPECT(getTrimStrL("   AAA") == "AAA");
	SSVUT_EXPECT(getTrimStrL("   AA  A") == "AA  A");
	SSVUT_EXPECT(getTrimStrL("A A A") == "A A A");
	SSVUT_EXPECT(getTrimStrL("  A A A") == "A A A");

	SSVUT_EXPECT(getTrimStrR("AAA   ") == "AAA");
	SSVUT_EXPECT(getTrimStrR("AA  A   ") == "AA  A");
	SSVUT_EXPECT(getTrimStrR("A A A") == "A A A");
	SSVUT_EXPECT(getTrimStrR("A A A  ") == "A A A");

	SSVUT_EXPECT(getTrimStrLR("   A A A  ") == "A A A");
	SSVUT_EXPECT(getTrimStrLR("      AaA  ") == "AaA");

	SSVUT_EXPECT(isDigit('0'));
	SSVUT_EXPECT(isDigit('1'));
	SSVUT_EXPECT(isDigit('2'));
	SSVUT_EXPECT(isDigit('3'));
	SSVUT_EXPECT(isDigit('4'));
	SSVUT_EXPECT(isDigit('5'));
	SSVUT_EXPECT(isDigit('6'));
	SSVUT_EXPECT(isDigit('7'));
	SSVUT_EXPECT(isDigit('8'));
	SSVUT_EXPECT(isDigit('9'));

	SSVUT_EXPECT(isUppercase('A'));
	SSVUT_EXPECT(isUppercase('B'));
	SSVUT_EXPECT(isUppercase('C'));

	SSVUT_EXPECT(isLowercase('a'));
	SSVUT_EXPECT(isLowercase('b'));
	SSVUT_EXPECT(isLowercase('c'));

	SSVUT_EXPECT(isControl('\n'));
	SSVUT_EXPECT(isBlank(' '));
	SSVUT_EXPECT(isSpace(' '));
	SSVUT_EXPECT(isSpace('\t'));

	SSVUT_EXPECT(isPunctuation('.'));
	SSVUT_EXPECT(isPunctuation(','));
	SSVUT_EXPECT(isPunctuation('!'));
}

#endif
