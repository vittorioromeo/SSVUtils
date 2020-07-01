// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0


#include "SSVUtils/Core/Core.hpp"

#include "SSVUtils/Range/Range.hpp"

#include "./utils/test_utils.hpp"
int main()
{
    {
        using namespace ssvu;

        int valsr[]{6, 5, 4, 3, 2, 1};

        std::array<int, 6> ca{{1, 2, 3, 4, 5, 6}};
        std::vector<int> cv{1, 2, 3, 4, 5, 6};
        std::list<int> cl{1, 2, 3, 4, 5, 6};

#define EXEC_TEST(mC)                            \
    {                                            \
        int k{0};                                \
        for(const auto& i : asRange(mC)) k += i; \
        TEST_ASSERT(k == 21);                    \
        k = 0;                                   \
        for(auto& i : asRange(mC)) k += i;       \
        TEST_ASSERT(k == 21);                    \
        k = 0;                                   \
        for(const auto& i : asRangeReverse(mC))  \
        {                                        \
            TEST_ASSERT_OP(i, ==, valsr[k]);     \
            ++k;                                 \
        }                                        \
        k = 0;                                   \
        for(auto& i : asRangeReverse(mC))        \
        {                                        \
            TEST_ASSERT_OP(i, ==, valsr[k]);     \
            ++k;                                 \
        }                                        \
    }

        EXEC_TEST(ca)
        EXEC_TEST(cv)
        EXEC_TEST(cl)

#undef EXEC_TEST
    }

    {
        using namespace ssvu;

        struct XTBase
        {
            inline virtual ~XTBase() {}
        };
        struct XTDer : public XTBase
        {
            int n{1};
            inline int f(int mK) const { return mK + n; }
        };

        std::vector<std::unique_ptr<XTBase>> v;
        v.emplace_back(mkUPtr<XTDer>());
        v.emplace_back(mkUPtr<XTDer>());
        v.emplace_back(mkUPtr<XTDer>());
        v.emplace_back(mkUPtr<XTDer>());
        v.emplace_back(mkUPtr<XTDer>());

        for(const auto& i : asRangeCastPtr<const XTDer&>(v))
        {
            TEST_ASSERT(i.f(2) == 3);
        }
        for(auto& i : asRangeCastPtr<XTDer&>(v))
        {
            i.n = 3;
            TEST_ASSERT(i.f(2) == 5);
        }



        std::list<std::unique_ptr<XTBase>> l;
        l.emplace_back(mkUPtr<XTDer>());
        l.emplace_back(mkUPtr<XTDer>());
        l.emplace_back(mkUPtr<XTDer>());
        l.emplace_back(mkUPtr<XTDer>());
        l.emplace_back(mkUPtr<XTDer>());

        for(const auto& i : asRangeCastPtr<const XTDer&>(l))
        {
            TEST_ASSERT(i.f(2) == 3);
        }
        for(auto& i : asRangeCastPtr<XTDer&>(l))
        {
            i.n = 3;
            TEST_ASSERT(i.f(2) == 5);
        }
    }
}
