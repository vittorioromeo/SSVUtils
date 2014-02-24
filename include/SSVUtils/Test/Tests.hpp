// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TEST_TESTS
#define SSVU_TEST_TESTS

#include <list>
#include <forward_list>
#include <map>
#include <unordered_map>
#include "SSVUtils/Test/Test.hpp"
#include "SSVUtils/Preprocessor/Preprocessor.hpp"

SSVU_TEST(UtilsMathTests)
{
	using namespace std;
	using namespace ssvu;

	for(int i{0}; i < 100; ++i)
	{
		auto r(getRnd<int>(-10, 10));
		SSVUT_EXPECT(r >= -10 && r < 10);

		// getRnd is [a, b)
		auto ru(getRnd<unsigned int>(1, 10));
		SSVUT_EXPECT(ru > 0 && ru < 10);

		// getRndR is [a, b]
		auto rf(getRndR<float>(0.f, 2.f));
		SSVUT_EXPECT(rf >= 0.f && rf <= 2.f);

		auto rs(getRndSign());
		SSVUT_EXPECT(rs == 1 || rs == -1);
	}

	SSVUT_EXPECT(getSign(-1) == -1);
	SSVUT_EXPECT(getSign(1) == 1);
	SSVUT_EXPECT(getSign(0) == 0);

	SSVUT_EXPECT(getSign(-1u) == 1);
	SSVUT_EXPECT(getSign(1u) == 1);
	SSVUT_EXPECT(getSign(0u) == 0);

	SSVUT_EXPECT(getSign(-1.5f) == -1);
	SSVUT_EXPECT(getSign(1.5f) == 1);
	SSVUT_EXPECT(getSign(0.5f) == 1);
	SSVUT_EXPECT(getSign(0.0f) == 0);

	SSVUT_EXPECT(getClampedMin(0, 100) == 100);
	SSVUT_EXPECT(getClampedMin(150, 100) == 150);
	SSVUT_EXPECT(getClampedMax(0, 100) == 0);
	SSVUT_EXPECT(getClampedMax(150, 100) == 100);
	SSVUT_EXPECT(getClamped(1000, 0, 500) == 500);
	SSVUT_EXPECT(getClamped(1000, 1500, 2500) == 1500);

	SSVUT_EXPECT(wrapDeg(720) == 0);
	SSVUT_EXPECT(wrapDeg(720 + 180) == 180);

	SSVUT_EXPECT(getSIMod(10, 9) == 1);
	SSVUT_EXPECT(getSIMod(-10, 9) == 8);
	SSVUT_EXPECT(getSIMod(10, -9) == 8);

	SSVUT_EXPECT(getWrapIdx(10, 5) == 0);
	SSVUT_EXPECT(getWrapIdx(10, 10) == 0);
	SSVUT_EXPECT(getWrapIdx(10, 15) == 10);
	SSVUT_EXPECT(getWrapIdx(4, 3) == 1);
	SSVUT_EXPECT(getWrapIdx(5, 3) == 2);
	SSVUT_EXPECT(getWrapIdx(6, 3) == 0);
	SSVUT_EXPECT(getWrapIdx(-1, 3) == 2);
	SSVUT_EXPECT(getWrapIdx(-2, 3) == 1);
	SSVUT_EXPECT(getWrapIdx(-3, 3) == 0);
	SSVUT_EXPECT(getWrapIdx(-4, 3) == 2);
}
SSVU_TEST_END();

