// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TEST_TESTS
#define SSVU_TEST_TESTS

#include "SSVUtils/Test/Test.hpp"
#include "SSVUtils/Preprocessor/Preprocessor.hpp"

SSVU_TEST(UtilsMathTests)
{
	using namespace std;
	using namespace ssvu;

	for(int i{0}; i < 100; ++i)
	{
		auto r(getRnd<int>(-10, 10));
		EXPECT(r >= -10 && r < 10);

		// getRnd is [a, b)
		auto ru(getRnd<unsigned int>(1, 10));
		EXPECT(ru > 0 && ru < 10);

		// getRndR is [a, b]
		auto rf(getRndR<float>(0.f, 2.f));
		EXPECT(rf >= 0.f && rf <= 2.f);

		auto rs(getRndSign());
		EXPECT(rs == 1 || rs == -1);
	}

	EXPECT(getSign(-1) == -1);
	EXPECT(getSign(1) == 1);
	EXPECT(getSign(0) == 0);

	EXPECT(getSign(-1u) == 1);
	EXPECT(getSign(1u) == 1);
	EXPECT(getSign(0u) == 0);

	EXPECT(getSign(-1.5f) == -1);
	EXPECT(getSign(1.5f) == 1);
	EXPECT(getSign(0.5f) == 1);
	EXPECT(getSign(0.0f) == 0);

	EXPECT(getClampedMin(0, 100) == 100);
	EXPECT(getClampedMin(150, 100) == 150);
	EXPECT(getClampedMax(0, 100) == 0);
	EXPECT(getClampedMax(150, 100) == 100);
	EXPECT(getClamped(1000, 0, 500) == 500);
	EXPECT(getClamped(1000, 1500, 2500) == 1500);

	EXPECT(wrapDeg(720) == 0);
	EXPECT(wrapDeg(720 + 180) == 180);

	EXPECT(getSIMod(10, 9) == 1);
	EXPECT(getSIMod(-10, 9) == 8);
	EXPECT(getSIMod(10, -9) == 8);

	EXPECT(getWrapIdx(10, 5) == 0);
	EXPECT(getWrapIdx(10, 10) == 0);
	EXPECT(getWrapIdx(10, 15) == 10);
	EXPECT(getWrapIdx(4, 3) == 1);
	EXPECT(getWrapIdx(5, 3) == 2);
	EXPECT(getWrapIdx(6, 3) == 0);
	EXPECT(getWrapIdx(-1, 3) == 2);
	EXPECT(getWrapIdx(-2, 3) == 1);
	EXPECT(getWrapIdx(-3, 3) == 0);
	EXPECT(getWrapIdx(-4, 3) == 2);
}
SSVU_TEST_END();

SSVU_TEST(BimapTests)
{
	using namespace std;
	using namespace ssvu;

	Bimap<string, int> bimap;
	bimap.insert({"hi", 10});
	EXPECT(bimap[10] == "hi");
	EXPECT(bimap["hi"] == 10);

	bimap.insert({"hi", 1000});
	EXPECT(bimap[10] == "hi");
	EXPECT(bimap["hi"] != 10);
	EXPECT(bimap[1000] == "hi");
	EXPECT(bimap["hi"] == 1000);

	bimap.erase(10);
	EXPECT(!bimap.has(10));
	EXPECT(!bimap.has("hi"));
	EXPECT(bimap.has(1000));
}
SSVU_TEST_END();

SSVU_TEST(DelegateTests)
{
	using namespace std;
	using namespace ssvu;

	bool testState{false};
	Delegate<void()> del1;
	del1 += [&testState]{ testState = !testState; };

	del1(); EXPECT(testState == true);
	del1(); EXPECT(testState == false);

	Delegate<int(int)> del2;
	del2 += [](int x){ return x + x; };
	del2 += [](int x){ return x * x; };

	auto del2result(del2(3));
	EXPECT(del2result[0] == 6);
	EXPECT(del2result[1] == 9);
	EXPECT(del2result.size() == 2);

	Delegate<void()> del3;
	del3 += [&del1]{ del1(); };
	del3(); EXPECT(testState == true);
}
SSVU_TEST_END();

