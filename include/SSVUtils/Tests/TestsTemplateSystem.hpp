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
	SSVUT_EXPECT_OP(r, ==, R"({{ Hello, my name is Meow. My skills are meowing, jumping, dancing.)");

	// Test basic expansion, maintaining "empty" keys
	auto r2(d.getExpanded(src, Settings::MaintainUnexisting));
	SSVUT_EXPECT_OP(r2, ==, R"({{ Hello, my name is Meow.{{null}} My skills are meowing, jumping, dancing.)");

	// Test basic expansion, maintaining only content from "empty" keys
	auto r3(d.getExpanded(src, Settings::MaintainUnexistingOnlyCnt));
	SSVUT_EXPECT_OP(r3, ==, R"({{ Hello, my name is Meow. My skills are meowing, jumping, dancing.)");
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
	SSVUT_EXPECT_OP(r, ==, R"(Test: end)");
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
	SSVUT_EXPECT_OP(r, ==, R"(MeowWoof0MeowWoof1)");
}

SSVUT_TEST(TemplateSystemTestsEdgeCases)
{
	using namespace std;
	using namespace ssvu;
	using namespace ssvu::TemplateSystem;

	{
		string src{R"(\{{test}}{{test}}\\{{test}})"};
		Dictionary d{"test", "exp"};
		SSVUT_EXPECT_OP(d.getExpanded(src), ==, R"({{test}}exp\{{test}})");
	}

	{
		string src{R"(\{{#test}}{{#test}}{{t}}{{/test}}\\{{/test}})"};
		Dictionary d{"test",	Dictionary::DictVec
								{
									{"t", "0"}, {"t", "1"}, {"t", "2"}
								}
		};
		SSVUT_EXPECT_OP(d.getExpanded(src), ==, R"({{#test}}012\{{/test}})");
	}

	{
		string src{R"(\{{}}{{}}\\{{}})"};
		Dictionary d{"", "exp"};
		SSVUT_EXPECT_OP(d.getExpanded(src), ==, R"({{}}exp\{{}})");
	}

	{
		string src{R"()"};
		Dictionary d{"", "exp"};
		SSVUT_EXPECT_OP(d.getExpanded(src), ==, R"()");
	}

	{
		string src{R"({{}})"};
		Dictionary d{"x", "y"};
		SSVUT_EXPECT_OP(d.getExpanded(src), ==, R"()");
		SSVUT_EXPECT_OP(d.getExpanded(src, Settings::MaintainUnexisting), ==, R"({{}})");
		SSVUT_EXPECT_OP(d.getExpanded(src, Settings::MaintainUnexistingOnlyCnt), ==, R"()");
	}

	{
		string src{R"(abcd{{#xsect}}efgh{{/xsect}}ilmn)"};
		Dictionary d{};
		SSVUT_EXPECT_OP(d.getExpanded(src), ==, R"(abcdilmn)");
		SSVUT_EXPECT_OP(d.getExpanded(src, Settings::MaintainUnexisting), ==, R"(abcd{{#xsect}}efgh{{/xsect}}ilmn)");
		SSVUT_EXPECT_OP(d.getExpanded(src, Settings::MaintainUnexistingOnlyCnt), ==, R"(abcdefghilmn)");
	}
}

#endif
