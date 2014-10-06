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

	string src{R"(Hello, my name is {{name}}. My skills are {{#skills}}{{skill}}, {{/skills}})"};

	Dictionary d;
	d["name"] = "Meow";
	d += {"skills", {{"skill", "meowing"}}};
	d += {"skills", {{"skill", "jumping"}}};
	d += {"skills", {{"skill", "dancing"}}};

	auto r(d.getExpanded(src));

	SSVUT_EXPECT(r == R"(Hello, my name is Meow. My skills are meowing, jumping, dancing, )");
}

#endif