SSVU_TEST(BimapTests)
{
	using namespace std;
	using namespace ssvu;

	{
		Bimap<string, int> bimap;
		bimap.emplace("hi", 10);
		SSVUT_EXPECT(bimap[10] == "hi");
		SSVUT_EXPECT(bimap["hi"] == 10);
		SSVUT_EXPECT(bimap.at(10) == "hi");
		SSVUT_EXPECT(bimap.at("hi") == 10);

		bimap.replace("hi", 1000);
		SSVUT_EXPECT(!bimap.has(10));
		SSVUT_EXPECT(bimap.has(1000));
		SSVUT_EXPECT(bimap.has("hi"));
		SSVUT_EXPECT(bimap["hi"] == 1000);
		SSVUT_EXPECT(bimap[1000] == "hi");
		SSVUT_EXPECT(bimap["hi"] != 10);

		bimap.erase(1000);
		SSVUT_EXPECT(!bimap.has(1000));
		SSVUT_EXPECT(!bimap.has("hi"));
	}

	{
		Bimap<string, int, std::unordered_map> bimap;
		bimap.emplace("hi", 10);
		SSVUT_EXPECT(bimap[10] == "hi");
		SSVUT_EXPECT(bimap["hi"] == 10);
		SSVUT_EXPECT(bimap.at(10) == "hi");
		SSVUT_EXPECT(bimap.at("hi") == 10);

		bimap.replace("hi", 1000);
		SSVUT_EXPECT(!bimap.has(10));
		SSVUT_EXPECT(bimap.has(1000));
		SSVUT_EXPECT(bimap.has("hi"));
		SSVUT_EXPECT(bimap["hi"] == 1000);
		SSVUT_EXPECT(bimap[1000] == "hi");
		SSVUT_EXPECT(bimap["hi"] != 10);

		bimap.erase(1000);
		SSVUT_EXPECT(!bimap.has(1000));
		SSVUT_EXPECT(!bimap.has("hi"));
	}
}
SSVU_TEST_END();

SSVU_TEST(DelegateTests)
{
	using namespace std;
	using namespace ssvu;

	bool testState{false};
	Delegate<void()> del1;
	del1 += [&testState]{ testState = !testState; };

	del1(); SSVUT_EXPECT(testState == true);
	del1(); SSVUT_EXPECT(testState == false);

	Delegate<int(int)> del2;
	del2 += [](int x){ return x + x; };
	del2 += [](int x){ return x * x; };

	auto del2result(del2(3));
	SSVUT_EXPECT(del2result[0] == 6);
	SSVUT_EXPECT(del2result[1] == 9);
	SSVUT_EXPECT(del2result.size() == 2);

	Delegate<void()> del3;
	del3 += [&del1]{ del1(); };
	del3(); SSVUT_EXPECT(testState == true);
}
SSVU_TEST_END();

SSVU_TEST(PathTests)
{
	using namespace std;
	using namespace ssvu;
	using namespace ssvu::FileSystem;

	Path path{"/usr"};
	SSVUT_EXPECT(path.getStr() == "/usr/");
	SSVUT_EXPECT(path.getFolderName() == "usr");

	path = "/usr.txt";
	SSVUT_EXPECT(path.getStr() == "/usr.txt");
	//SSVUT_EXPECT(path.getFolderName() == "");

	path = "/usr.txt/banana/.log";
	SSVUT_EXPECT(path.getStr() == "/usr.txt/banana/.log");
	SSVUT_EXPECT(path.getParent().getFolderName() == "banana");
	SSVUT_EXPECT(path.getExtension() == "");
	SSVUT_EXPECT(path.getAllExtensions() == "");
	SSVUT_EXPECT(path.getFileName() == ".log");
	SSVUT_EXPECT(path.getFileNameNoExtensions() == ".log");

	path = "///////////usr//////";
	SSVUT_EXPECT(path.getStr() == "/usr/");
	SSVUT_EXPECT(path.getFolderName() == "usr");

	path = "/////\\//\\usr///\\//test//test2/////test4.png";
	SSVUT_EXPECT(path.getStr() == "/usr/test/test2/test4.png");
	SSVUT_EXPECT(path.getExtension() == ".png");
	SSVUT_EXPECT(path.getAllExtensions() == ".png");
	SSVUT_EXPECT(path.getFileName() == "test4.png");
	SSVUT_EXPECT(path.getFileNameNoExtensions() == "test4");

	path = "/////\\//\\usr///\\//test//test2/////test4.png.bak";
	SSVUT_EXPECT(path.getStr() == "/usr/test/test2/test4.png.bak");
	SSVUT_EXPECT(path.getExtension() == ".bak");
	SSVUT_EXPECT(path.getAllExtensions() == ".png.bak");
	SSVUT_EXPECT(path.getFileName() == "test4.png.bak");
	SSVUT_EXPECT(path.getFileNameNoExtensions() == "test4");
}
SSVU_TEST_END();

