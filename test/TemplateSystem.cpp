// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"

#include "SSVUtils/TemplateSystem/TemplateSystem.hpp"

#include "./utils/test_utils.hpp"
int main()
{
    {
        using namespace std;
        using namespace ssvu;
        using namespace ssvu::TemplateSystem;

        string src{
        R"(\{{ Hello, my name is {{name}}.{{null}} My skills are {{#skills}}{{skill}}{{*, }}{{/skills}}.)"};

        Dictionary d{
        "name", "Meow", "skills", Dictionary::DictVec{{"skill", "meowing"},
                                  {"skill", "jumping"}, {"skill", "dancing"}}};

        // Test basic expansion, removing "empty" keys
        auto r(d.getExpanded(src));
        TEST_ASSERT_OP(r, ==,
        R"({{ Hello, my name is Meow. My skills are meowing, jumping, dancing.)");

        // Test basic expansion, maintaining "empty" keys
        auto r2(d.getExpanded(src, Settings::MaintainUnexisting));
        TEST_ASSERT_OP(r2, ==,
        R"({{ Hello, my name is Meow.{{null}} My skills are meowing, jumping, dancing.)");

        // Test basic expansion, maintaining only content from "empty" keys
        auto r3(d.getExpanded(src, Settings::MaintainUnexistingOnlyCnt));
        TEST_ASSERT_OP(r3, ==,
        R"({{ Hello, my name is Meow. My skills are meowing, jumping, dancing.)");
    }
    {
        using namespace std;
        using namespace ssvu;
        using namespace ssvu::TemplateSystem;

        string src{R"(Test: {{step0}})"};

        Dictionary d{
        "step0", "{{step1}}", "step1", "{{step2}}", "step2", "end"};


        auto r(d.getExpanded(src));
        TEST_ASSERT_OP(r, ==, R"(Test: end)");
    }
    {
        using namespace std;
        using namespace ssvu;
        using namespace ssvu::TemplateSystem;

        string src{R"({{#sect}}{{parentVar}}{{childVar}}{{/sect}})"};

        Dictionary d{"parentVar", "Meow", "sect",
        Dictionary::DictVec{{"childVar", "Woof0"}, {"childVar", "Woof1"}}};

        auto r(d.getExpanded(src));
        TEST_ASSERT_OP(r, ==, R"(MeowWoof0MeowWoof1)");
    }

    {
        using namespace std;
        using namespace ssvu;
        using namespace ssvu::TemplateSystem;

        {
            string src{R"(\{{test}}{{test}}\\{{test}})"};
            Dictionary d{"test", "exp"};
            TEST_ASSERT_OP(d.getExpanded(src), ==, R"({{test}}exp\{{test}})");
        }

        {
            string src{R"(\{{#test}}{{#test}}{{t}}{{/test}}\\{{/test}})"};
            Dictionary d{
            "test", Dictionary::DictVec{{"t", "0"}, {"t", "1"}, {"t", "2"}}};
            TEST_ASSERT_OP(d.getExpanded(src), ==, R"({{#test}}012\{{/test}})");
        }

        {
            string src{R"(\{{}}{{}}\\{{}})"};
            Dictionary d{"", "exp"};
            TEST_ASSERT_OP(d.getExpanded(src), ==, R"({{}}exp\{{}})");
        }

        {
            string src{R"()"};
            Dictionary d{"", "exp"};
            TEST_ASSERT_OP(d.getExpanded(src), ==, R"()");
        }

        {
            string src{R"({{}})"};
            Dictionary d{"x", "y"};
            TEST_ASSERT_OP(d.getExpanded(src), ==, R"()");
            TEST_ASSERT_OP(d.getExpanded(src, Settings::MaintainUnexisting), ==,
            R"({{}})");
            TEST_ASSERT_OP(
            d.getExpanded(src, Settings::MaintainUnexistingOnlyCnt), ==,
            R"()");
        }

        {
            string src{R"(abcd{{#xsect}}efgh{{/xsect}}ilmn)"};
            Dictionary d{};
            TEST_ASSERT_OP(d.getExpanded(src), ==, R"(abcdilmn)");
            TEST_ASSERT_OP(d.getExpanded(src, Settings::MaintainUnexisting), ==,
            R"(abcd{{#xsect}}efgh{{/xsect}}ilmn)");
            TEST_ASSERT_OP(
            d.getExpanded(src, Settings::MaintainUnexistingOnlyCnt), ==,
            R"(abcdefghilmn)");
        }
    }
}