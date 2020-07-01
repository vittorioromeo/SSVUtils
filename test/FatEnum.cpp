// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"

#include "SSVUtils/FatEnum/FatEnum.hpp"

SSVU_FATENUM_MGR(_ssvutTestMgr);
SSVU_FATENUM_VALS(_ssvutTestMgr, _ssvutTestEnum, int, (A, 5), (B, 4), (C, -3))
SSVU_FATENUM_DEFS(_ssvutTestMgr, _ssvutTestEnumColors, int, Red, Green, Blue)
#include "./utils/test_utils.hpp"

#include <string>

int main()
{
    TEST_ASSERT(int(_ssvutTestEnum::A) == 5);
    TEST_ASSERT(int(_ssvutTestEnum::B) == 4);
    TEST_ASSERT(int(_ssvutTestEnum::C) == -3);

    TEST_ASSERT(
        _ssvutTestMgr<_ssvutTestEnum>::getAsStr<_ssvutTestEnum::A>() == "A");
    TEST_ASSERT(
        _ssvutTestMgr<_ssvutTestEnum>::getAsStr<_ssvutTestEnum::B>() == "B");
    TEST_ASSERT(
        _ssvutTestMgr<_ssvutTestEnum>::getAsStr<_ssvutTestEnum::C>() == "C");

    TEST_ASSERT(
        _ssvutTestMgr<_ssvutTestEnum>::getAsStr(_ssvutTestEnum::A) == "A");
    TEST_ASSERT(
        _ssvutTestMgr<_ssvutTestEnum>::getAsStr(_ssvutTestEnum::B) == "B");
    TEST_ASSERT(
        _ssvutTestMgr<_ssvutTestEnum>::getAsStr(_ssvutTestEnum::C) == "C");

    TEST_ASSERT(
        _ssvutTestMgr<_ssvutTestEnum>::getFromStr("A") == _ssvutTestEnum::A);
    TEST_ASSERT(
        _ssvutTestMgr<_ssvutTestEnum>::getFromStr("B") == _ssvutTestEnum::B);
    TEST_ASSERT(
        _ssvutTestMgr<_ssvutTestEnum>::getFromStr("C") == _ssvutTestEnum::C);

    {
        std::string temp;
        for(auto v : _ssvutTestMgr<_ssvutTestEnum>::getElementNames())
            temp += v;
        TEST_ASSERT(temp == "ABC");
    }

    {
        int temp{0};
        for(auto v : _ssvutTestMgr<_ssvutTestEnum>::getValues()) temp += int(v);
        TEST_ASSERT(temp == 6);
    }

    TEST_ASSERT(_ssvutTestMgr<_ssvutTestEnum>::getSize() == 3);

    TEST_ASSERT(int(_ssvutTestEnumColors::Red) == 0);
    TEST_ASSERT(int(_ssvutTestEnumColors::Green) == 1);
    TEST_ASSERT(int(_ssvutTestEnumColors::Blue) == 2);

    TEST_ASSERT(_ssvutTestMgr<_ssvutTestEnumColors>::getAsStr<
                    _ssvutTestEnumColors::Red>() == "Red");
    TEST_ASSERT(_ssvutTestMgr<_ssvutTestEnumColors>::getAsStr<
                    _ssvutTestEnumColors::Green>() == "Green");
    TEST_ASSERT(_ssvutTestMgr<_ssvutTestEnumColors>::getAsStr<
                    _ssvutTestEnumColors::Blue>() == "Blue");

    TEST_ASSERT(_ssvutTestMgr<_ssvutTestEnumColors>::getAsStr(
                    _ssvutTestEnumColors::Red) == "Red");
    TEST_ASSERT(_ssvutTestMgr<_ssvutTestEnumColors>::getAsStr(
                    _ssvutTestEnumColors::Green) == "Green");
    TEST_ASSERT(_ssvutTestMgr<_ssvutTestEnumColors>::getAsStr(
                    _ssvutTestEnumColors::Blue) == "Blue");

    TEST_ASSERT(_ssvutTestMgr<_ssvutTestEnumColors>::getFromStr("Red") ==
                _ssvutTestEnumColors::Red);
    TEST_ASSERT(_ssvutTestMgr<_ssvutTestEnumColors>::getFromStr("Green") ==
                _ssvutTestEnumColors::Green);
    TEST_ASSERT(_ssvutTestMgr<_ssvutTestEnumColors>::getFromStr("Blue") ==
                _ssvutTestEnumColors::Blue);

    TEST_ASSERT(_ssvutTestMgr<_ssvutTestEnumColors>::getSize() == 3);

    {
        std::string temp;
        for(auto v : _ssvutTestMgr<_ssvutTestEnumColors>::getElementNames())
            temp += v;
        TEST_ASSERT(temp == "RedGreenBlue");
    }

    {
        int temp{0};
        for(auto v : _ssvutTestMgr<_ssvutTestEnumColors>::getValues())
            temp += int(v);
        TEST_ASSERT(temp == 3);
    }
}