SSVU_TEST(PathTests)
{
	using namespace std;
	using namespace ssvu;
	using namespace ssvu::FileSystem;

	Path path{"/usr"};
	EXPECT(path.getStr() == "/usr/");
	EXPECT(path.getFolderName() == "usr");

	path = "/usr.txt";
	EXPECT(path.getStr() == "/usr.txt");
	//EXPECT(path.getFolderName() == "");

	path = "/usr.txt/banana/.log";
	EXPECT(path.getStr() == "/usr.txt/banana/.log");
	EXPECT(path.getParent().getFolderName() == "banana");
	EXPECT(path.getExtension() == "");
	EXPECT(path.getAllExtensions() == "");
	EXPECT(path.getFileName() == ".log");
	EXPECT(path.getFileNameNoExtensions() == ".log");

	path = "///////////usr//////";
	EXPECT(path.getStr() == "/usr/");
	EXPECT(path.getFolderName() == "usr");

	path = "/////\\//\\usr///\\//test//test2/////test4.png";
	EXPECT(path.getStr() == "/usr/test/test2/test4.png");
	EXPECT(path.getExtension() == ".png");
	EXPECT(path.getAllExtensions() == ".png");
	EXPECT(path.getFileName() == "test4.png");
	EXPECT(path.getFileNameNoExtensions() == "test4");

	path = "/////\\//\\usr///\\//test//test2/////test4.png.bak";
	EXPECT(path.getStr() == "/usr/test/test2/test4.png.bak");
	EXPECT(path.getExtension() == ".bak");
	EXPECT(path.getAllExtensions() == ".png.bak");
	EXPECT(path.getFileName() == "test4.png.bak");
	EXPECT(path.getFileNameNoExtensions() == "test4");
}
SSVU_TEST_END();

SSVU_TEST(UtilsContainersTests)
{
	using namespace std;
	using namespace ssvu;

	std::vector<string> vec{"abc", "bcd", "efg", "ghi"};
	EXPECT(ssvu::find(vec, "abc") == std::begin(vec));
	EXPECT(ssvu::find(vec, "wut") == std::end(vec));
	EXPECT(ssvu::idxOf(vec, "efg") == 2);
	EXPECT(ssvu::findIf(vec, [](const string& s){ return beginsWith(s, "gh"); }) == std::begin(vec) + 3);
	EXPECT(ssvu::contains(vec, "bcd") == true);
	EXPECT(ssvu::contains(vec, "bcdnbdf") == false);
	EXPECT(ssvu::containsAnyIf(vec, [](const string& s){ return beginsWith(s, "gh"); }) == true);

	ssvu::eraseRemoveIf(vec, [](const string& s){ return beginsWith(s, "gh"); });
	EXPECT(vec.size() == 3);

	std::map<string, int> m{{"abc", 0}, {"bcd", 1}, {"def", 2}, {"efg", 3}};
	EXPECT(ssvu::getKeys(m)[2] == "def");
}
SSVU_TEST_END();

