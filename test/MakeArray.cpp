// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0


#include "SSVUtils/Core/Core.hpp"


#define EXECTEST(mType)                                                        \
    {                                                                          \
        using TT = mType;                                                      \
        auto x = ssvu::mkArray(TT(0), TT(1), TT(2));                           \
        SSVU_ASSERT_STATIC_NM(ssvu::isSame<decltype(x), std::array<TT, 3>>()); \
        TT acc = TT(0);                                                        \
        for(const auto& i : x) acc += TT(i);                                   \
        TEST_ASSERT(acc == TT(3));                                             \
    }

#include "./utils/test_utils.hpp"
int main()
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
