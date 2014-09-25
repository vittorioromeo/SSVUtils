// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSENCRYPTION
#define SSVU_TESTS_TESTSENCRYPTION

SSVUT_TEST(EncryptionTests)
{
	using namespace std;
	using namespace ssvu;
	using namespace ssvu::Encryption;

	SSVUT_EXPECT(encrypt<Type::MD5>("testhash") == "082949a8dfacccda185a135db425377b");
	SSVUT_EXPECT(encrypt<Type::MD5>("") == "d41d8cd98f00b204e9800998ecf8427e");

	SSVUT_EXPECT(encrypt<Type::Base64>("testhash") == "dGVzdGhhc2g=");
	SSVUT_EXPECT(decrypt<Type::Base64>("dGVzdGhhc2g=") == "testhash");
}

#endif
