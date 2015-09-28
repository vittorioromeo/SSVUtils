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

    ObfuscatedValue<float> v{10.f};
    TEST_ASSERT(v.get() == 10.f);
    TEST_ASSERT(v == 10.f);
    v = 15.f;
    TEST_ASSERT(v.get() == 15.f);
    TEST_ASSERT(v == 15.f);
}