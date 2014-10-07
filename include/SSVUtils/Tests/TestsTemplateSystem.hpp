// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSTEMPLATESYSTEM
#define SSVU_TESTS_TESTSTEMPLATESYSTEM

SSVUT_TEST(TemplateSystemTests)
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

	auto r(d.getExpanded(src));
	ssvu::lo() << r << "\n";
	SSVUT_EXPECT(r == R"(\{{ Hello, my name is Meow. My skills are meowing, jumping, dancing.)");
}

#endif
