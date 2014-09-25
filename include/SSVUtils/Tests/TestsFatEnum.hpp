// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSFATENUM
#define SSVU_TESTS_TESTSFATENUM

SSVU_FATENUM_MGR(_ssvutTestMgr);
SSVU_FATENUM_VALS(_ssvutTestMgr, _ssvutTestEnum, int, (A, 5), (B, 4), (C, -3))
SSVU_FATENUM_DEFS(_ssvutTestMgr, _ssvutTestEnumColors, int, Red, Green, Blue)
SSVUT_TEST(FatEnumTests)
{
	SSVUT_EXPECT(int(_ssvutTestEnum::A) == 5);
	SSVUT_EXPECT(int(_ssvutTestEnum::B) == 4);
	SSVUT_EXPECT(int(_ssvutTestEnum::C) == -3);

	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnum>::getAsString<_ssvutTestEnum::A>() == "A");
	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnum>::getAsString<_ssvutTestEnum::B>() == "B");
	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnum>::getAsString<_ssvutTestEnum::C>() == "C");

	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnum>::getAsString(_ssvutTestEnum::A) == "A");
	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnum>::getAsString(_ssvutTestEnum::B) == "B");
	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnum>::getAsString(_ssvutTestEnum::C) == "C");

	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnum>::getFromString("A") == _ssvutTestEnum::A);
	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnum>::getFromString("B") == _ssvutTestEnum::B);
	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnum>::getFromString("C") == _ssvutTestEnum::C);

	{
		std::string temp;
		for(auto v : _ssvutTestMgr<_ssvutTestEnum>::getElementNames()) temp += v;
		SSVUT_EXPECT(temp == "ABC");
	}

	{
		int temp{0};
		for(auto v : _ssvutTestMgr<_ssvutTestEnum>::getValues()) temp += int(v);
		SSVUT_EXPECT(temp == 6);
	}

	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnum>::getSize() == 3);

	SSVUT_EXPECT(int(_ssvutTestEnumColors::Red) == 0);
	SSVUT_EXPECT(int(_ssvutTestEnumColors::Green) == 1);
	SSVUT_EXPECT(int(_ssvutTestEnumColors::Blue) == 2);

	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnumColors>::getAsString<_ssvutTestEnumColors::Red>() == "Red");
	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnumColors>::getAsString<_ssvutTestEnumColors::Green>() == "Green");
	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnumColors>::getAsString<_ssvutTestEnumColors::Blue>() == "Blue");

	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnumColors>::getAsString(_ssvutTestEnumColors::Red) == "Red");
	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnumColors>::getAsString(_ssvutTestEnumColors::Green) == "Green");
	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnumColors>::getAsString(_ssvutTestEnumColors::Blue) == "Blue");

	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnumColors>::getFromString("Red") == _ssvutTestEnumColors::Red);
	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnumColors>::getFromString("Green") == _ssvutTestEnumColors::Green);
	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnumColors>::getFromString("Blue") == _ssvutTestEnumColors::Blue);

	SSVUT_EXPECT(_ssvutTestMgr<_ssvutTestEnumColors>::getSize() == 3);

	{
		std::string temp;
		for(auto v : _ssvutTestMgr<_ssvutTestEnumColors>::getElementNames()) temp += v;
		SSVUT_EXPECT(temp == "RedGreenBlue");
	}

	{
		int temp{0};
		for(auto v : _ssvutTestMgr<_ssvutTestEnumColors>::getValues()) temp += int(v);
		SSVUT_EXPECT(temp == 3);
	}
}

#endif