SSVU_TEST(UtilsContainersTests)
{
	using namespace std;
	using namespace ssvu;

	std::vector<string> vec{"abc", "bcd", "efg", "ghi"};
	SSVUT_EXPECT(ssvu::find(vec, "abc") == std::begin(vec));
	SSVUT_EXPECT(ssvu::find(vec, "wut") == std::end(vec));
	SSVUT_EXPECT(ssvu::idxOf(vec, "efg") == 2);
	SSVUT_EXPECT(ssvu::findIf(vec, [](const string& s){ return beginsWith(s, "gh"); }) == std::begin(vec) + 3);
	SSVUT_EXPECT(ssvu::contains(vec, "bcd") == true);
	SSVUT_EXPECT(ssvu::contains(vec, "bcdnbdf") == false);
	SSVUT_EXPECT(ssvu::containsAnyIf(vec, [](const string& s){ return beginsWith(s, "gh"); }) == true);

	ssvu::eraseRemoveIf(vec, [](const string& s){ return beginsWith(s, "gh"); });
	SSVUT_EXPECT(vec.size() == 3);

	std::map<string, int> m{{"abc", 0}, {"bcd", 1}, {"def", 2}, {"efg", 3}};
	SSVUT_EXPECT(ssvu::getKeys(m)[2] == "def");
}
SSVU_TEST_END();

SSVU_TEST(StringUtilsTests)
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

	SSVUT_EXPECT(getTrimmedStrL("   AAA") == "AAA");
	SSVUT_EXPECT(getTrimmedStrL("   AA  A") == "AA  A");
	SSVUT_EXPECT(getTrimmedStrL("A A A") == "A A A");
	SSVUT_EXPECT(getTrimmedStrL("  A A A") == "A A A");

	SSVUT_EXPECT(getTrimmedStrR("AAA   ") == "AAA");
	SSVUT_EXPECT(getTrimmedStrR("AA  A   ") == "AA  A");
	SSVUT_EXPECT(getTrimmedStrR("A A A") == "A A A");
	SSVUT_EXPECT(getTrimmedStrR("A A A  ") == "A A A");

	SSVUT_EXPECT(getTrimmedStrLR("   A A A  ") == "A A A");
	SSVUT_EXPECT(getTrimmedStrLR("      AaA  ") == "AaA");

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
SSVU_TEST_END();

