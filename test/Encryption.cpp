// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"

#include "SSVUtils/Encryption/Encryption.hpp"

#include "./utils/test_utils.hpp"
int main()
{
    using namespace std;
    using namespace ssvu;
    using namespace ssvu::Encryption;

    TEST_ASSERT(
        encrypt<Type::MD5>("testhash") == "082949a8dfacccda185a135db425377b");
    TEST_ASSERT(encrypt<Type::MD5>("") == "d41d8cd98f00b204e9800998ecf8427e");

    TEST_ASSERT(encrypt<Type::Base64>("testhash") == "dGVzdGhhc2g=");
    TEST_ASSERT(decrypt<Type::Base64>("dGVzdGhhc2g=") == "testhash");
}
