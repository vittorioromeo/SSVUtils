// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"


// MEMFN_DETECTORs (and other macros) must be defined outside lambdas/function
// bodies
SSVU_DEFINE_MEMFN_DETECTOR(SsvuTestMacroTestChecker1, hello)
SSVU_DEFINE_MEMFN_DETECTOR(SsvuTestMacroTestChecker2, hello2)
SSVU_DEFINE_MEMFN_DETECTOR(SsvuTestMacroTestChecker3, nohello)
SSVU_DEFINE_MEMFN_CALLER(SsvuTestMacroTestCallGoodbye1, goodbye1, int())
SSVU_DEFINE_MEMFN_CALLER(SsvuTestMacroTestCallGoodbye2, goodbye2, int())
#include "./utils/test_utils.hpp"
int main()
{
    SSVU_DEFINE_DUMMY_STRUCT(blah, test, abc);

    struct MacroTestStruct
    {
        void hello() {}
        float hello2(int) { return 3.f; }
        int goodbye1() { return 2; }
        int goodbye2() { return 4; }
    };

    MacroTestStruct mts;

    TEST_ASSERT(
    __R(SsvuTestMacroTestChecker1<MacroTestStruct, void()>()) == true);
    TEST_ASSERT(
    __R(SsvuTestMacroTestChecker2<MacroTestStruct, void()>()) == false);
    TEST_ASSERT(
    __R(SsvuTestMacroTestChecker2<MacroTestStruct, float(int)>()) == true);
    TEST_ASSERT(
    __R(SsvuTestMacroTestChecker3<MacroTestStruct, void()>()) == false);
    TEST_ASSERT(
    __R(SsvuTestMacroTestChecker3<MacroTestStruct, float(int)>()) == false);
    TEST_ASSERT(SsvuTestMacroTestCallGoodbye1(mts) == 2);
    TEST_ASSERT(SsvuTestMacroTestCallGoodbye2(mts) == 4);
}