SSVU_TEST(StringSplitTests)
{
	using namespace std;
	using namespace ssvu;
	string s1{"test a b c d e"};
	string s2{"test,a,b,c,d,e"};
	string s3{"test##a##b##c##d##e"};

	auto sp1 = getSplit(s1, ' ');
	auto sp2 = getSplit(s2, ',');
	auto sp3 = getSplit(s3, "##");

	auto sps1 = getSplit(s1, std::string{" "});
	auto sps2 = getSplit(s2, std::string{","});
	auto sps3 = getSplit(s3, std::string{"##"});

	auto spks1 = getSplit<Split::TrailingSeparator>(s1, ' ');
	auto spks2 = getSplit<Split::TrailingSeparator>(s2, ',');
	auto spks3 = getSplit<Split::TrailingSeparator>(s3, "##");

	auto spksat1 = getSplit<Split::TokenizeSeparator>(s1, ' ');
	auto spksat2 = getSplit<Split::TokenizeSeparator>(s2, ',');
	auto spksat3 = getSplit<Split::TokenizeSeparator>(s3, "##");

	SSVUT_EXPECT((sp1.size() == sp2.size()) == (sp3.size() == 6));
	SSVUT_EXPECT((sps1.size() == sps2.size()) == (sps3.size() == 6));
	SSVUT_EXPECT((spks1.size() == spks2.size()) == (spks3.size() == 6));
	SSVUT_EXPECT((spksat1.size() == spksat2.size()) == (spksat3.size() == 11));

	SSVUT_EXPECT((sp1[0] == sp2[0]) == (sp3[0] == "test"));
	SSVUT_EXPECT((sp1[1] == sp2[1]) == (sp3[1] == "a"));
	SSVUT_EXPECT((sp1[2] == sp2[2]) == (sp3[2] == "b"));
	SSVUT_EXPECT((sp1[3] == sp2[3]) == (sp3[3] == "c"));
	SSVUT_EXPECT((sp1[4] == sp2[4]) == (sp3[4] == "d"));
	SSVUT_EXPECT((sp1[5] == sp2[5]) == (sp3[5] == "e"));

	SSVUT_EXPECT((sps1[0] == sps2[0]) == (sps3[0] == "test"));
	SSVUT_EXPECT((sps1[1] == sps2[1]) == (sps3[1] == "a"));
	SSVUT_EXPECT((sps1[2] == sps2[2]) == (sps3[2] == "b"));
	SSVUT_EXPECT((sps1[3] == sps2[3]) == (sps3[3] == "c"));
	SSVUT_EXPECT((sps1[4] == sps2[4]) == (sps3[4] == "d"));
	SSVUT_EXPECT((sps1[5] == sps2[5]) == (sps3[5] == "e"));

	SSVUT_EXPECT((spksat1[0] == spksat2[0]) == (spksat3[0] == "test"));
	SSVUT_EXPECT((spksat1[2] == spksat2[2]) == (spksat3[2] == "a"));
	SSVUT_EXPECT((spksat1[4] == spksat2[4]) == (spksat3[4] == "b"));
	SSVUT_EXPECT((spksat1[6] == spksat2[6]) == (spksat3[6] == "c"));
	SSVUT_EXPECT((spksat1[8] == spksat2[8]) == (spksat3[8] == "d"));
	SSVUT_EXPECT((spksat1[10] == spksat2[10]) == (spksat3[10] == "e"));

	SSVUT_EXPECT(spks1[2] == "b ");
	SSVUT_EXPECT(spks2[2] == "b,");
	SSVUT_EXPECT(spks3[2] == "b##");

	string s4{"test##a,b##c,d##efg"};
	vector<string> splits{"##", ","};

	auto spv1 = getSplit(s4, splits);
	auto spv2 = getSplit<Split::TrailingSeparator>(s4, splits);
	auto spv3 = getSplit<Split::TokenizeSeparator>(s4, splits);

	SSVUT_EXPECT(spv1.size() == 6);
	SSVUT_EXPECT(spv2.size() == 6);
	SSVUT_EXPECT(spv3.size() == 11);

	SSVUT_EXPECT((spv1[0] == "test") && spv2[0] == "test##");
	SSVUT_EXPECT((spv1[1] == "a") && spv2[1] == "a,");
	SSVUT_EXPECT((spv1[2] == "b") && spv2[2] == "b##");
	SSVUT_EXPECT((spv1[3] == "c") && spv2[3] == "c,");
	SSVUT_EXPECT((spv1[4] == "d") && spv2[4] == "d##");
	SSVUT_EXPECT((spv1[5] == "efg") && spv2[5] == "efg");

	SSVUT_EXPECT(spv3[0] == "test");
	SSVUT_EXPECT(spv3[1] == "##");
	SSVUT_EXPECT(spv3[2] == "a");
	SSVUT_EXPECT(spv3[3] == ",");
	SSVUT_EXPECT(spv3[4] == "b");
	SSVUT_EXPECT(spv3[5] == "##");
	SSVUT_EXPECT(spv3[6] == "c");
	SSVUT_EXPECT(spv3[7] == ",");
	SSVUT_EXPECT(spv3[8] == "d");
	SSVUT_EXPECT(spv3[9] == "##");
	SSVUT_EXPECT(spv3[10] == "efg");
}
SSVU_TEST_END();

SSVU_TEST(EncryptionTests)
{
	using namespace std;
	using namespace ssvu;
	using namespace ssvu::Encryption;

	SSVUT_EXPECT(encrypt<Type::MD5>("testhash") == "082949a8dfacccda185a135db425377b");
	SSVUT_EXPECT(encrypt<Type::MD5>("") == "d41d8cd98f00b204e9800998ecf8427e");

	SSVUT_EXPECT(encrypt<Type::Base64>("testhash") == "dGVzdGhhc2g=");
	SSVUT_EXPECT(decrypt<Type::Base64>("dGVzdGhhc2g=") == "testhash");
}
SSVU_TEST_END();

SSVU_TEST(ObfuscatedValueTests)
{
	using namespace std;
	using namespace ssvu;
	using namespace ssvu::Encryption;

	ObfuscatedValue<float> v{10.f};
	SSVUT_EXPECT(v.get() == 10.f);
	SSVUT_EXPECT(v == 10.f);
	v = 15.f;
	SSVUT_EXPECT(v.get() == 15.f);
	SSVUT_EXPECT(v == 15.f);
}
SSVU_TEST_END();

