// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TEST_TESTS
#define SSVU_TEST_TESTS

SSVU_TEST("UtilsMath tests")
{
	using namespace std;
	using namespace ssvu;

	for(int i{0}; i < 100; ++i)
	{
		auto r(getRnd<int>(-10, 10));
		EXPECT(r >= -10 && r < 10);

		auto ru(getRnd<unsigned int>(0, 10));
		EXPECT(ru >= 0 && ru < 10);

		auto rf(getRndR<float>(0.f, 2.f));
		EXPECT(rf >= 0.f && rf < 2.f);
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

SSVU_TEST("Bimap tests")
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

SSVU_TEST("Delegate tests")
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

SSVU_TEST("Path tests")
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

	path = "/////\\\//\\\usr///\\\//test//test2/////test4.png";
	EXPECT(path.getStr() == "/usr/test/test2/test4.png");
	EXPECT(path.getExtension() == ".png");
	EXPECT(path.getAllExtensions() == ".png");
	EXPECT(path.getFileName() == "test4.png");
	EXPECT(path.getFileNameNoExtensions() == "test4");

	path = "/////\\\//\\\usr///\\\//test//test2/////test4.png.bak";
	EXPECT(path.getStr() == "/usr/test/test2/test4.png.bak");
	EXPECT(path.getExtension() == ".bak");
	EXPECT(path.getAllExtensions() == ".png.bak");
	EXPECT(path.getFileName() == "test4.png.bak");
	EXPECT(path.getFileNameNoExtensions() == "test4");
}
SSVU_TEST_END();

SSVU_TEST("UtilsContainers tests")
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

SSVU_TEST("String utils tests")
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
}
SSVU_TEST_END();

SSVU_TEST("String split tests")
{
	using namespace std;
	using namespace ssvu;
	string s1{"test a b c d e"};
	string s2{"test,a,b,c,d,e"};
	string s3{"test##a##b##c##d##e"};

	auto sp1 = getSplit(s1, ' ');
	auto sp2 = getSplit(s2, ',');
	auto sp3 = getSplit<string>(s3, "##");

	auto spks1 = getSplit<char, Split::KeepSeparator>(s1, ' ');
	auto spks2 = getSplit<char, Split::KeepSeparator>(s2, ',');
	auto spks3 = getSplit<string, Split::KeepSeparator>(s3, "##");

	EXPECT((sp1.size() == sp2.size()) == (sp3.size() == 6));
	EXPECT((spks1.size() == spks2.size()) == (spks3.size() == 6));

	EXPECT((sp1[0] == sp2[0]) == (sp3[0] == "test"));
	EXPECT((sp1[1] == sp2[1]) == (sp3[1] == "a"));
	EXPECT((sp1[2] == sp2[2]) == (sp3[2] == "b"));
	EXPECT((sp1[3] == sp2[3]) == (sp3[3] == "c"));
	EXPECT((sp1[4] == sp2[4]) == (sp3[4] == "d"));
	EXPECT((sp1[5] == sp2[5]) == (sp3[5] == "e"));

	EXPECT(spks1[2] == "b ");
	EXPECT(spks2[2] == "b,");
	EXPECT(spks3[2] == "b##");
}
SSVU_TEST_END();

SSVU_TEST("Encryption tests")
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

SSVU_TEST("ObfuscatedValue tests")
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

#endif
