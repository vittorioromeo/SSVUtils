// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSUTILSCONTAINER
#define SSVU_TESTS_TESTSUTILSCONTAINER

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Test/Test.hpp"

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

// TODO: move
SSVUT_TEST(ContainerMkTests)
{
	using namespace ssvu;

	{
		auto vecInt(mkVector(0, 1, 2, 3, 4, 5));
		SSVU_ASSERT_STATIC_NM(isSame<decltype(vecInt), std::vector<int>>());
		SSVUT_EXPECT_OP(vecInt.size(), ==, 6);
	}

	{
		auto vecFloat(mkVector(0, 1, 2, 3, 4, 5.f));
		SSVU_ASSERT_STATIC_NM(isSame<decltype(vecFloat), std::vector<float>>());
		SSVUT_EXPECT_OP(vecFloat.size(), ==, 6);
	}

	{
		auto vecDouble(mkVector(0., 1, 2.f, 3, 4, 5));
		SSVU_ASSERT_STATIC_NM(isSame<decltype(vecDouble), std::vector<double>>());
		SSVUT_EXPECT_OP(vecDouble.size(), ==, 6);
	}

	{
		auto umIntInt(mkUnorderedMap(0, 0, 1, 1, 2, 2));
		SSVU_ASSERT_STATIC_NM(isSame<decltype(umIntInt), std::unordered_map<int, int>>());
		SSVUT_EXPECT_OP(umIntInt.size(), ==, 3);
	}

	{
		auto umIntFloat(mkUnorderedMap(0, 0, 1, 1, 2, 2.f));
		SSVU_ASSERT_STATIC_NM(isSame<decltype(umIntFloat), std::unordered_map<int, float>>());
		SSVUT_EXPECT_OP(umIntFloat.size(), ==, 3);
	}

	{
		auto umStrInt(mkUnorderedMap("0", 0, "1", 1, std::string{"2"}, 2));
		SSVU_ASSERT_STATIC_NM(isSame<decltype(umStrInt), std::unordered_map<std::string, int>>());
		SSVUT_EXPECT_OP(umStrInt.size(), ==, 3);
	}
}

#endif