SSVU_TEST(PreprocessorTests)
{
	SSVUT_EXPECT(SSVPP_ARGCOUNT() == 0);
	SSVUT_EXPECT(SSVPP_ARGCOUNT(1) == 1);
	SSVUT_EXPECT(SSVPP_ARGCOUNT(1, 1) == 2);
	SSVUT_EXPECT(SSVPP_ARGCOUNT(1, 1, 1) == 3);
	SSVUT_EXPECT(SSVPP_ARGCOUNT(1, 1, 1, 1) == 4);
	SSVUT_EXPECT(SSVPP_ARGCOUNT(1, 1, 1, 1, 1) == 5);
	SSVUT_EXPECT(SSVPP_ARGCOUNT(1, 1, 1, 1, 1, 1) == 6);

	SSVUT_EXPECT(std::string{SSVPP_TOSTR(SSVPP_CAT())} == "");
	SSVUT_EXPECT(SSVPP_CAT(15) == 15);
	SSVUT_EXPECT(SSVPP_CAT(15, 15) == 1515);
	SSVUT_EXPECT(SSVPP_CAT(15, 15, 15) == 151515);

	SSVUT_EXPECT(SSVPP_INCREMENT(1) == 2);
	SSVUT_EXPECT(SSVPP_INCREMENT(100) == 101);
	SSVUT_EXPECT(SSVPP_DECREMENT(1) == 0);
	SSVUT_EXPECT(SSVPP_DECREMENT(100) == 99);

	SSVUT_EXPECT(SSVPP_BOOL(0) == 0);
	SSVUT_EXPECT(SSVPP_BOOL(1) == 1);
	SSVUT_EXPECT(SSVPP_BOOL(2) == 1);
	SSVUT_EXPECT(SSVPP_BOOL(3) == 1);

	SSVUT_EXPECT(SSVPP_AND(0, 0) == 0);
	SSVUT_EXPECT(SSVPP_AND(0, 53) == 0);
	SSVUT_EXPECT(SSVPP_AND(22, 0) == 0);
	SSVUT_EXPECT(SSVPP_AND(35, 11) == 1);

	SSVUT_EXPECT(SSVPP_OR(0, 0) == 0);
	SSVUT_EXPECT(SSVPP_OR(0, 53) == 1);
	SSVUT_EXPECT(SSVPP_OR(22, 0) == 1);
	SSVUT_EXPECT(SSVPP_OR(35, 11) == 1);

	SSVUT_EXPECT(SSVPP_NOR(0, 0) == 1);
	SSVUT_EXPECT(SSVPP_NOR(0, 53) == 0);
	SSVUT_EXPECT(SSVPP_NOR(22, 0) == 0);
	SSVUT_EXPECT(SSVPP_NOR(35, 11) == 0);

	SSVUT_EXPECT(SSVPP_XOR(0, 0) == 0);
	SSVUT_EXPECT(SSVPP_XOR(0, 53) == 1);
	SSVUT_EXPECT(SSVPP_XOR(22, 0) == 1);
	SSVUT_EXPECT(SSVPP_XOR(35, 11) == 0);

	SSVUT_EXPECT(SSVPP_NOT(35) == 0);
	SSVUT_EXPECT(SSVPP_NOT(0) == 1);

	SSVUT_EXPECT(SSVPP_IF(SSVPP_XOR(35, 11), 10, 20) == 20);
	SSVUT_EXPECT(SSVPP_IF(1, 10, 20) == 10);

	{
		int k{0};

		#define SSVU_TEST_FOREFFECT(mIdx, mData, mArg)	k += mIdx; k += mArg;
		//					v action(idx, data, arg)	v data				v vargs
		SSVPP_FOREACH(SSVU_TEST_FOREFFECT,		SSVPP_EMPTY(),	1, 2, 3, 4)
		#undef SSVU_TEST_FOREFFECT

		SSVUT_EXPECT(k == 16);
	}

	{
		std::string s(SSVPP_TOSTR_SEP(", ", 1, 2, 3));
		SSVUT_EXPECT(s == "1, 2, 3");
	}

	{
		#define SSVU_TEST_ADDTEN(mX)					SSVPP_CAT(1, mX)
		#define SSVU_TEST_FOREFFECT(mIdx, mData, mArg)	SSVU_TEST_ADDTEN(mArg)SSVPP_COMMA_IF(mIdx)

		std::string s(SSVPP_TOSTR_SEP(", ", SSVPP_FOREACH(SSVU_TEST_FOREFFECT, SSVPP_EMPTY(), 1, 2, 3, 4)));
		SSVUT_EXPECT(s == "11, 12, 13, 14");

		#undef SSVU_TEST_ADDTEN
		#undef SSVU_TEST_FOREFFECT
	}

	{
		SSVUT_EXPECT(SSVPP_TPL_ELEM((1, 2, 3), 0) == 1);
		SSVUT_EXPECT(SSVPP_TPL_ELEM((1, 2, 3), 1) == 2);
		SSVUT_EXPECT(SSVPP_TPL_ELEM((1, 2, 3), 2) == 3);

		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_MAKE(1, 2, 3), 0) == 1);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_MAKE(1, 2, 3), 1) == 2);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_MAKE(1, 2, 3), 2) == 3);

		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_EXPLODE(((1, 2))), 0) == 1);
		SSVUT_EXPECT(std::string{SSVPP_TOSTR_SEP(", ", SSVPP_TPL_EXPLODE((1, 2)))} == "1, 2");

		#define SSVU_TEST_CONCATENATED_TPLS SSVPP_TPL_CAT((1, 2), (3, 4))
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 0) == 1);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 1) == 2);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 2) == 3);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 3) == 4);
		#undef SSVU_TEST_CONCATENATED_TPLS

		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_CAT((1), (3)), 1) == 3);

		#define SSVU_TEST_CONCATENATED_TPLS SSVPP_TPL_CAT((1), (1, 2), (3, 4), (4, 5, 6), (1))

		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 0) == 1);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 1) == 1);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 2) == 2);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 3) == 3);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 4) == 4);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 5) == 4);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 6) == 5);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 7) == 6);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVU_TEST_CONCATENATED_TPLS, 8) == 1);

		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 0) == 1);
		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 1) == 6);
		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 2) == 5);
		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 3) == 4);
		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 4) == 4);
		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 5) == 3);
		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 6) == 2);
		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 7) == 1);
		SSVUT_EXPECT(SSVPP_TPL_ELEM_REVERSE(SSVU_TEST_CONCATENATED_TPLS, 8) == 1);

		#undef SSVU_TEST_CONCATENATED_TPLS

		SSVUT_EXPECT(SSVPP_TPL_SIZE((1, 2, 3)) == 3);
		SSVUT_EXPECT(SSVPP_TPL_SIZE((2)) == 1);
		SSVUT_EXPECT(SSVPP_TPL_SIZE((1, 2, 3, 1, 1)) == 5);
		SSVUT_EXPECT(SSVPP_TPL_SIZE(()) == 0);

		SSVUT_EXPECT(SSVPP_TPL_HEAD((1, 2, 3)) == 1);
		SSVUT_EXPECT(SSVPP_TPL_TAIL((1, 2, 3)) == 3);

		SSVUT_EXPECT(SSVPP_TPL_HEAD((1, 2)) == 1);
		SSVUT_EXPECT(SSVPP_TPL_TAIL((1, 2)) == 2);

		SSVUT_EXPECT(SSVPP_TPL_HEAD((1)) == 1);
		SSVUT_EXPECT(SSVPP_TPL_TAIL((1)) == 1);

		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_PUSH_FRONT((1, 2), 3), 0) == 3);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_PUSH_FRONT((1, 2), 3), 2) == 2);

		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_PUSH_BACK((1, 2), 3), 0) == 1);
		SSVUT_EXPECT(SSVPP_TPL_ELEM(SSVPP_TPL_PUSH_BACK((1, 2), 3), 2) == 3);
	}

	{
		SSVUT_EXPECT(SSVPP_TPL_SIZE(SSVPP_TPL_FILL(())) == SSVPP_TPL_MAX_SIZE);
		SSVUT_EXPECT(SSVPP_TPL_SIZE(SSVPP_TPL_FILL((1))) == SSVPP_TPL_MAX_SIZE);
		SSVUT_EXPECT(SSVPP_TPL_SIZE(SSVPP_TPL_FILL((1, 1))) == SSVPP_TPL_MAX_SIZE);
		SSVUT_EXPECT(SSVPP_TPL_SIZE(SSVPP_TPL_FILL((1, 1, 1))) == SSVPP_TPL_MAX_SIZE);
	}

	{
		#define SSVU_TEST_GEN_LMBD(mReturn, mName, mBody) auto mName = []() -> mReturn { mBody };

		SSVU_TEST_GEN_LMBD(__R(std::pair<int, int>), __R(lambda1), __R(return std::make_pair(1, 5);));
		SSVU_TEST_GEN_LMBD(__R(std::pair<int, std::pair<float, float>>), __R(lambda2), __R(return std::make_pair(2, std::pair<float, float>(1.5f, 2.5f));));

		SSVUT_EXPECT(lambda1().first == 1);
		SSVUT_EXPECT(lambda1().second == 5);

		SSVUT_EXPECT(lambda2().first == 2);
		SSVUT_EXPECT(lambda2().second.first == 1.5f);
		SSVUT_EXPECT(lambda2().second.second == 2.5f);

		#undef SSVU_TEST_GEN_LMBD
	}
}
SSVU_TEST_END();

