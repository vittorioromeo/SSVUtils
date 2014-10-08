// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSTEMPLATESYSTEM
#define SSVU_TESTS_TESTSTEMPLATESYSTEM

SSVUT_TEST(TemplateSystemTests1)
{
	using namespace std;
	using namespace ssvu;
	using namespace ssvu::TemplateSystem;

	string src{R"(\{{ Hello, my name is {{name}}.{{null}} My skills are {{#skills}}{{skill}}{{*, }}{{/skills}}.)"};

	Dictionary d
	{
		"name",		"Meow",
		"skills",	Dictionary::DictVec
					{
						{"skill", "meowing"},
						{"skill", "jumping"},
						{"skill", "dancing"}
					}
	};

	// Test basic expansion, removing "empty" keys
	auto r(d.getExpanded(src));
	SSVUT_EXPECT(r == R"(\{{ Hello, my name is Meow. My skills are meowing, jumping, dancing.)");

	// Test basic expansion, maintaining "empty" keys
	auto r2(d.getExpanded(src, true));
	SSVUT_EXPECT(r2 == R"(\{{ Hello, my name is Meow.{{null}} My skills are meowing, jumping, dancing.)");
}

SSVUT_TEST(TemplateSystemTestsRecursion)
{
	using namespace std;
	using namespace ssvu;
	using namespace ssvu::TemplateSystem;

	string src{R"(Test: {{step0}})"};

	Dictionary d
	{
		"step0",	"{{step1}}",
		"step1",	"{{step2}}",
		"step2",	"end"
	};


	auto r(d.getExpanded(src));
	SSVUT_EXPECT(r == R"(Test: end)");
}

SSVUT_TEST(TemplateSystemTestsParentInheritance)
{
	using namespace std;
	using namespace ssvu;
	using namespace ssvu::TemplateSystem;

	string src{R"({{#sect}}{{parentVar}}{{childVar}}{{/sect}})"};

	Dictionary d
	{
		"parentVar",	"Meow",
		"sect",			Dictionary::DictVec
						{
							{"childVar", "Woof0"},
							{"childVar", "Woof1"}
						}
	};

	auto r(d.getExpanded(src));
	SSVUT_EXPECT(r == R"(MeowWoof0MeowWoof1)");
}

#endif

// TODO: empty case? escaped section?
