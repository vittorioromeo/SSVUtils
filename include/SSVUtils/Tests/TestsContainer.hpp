// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSCONTAINER
#define SSVU_TESTS_TESTSCONTAINER

SSVUT_TEST(ContainerVecMapPODTests)
{
	using namespace ssvu;

	VecMap<int, int> tm;

	SSVUT_EXPECT(tm.empty());
	SSVUT_EXPECT(tm.size() == 0);

	tm.reserve(3);

	SSVUT_EXPECT(tm.empty());
	SSVUT_EXPECT(tm.size() == 0);

	tm[0] = 0;
	tm[1] = 2;
	tm[2] = 4;

	SSVUT_EXPECT(!tm.empty());
	SSVUT_EXPECT(tm.size() == 3);

	SSVUT_EXPECT(tm[0] == 0 && tm.at(0) == 0);
	SSVUT_EXPECT(tm[1] == 2 && tm.at(1) == 2);
	SSVUT_EXPECT(tm[2] == 4 && tm.at(2) == 4);

	auto tmc(tm);

	SSVUT_EXPECT(tm == tmc);

	VecMap<int, int> tmil{{0, 0}, {1, 2}, {2, 4}};

	SSVUT_EXPECT(tm == tmil);

	auto tm2(std::move(tm));

	SSVUT_EXPECT(tm2[0] == 0 && tm2.at(0) == 0);
	SSVUT_EXPECT(tm2[1] == 2 && tm2.at(1) == 2);
	SSVUT_EXPECT(tm2[2] == 4 && tm2.at(2) == 4);

	tm2[0] = 5;
	SSVUT_EXPECT(tm2[0] == 5 && tm2.at(0) == 5);
}

SSVUT_TEST(ContainerVecMapNonPODTests)
{
	using namespace ssvu;

	VecMap<std::string, std::string> tm;

	SSVUT_EXPECT(tm.empty());
	SSVUT_EXPECT(tm.size() == 0);

	tm.reserve(3);

	SSVUT_EXPECT(tm.empty());
	SSVUT_EXPECT(tm.size() == 0);

	std::vector<std::string> words
	{
		"a",
		"klab",
		"eacbds",
		"haadfopja",
		"bdasaa",
		"aasdfpoasfas"
	};

	for(const auto& w : words) tm[w] = w + "val";

	SSVUT_EXPECT(!tm.empty());
	SSVUT_EXPECT(tm.size() == words.size());

	for(const auto& w : words)
	{
		SSVUT_EXPECT_OP(tm[w], ==, w + "val");
	}

	SSVUT_EXPECT_OP(tm.atOrDefault("banana"), ==, "");

	const auto& tv(tm["a"]);
	SSVUT_EXPECT(tv == "aval");

	tm.clear();

	SSVUT_EXPECT(tm.empty());
	SSVUT_EXPECT(tm.size() == 0);
}

SSVUT_TEST(ContainerVecSortedTests)
{
	using namespace ssvu;

	VecSorted<std::string> vs;

	SSVUT_EXPECT(vs.empty());
	SSVUT_EXPECT(vs.size() == 0);

	vs.reserve(5);

	SSVUT_EXPECT(vs.empty());
	SSVUT_EXPECT(vs.size() == 0);

	std::vector<std::string> words
	{
		"a",
		"klab",
		"eacbds",
		"haadfopja",
		"bdasaa",
		"aasdfpoasfas"
	};

	for(const auto& w : words) vs.insert(w);

	sort(words);

	for(auto i(0u); i < words.size(); ++i)
	{
		SSVUT_EXPECT_OP(words[i], ==, vs[i]);
	}

	vs.clear();

	SSVUT_EXPECT(vs.empty());
	SSVUT_EXPECT(vs.size() == 0);
}

#endif