// NOTE: MEMFN_DETECTORs (and other macros) must be defined outside lambdas/function bodies
SSVU_DEFINE_MEMFN_DETECTOR(SsvuTestMacroTestChecker1, hello);
SSVU_DEFINE_MEMFN_DETECTOR(SsvuTestMacroTestChecker2, hello2);
SSVU_DEFINE_MEMFN_DETECTOR(SsvuTestMacroTestChecker3, nohello);
SSVU_DEFINE_MEMFN_CALLER(SsvuTestMacroTestCallGoodbye1, goodbye1, int());
SSVU_DEFINE_MEMFN_CALLER(SsvuTestMacroTestCallGoodbye2, goodbye2, int());
SSVU_TEST(MacrosTests)
{
	SSVU_DEFINE_DUMMY_STRUCT(blah, test, abc);

	struct MacroTestStruct
	{
		void hello() { }
		float hello2(int) { return 3.f; }
		int goodbye1() { return 2; }
		int goodbye2() { return 4; }
	};

	MacroTestStruct mts;

	SSVUT_EXPECT(__R(SsvuTestMacroTestChecker1<MacroTestStruct, void()>()) == true);
	SSVUT_EXPECT(__R(SsvuTestMacroTestChecker2<MacroTestStruct, void()>()) == false);
	SSVUT_EXPECT(__R(SsvuTestMacroTestChecker2<MacroTestStruct, float(int)>()) == true);
	SSVUT_EXPECT(__R(SsvuTestMacroTestChecker3<MacroTestStruct, void()>()) == false);
	SSVUT_EXPECT(__R(SsvuTestMacroTestChecker3<MacroTestStruct, float(int)>()) == false);
	SSVUT_EXPECT(SsvuTestMacroTestCallGoodbye1(mts) == 2);
	SSVUT_EXPECT(SsvuTestMacroTestCallGoodbye2(mts) == 4);
}
SSVU_TEST_END();

