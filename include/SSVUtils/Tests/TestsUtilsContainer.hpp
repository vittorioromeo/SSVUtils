// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSUTILSCONTAINER
#define SSVU_TESTS_TESTSUTILSCONTAINER

SSVUT_TEST(UtilsContainersTests)
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

#endif