SSVU_TEST(StringUtilsTests)
{
	using namespace std;
	using namespace ssvu;
	string s{"test abc string abc"};

	EXPECT(toStr(100) == "100");
	EXPECT(toStr(100.f) == "100");
	EXPECT(getReplaced(s, "test", "banana") == "banana abc string abc");
	EXPECT(getReplacedAll(s, "abc", "cba") == "test cba string cba");
	EXPECT(beginsWith(s, "test") == true);
	EXPECT(beginsWith(s, 't') == true);
	EXPECT(beginsWith(s, "testa") == false);
	EXPECT(beginsWith(s, 'k') == false);
	EXPECT(endsWith(s, "abc") == true);
	EXPECT(endsWith(s, 'c') == true);
	EXPECT(endsWith(s, "cba") == false);
	EXPECT(endsWith(s, 'a') == false);
	EXPECT(toLower("AAA") == "aaa");
	EXPECT(getCharCount("AAA", 'A') == 3);
	EXPECT(getLevenshteinDistance("AAA", "AAB") == 1);

	EXPECT(getTrimmedStrL("   AAA") == "AAA");
	EXPECT(getTrimmedStrL("   AA  A") == "AA  A");
	EXPECT(getTrimmedStrL("A A A") == "A A A");
	EXPECT(getTrimmedStrL("  A A A") == "A A A");

	EXPECT(getTrimmedStrR("AAA   ") == "AAA");
	EXPECT(getTrimmedStrR("AA  A   ") == "AA  A");
	EXPECT(getTrimmedStrR("A A A") == "A A A");
	EXPECT(getTrimmedStrR("A A A  ") == "A A A");

	EXPECT(getTrimmedStrLR("   A A A  ") == "A A A");
	EXPECT(getTrimmedStrLR("      AaA  ") == "AaA");

	EXPECT(isDigit('0'));
	EXPECT(isDigit('1'));
	EXPECT(isDigit('2'));
	EXPECT(isDigit('3'));
	EXPECT(isDigit('4'));
	EXPECT(isDigit('5'));
	EXPECT(isDigit('6'));
	EXPECT(isDigit('7'));
	EXPECT(isDigit('8'));
	EXPECT(isDigit('9'));

	EXPECT(isUppercase('A'));
	EXPECT(isUppercase('B'));
	EXPECT(isUppercase('C'));

	EXPECT(isLowercase('a'));
	EXPECT(isLowercase('b'));
	EXPECT(isLowercase('c'));

	EXPECT(isControl('\n'));
	EXPECT(isBlank(' '));
	EXPECT(isSpace(' '));
	EXPECT(isSpace('\t'));

	EXPECT(isPunctuation('.'));
	EXPECT(isPunctuation(','));
	EXPECT(isPunctuation('!'));
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

	EXPECT((sp1.size() == sp2.size()) == (sp3.size() == 6));
	EXPECT((sps1.size() == sps2.size()) == (sps3.size() == 6));
	EXPECT((spks1.size() == spks2.size()) == (spks3.size() == 6));
	EXPECT((spksat1.size() == spksat2.size()) == (spksat3.size() == 11));

	EXPECT((sp1[0] == sp2[0]) == (sp3[0] == "test"));
	EXPECT((sp1[1] == sp2[1]) == (sp3[1] == "a"));
	EXPECT((sp1[2] == sp2[2]) == (sp3[2] == "b"));
	EXPECT((sp1[3] == sp2[3]) == (sp3[3] == "c"));
	EXPECT((sp1[4] == sp2[4]) == (sp3[4] == "d"));
	EXPECT((sp1[5] == sp2[5]) == (sp3[5] == "e"));

	EXPECT((sps1[0] == sps2[0]) == (sps3[0] == "test"));
	EXPECT((sps1[1] == sps2[1]) == (sps3[1] == "a"));
	EXPECT((sps1[2] == sps2[2]) == (sps3[2] == "b"));
	EXPECT((sps1[3] == sps2[3]) == (sps3[3] == "c"));
	EXPECT((sps1[4] == sps2[4]) == (sps3[4] == "d"));
	EXPECT((sps1[5] == sps2[5]) == (sps3[5] == "e"));

	EXPECT((spksat1[0] == spksat2[0]) == (spksat3[0] == "test"));
	EXPECT((spksat1[2] == spksat2[2]) == (spksat3[2] == "a"));
	EXPECT((spksat1[4] == spksat2[4]) == (spksat3[4] == "b"));
	EXPECT((spksat1[6] == spksat2[6]) == (spksat3[6] == "c"));
	EXPECT((spksat1[8] == spksat2[8]) == (spksat3[8] == "d"));
	EXPECT((spksat1[10] == spksat2[10]) == (spksat3[10] == "e"));

	EXPECT(spks1[2] == "b ");
	EXPECT(spks2[2] == "b,");
	EXPECT(spks3[2] == "b##");

	string s4{"test##a,b##c,d##efg"};
	vector<string> splits{"##", ","};

	auto spv1 = getSplit(s4, splits);
	auto spv2 = getSplit<Split::TrailingSeparator>(s4, splits);
	auto spv3 = getSplit<Split::TokenizeSeparator>(s4, splits);

	EXPECT(spv1.size() == 6);
	EXPECT(spv2.size() == 6);
	EXPECT(spv3.size() == 11);

	EXPECT((spv1[0] == "test") && spv2[0] == "test##");
	EXPECT((spv1[1] == "a") && spv2[1] == "a,");
	EXPECT((spv1[2] == "b") && spv2[2] == "b##");
	EXPECT((spv1[3] == "c") && spv2[3] == "c,");
	EXPECT((spv1[4] == "d") && spv2[4] == "d##");
	EXPECT((spv1[5] == "efg") && spv2[5] == "efg");

	EXPECT(spv3[0] == "test");
	EXPECT(spv3[1] == "##");
	EXPECT(spv3[2] == "a");
	EXPECT(spv3[3] == ",");
	EXPECT(spv3[4] == "b");
	EXPECT(spv3[5] == "##");
	EXPECT(spv3[6] == "c");
	EXPECT(spv3[7] == ",");
	EXPECT(spv3[8] == "d");
	EXPECT(spv3[9] == "##");
	EXPECT(spv3[10] == "efg");
}
SSVU_TEST_END();

