// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSMAKEARRAY
#define SSVU_TESTS_TESTSMAKEARRAY

#define EXECTEST(mType) \
	{ \
		using TT = mType; \
		auto x = ssvu::make_array(TT(0), TT(1), TT(2)); \
		SSVU_ASSERT_STATIC_NM(ssvu::isSame<decltype(x), std::array<TT, 3>>()); \
		TT acc = TT(0); for(const auto& i : x) acc += TT(i); SSVUT_EXPECT(acc == TT(3)); \
	}

SSVUT_TEST(MakeArrayTests)
{
	EXECTEST(char)
	EXECTEST(int)
	EXECTEST(long int)
	EXECTEST(unsigned char)
	EXECTEST(unsigned int)
	EXECTEST(unsigned long int)
	EXECTEST(float)
	EXECTEST(double)
}

#undef EXECTEST

#endif