SSVU_TEST(StringifierTests)
{
	auto nothing([]{ });
	int testArray[]{1, 2, 3};
	std::ostringstream trash;
	#define SSVUT_STRINGIFY_TEST(mValue) { auto k(mValue); ssvu::stringify<true>(trash, k); ssvu::stringify<false>(trash, k); } nothing()

	SSVUT_STRINGIFY_TEST(0);
	SSVUT_STRINGIFY_TEST(0l);
	SSVUT_STRINGIFY_TEST(0u);
	SSVUT_STRINGIFY_TEST(0ul);
	SSVUT_STRINGIFY_TEST(0.f);
	SSVUT_STRINGIFY_TEST(0.0);
	SSVUT_STRINGIFY_TEST(&trash);
	SSVUT_STRINGIFY_TEST("abc");
	SSVUT_STRINGIFY_TEST(std::string{"abc"});
	ssvu::stringify<true>(trash, testArray); ssvu::stringify<false>(trash, testArray);
	SSVUT_STRINGIFY_TEST(__R(std::array<int, 3>{{1, 2, 3}}));
	SSVUT_STRINGIFY_TEST(__R(std::vector<int>{1, 2, 3}));
	SSVUT_STRINGIFY_TEST(__R(std::list<int>{1, 2, 3}));
	SSVUT_STRINGIFY_TEST(__R(std::forward_list<int>{1, 2, 3}));
	SSVUT_STRINGIFY_TEST(__R(std::map<int, std::string>{{1, "aa"}, {2, "bb"}, {3, "cc"}}));
	SSVUT_STRINGIFY_TEST(__R(std::unordered_map<int, std::string>{{1, "aa"}, {2, "bb"}, {3, "cc"}}));
	SSVUT_STRINGIFY_TEST(__R(ssvu::Bimap<int, std::string>{{1, "aa"}, {2, "bb"}, {3, "cc"}}));
	SSVUT_STRINGIFY_TEST(__R(std::tuple<int, int, int>{1, 2, 3}));
	SSVUT_STRINGIFY_TEST(__R(std::pair<int, int>{2, 3}));
}
SSVU_TEST_END();

#endif