SSVU_TEST(EncryptionTests)
{
	using namespace std;
	using namespace ssvu;
	using namespace ssvu::Encryption;

	EXPECT(encrypt<Type::MD5>("testhash") == "082949a8dfacccda185a135db425377b");
	EXPECT(encrypt<Type::MD5>("") == "d41d8cd98f00b204e9800998ecf8427e");

	EXPECT(encrypt<Type::Base64>("testhash") == "dGVzdGhhc2g=");
	EXPECT(decrypt<Type::Base64>("dGVzdGhhc2g=") == "testhash");
}
SSVU_TEST_END();

SSVU_TEST(ObfuscatedValueTests)
{
	using namespace std;
	using namespace ssvu;
	using namespace ssvu::Encryption;

	ObfuscatedValue<float> v{10.f};
	EXPECT(v.get() == 10.f);
	EXPECT(v == 10.f);
	v = 15.f;
	EXPECT(v.get() == 15.f);
	EXPECT(v == 15.f);
}
SSVU_TEST_END();

SSVU_TEST(PreprocessorTests)
{
	EXPECT(SSVPP_VA_NUM_ARGS() == 0);
	EXPECT(SSVPP_VA_NUM_ARGS(1) == 1);
	EXPECT(SSVPP_VA_NUM_ARGS(1, 1) == 2);
	EXPECT(SSVPP_VA_NUM_ARGS(1, 1, 1) == 3);
	EXPECT(SSVPP_VA_NUM_ARGS(1, 1, 1, 1) == 4);
	EXPECT(SSVPP_VA_NUM_ARGS(1, 1, 1, 1, 1) == 5);
	EXPECT(SSVPP_VA_NUM_ARGS(1, 1, 1, 1, 1, 1) == 6);

	EXPECT(SSVPP_STRINGIFY(SSVPP_CAT()) == "");
	EXPECT(SSVPP_CAT(15) == 15);
	EXPECT(SSVPP_CAT(15, 15) == 1515);
	EXPECT(SSVPP_CAT(15, 15, 15) == 151515);

	EXPECT(SSVPP_INCREMENT(1) == 2);
	EXPECT(SSVPP_INCREMENT(100) == 101);
	EXPECT(SSVPP_DECREMENT(1) == 0);
	EXPECT(SSVPP_DECREMENT(100) == 99);

	EXPECT(SSVPP_BOOL(0) == 0);
	EXPECT(SSVPP_BOOL(1) == 1);
	EXPECT(SSVPP_BOOL(2) == 1);
	EXPECT(SSVPP_BOOL(3) == 1);

	EXPECT(SSVPP_AND(0, 0) == 0);
	EXPECT(SSVPP_AND(0, 53) == 0);
	EXPECT(SSVPP_AND(22, 0) == 0);
	EXPECT(SSVPP_AND(35, 11) == 1);

	EXPECT(SSVPP_OR(0, 0) == 0);
	EXPECT(SSVPP_OR(0, 53) == 1);
	EXPECT(SSVPP_OR(22, 0) == 1);
	EXPECT(SSVPP_OR(35, 11) == 1);

	EXPECT(SSVPP_NOR(0, 0) == 1);
	EXPECT(SSVPP_NOR(0, 53) == 0);
	EXPECT(SSVPP_NOR(22, 0) == 0);
	EXPECT(SSVPP_NOR(35, 11) == 0);

	EXPECT(SSVPP_XOR(0, 0) == 0);
	EXPECT(SSVPP_XOR(0, 53) == 1);
	EXPECT(SSVPP_XOR(22, 0) == 1);
	EXPECT(SSVPP_XOR(35, 11) == 0);

	EXPECT(SSVPP_NOT(35) == 0);
	EXPECT(SSVPP_NOT(0) == 1);

	EXPECT(SSVPP_IF(SSVPP_XOR(35, 11), 10, 20) == 20);
	EXPECT(SSVPP_IF(1, 10, 20) == 10);

	{
		int k{0};

		#define SSVU_TEST_FOREFFECT(mIdx, mData, mArg)	k += mIdx; k += mArg;
		//					v action(idx, data, arg)	v data				v vargs
		SSVPP_FOREACH(SSVU_TEST_FOREFFECT,		SSVPP_EMPTY(),	1, 2, 3, 4)
		#undef SSVU_TEST_FOREFFECT

		EXPECT(k == 16);
	}

	{
		std::string s(SSVPP_STRINGIFYWITHCOMMAS(1, 2, 3));
		EXPECT(s == "1, 2, 3");
	}

	{
		#define SSVU_TEST_ADDTEN(mX)					SSVPP_CAT(1, mX)
		#define SSVU_TEST_FOREFFECT(mIdx, mData, mArg)	SSVU_TEST_ADDTEN(mArg)SSVPP_COMMA_IF(mIdx)

		std::string s(SSVPP_STRINGIFYWITHCOMMAS(SSVPP_FOREACH(SSVU_TEST_FOREFFECT, SSVPP_EMPTY(), 1, 2, 3, 4)));
		EXPECT(s == "11, 12, 13, 14");

		#undef SSVU_TEST_ADDTEN
		#undef SSVU_TEST_FOREFFECT
	}

	{
		EXPECT(SSVPP_TPL_GET(0, (1, 2, 3)) == 1);
		EXPECT(SSVPP_TPL_GET(1, (1, 2, 3)) == 2);
		EXPECT(SSVPP_TPL_GET(2, (1, 2, 3)) == 3);

		EXPECT(SSVPP_TPL_GET(0, SSVPP_TPL_MAKE(1, 2, 3)) == 1);
		EXPECT(SSVPP_TPL_GET(1, SSVPP_TPL_MAKE(1, 2, 3)) == 2);
		EXPECT(SSVPP_TPL_GET(2, SSVPP_TPL_MAKE(1, 2, 3)) == 3);

		EXPECT(SSVPP_TPL_GET(0, SSVPP_TPL_EXPLODE(((1, 2)))) == 1);
		EXPECT(SSVPP_STRINGIFYWITHCOMMAS(SSVPP_TPL_EXPLODE((1, 2))) == "1, 2");

		#define SSVU_TEST_CONCATENATED_TPLS SSVPP_TPL_CAT((1, 2), (3, 4))
		EXPECT(SSVPP_TPL_GET(0, SSVU_TEST_CONCATENATED_TPLS) == 1);
		EXPECT(SSVPP_TPL_GET(1, SSVU_TEST_CONCATENATED_TPLS) == 2);
		EXPECT(SSVPP_TPL_GET(2, SSVU_TEST_CONCATENATED_TPLS) == 3);
		EXPECT(SSVPP_TPL_GET(3, SSVU_TEST_CONCATENATED_TPLS) == 4);
		#undef SSVU_TEST_CONCATENATED_TPLS

		EXPECT(SSVPP_TPL_GET(1, SSVPP_TPL_CAT((1), (3))) == 3);

		#define SSVU_TEST_CONCATENATED_TPLS SSVPP_TPL_CAT((1), (1, 2), (3, 4), (4, 5, 6), (1))
		EXPECT(SSVPP_TPL_GET(0, SSVU_TEST_CONCATENATED_TPLS) == 1);
		EXPECT(SSVPP_TPL_GET(1, SSVU_TEST_CONCATENATED_TPLS) == 1);
		EXPECT(SSVPP_TPL_GET(2, SSVU_TEST_CONCATENATED_TPLS) == 2);
		EXPECT(SSVPP_TPL_GET(3, SSVU_TEST_CONCATENATED_TPLS) == 3);
		EXPECT(SSVPP_TPL_GET(4, SSVU_TEST_CONCATENATED_TPLS) == 4);
		EXPECT(SSVPP_TPL_GET(5, SSVU_TEST_CONCATENATED_TPLS) == 4);
		EXPECT(SSVPP_TPL_GET(6, SSVU_TEST_CONCATENATED_TPLS) == 5);
		EXPECT(SSVPP_TPL_GET(7, SSVU_TEST_CONCATENATED_TPLS) == 6);
		EXPECT(SSVPP_TPL_GET(8, SSVU_TEST_CONCATENATED_TPLS) == 1);
		#undef SSVU_TEST_CONCATENATED_TPLS

		EXPECT(SSVPP_TPL_SIZE((1, 2, 3)) == 3);
		EXPECT(SSVPP_TPL_SIZE((2)) == 1);
		EXPECT(SSVPP_TPL_SIZE((1, 2, 3, 1, 1)) == 5);
		EXPECT(SSVPP_TPL_SIZE(()) == 0);
	}

	{
		EXPECT(SSVPP_TPL_SIZE(SSVPP_TPL_FILL(())) == SSVPP_TPL_MAX_SIZE);
		EXPECT(SSVPP_TPL_SIZE(SSVPP_TPL_FILL((1))) == SSVPP_TPL_MAX_SIZE);
		EXPECT(SSVPP_TPL_SIZE(SSVPP_TPL_FILL((1, 1))) == SSVPP_TPL_MAX_SIZE);
		EXPECT(SSVPP_TPL_SIZE(SSVPP_TPL_FILL((1, 1, 1))) == SSVPP_TPL_MAX_SIZE);
	}
}
SSVU_TEST_END();

#endif
