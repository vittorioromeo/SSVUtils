// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSOBFUSCATEDVAL
#define SSVU_TESTS_TESTSOBFUSCATEDVAL

SSVUT_TEST(ObfuscatedValueTests)
{
	using namespace std;
	using namespace ssvu;
	using namespace ssvu::Encryption;

	ObfuscatedValue<float> v{10.f};
	SSVUT_EXPECT(v.get() == 10.f);
	SSVUT_EXPECT(v == 10.f);
	v = 15.f;
	SSVUT_EXPECT(v.get() == 15.f);
	SSVUT_EXPECT(v == 15.f);
}

#endif
