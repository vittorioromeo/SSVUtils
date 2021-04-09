// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Core/Core.hpp"

#include "SSVUtils/Core/Utils/FnTraits.hpp"

namespace ssvu
{
    namespace Impl
    {
        namespace TestUnused
        {
            inline void x0() {}
            inline void x1(int) {}
            inline void x2(int, char) {}
            inline int x3(double) { return 0; }
        }
    }
}

#include "./utils/test_utils.hpp"
TEST_MAIN()
{
    using namespace std;
    using namespace ssvu;
    using namespace ssvu::Impl::TestUnused;

    auto l0 = []()
    {
    };
    auto l1 = [](int)
    {
    };
    auto l2 = [](int, char)
    {
    };
    auto l3 = [](double) -> int
    {
        return 0;
    };

    static_assert(std::is_same_v<FnTraits<decltype(x0)>::Type, void()>);
    static_assert(std::is_same_v<FnTraits<decltype(x0)>::Return, void>);
    static_assert(FnTraits<decltype(x0)>::arity == 0);

    static_assert(std::is_same_v<FnTraits<decltype(l0)>::Type, void()>);
    static_assert(std::is_same_v<FnTraits<decltype(l0)>::Return, void>);
    static_assert(FnTraits<decltype(l0)>::arity == 0);



    static_assert(std::is_same_v<FnTraits<decltype(x1)>::Type, void(int)>);
    static_assert(std::is_same_v<FnTraits<decltype(x1)>::Return, void>);
    static_assert(std::is_same_v<FnTraits<decltype(x1)>::Arg<0>, int>);
    static_assert(FnTraits<decltype(x1)>::arity == 1);

    static_assert(std::is_same_v<FnTraits<decltype(l1)>::Type, void(int)>);
    static_assert(std::is_same_v<FnTraits<decltype(l1)>::Return, void>);
    static_assert(std::is_same_v<FnTraits<decltype(l1)>::Arg<0>, int>);
    static_assert(FnTraits<decltype(l1)>::arity == 1);



    static_assert(
        std::is_same_v<FnTraits<decltype(x2)>::Type, void(int, char)>);
    static_assert(std::is_same_v<FnTraits<decltype(x2)>::Return, void>);
    static_assert(std::is_same_v<FnTraits<decltype(x2)>::Arg<0>, int>);
    static_assert(std::is_same_v<FnTraits<decltype(x2)>::Arg<1>, char>);
    static_assert(FnTraits<decltype(x2)>::arity == 2);

    static_assert(
        std::is_same_v<FnTraits<decltype(l2)>::Type, void(int, char)>);
    static_assert(std::is_same_v<FnTraits<decltype(l2)>::Return, void>);
    static_assert(std::is_same_v<FnTraits<decltype(l2)>::Arg<0>, int>);
    static_assert(std::is_same_v<FnTraits<decltype(l2)>::Arg<1>, char>);
    static_assert(FnTraits<decltype(l2)>::arity == 2);



    static_assert(std::is_same_v<FnTraits<decltype(x3)>::Type, int(double)>);
    static_assert(std::is_same_v<FnTraits<decltype(x3)>::Return, int>);
    static_assert(std::is_same_v<FnTraits<decltype(x3)>::Arg<0>, double>);
    static_assert(FnTraits<decltype(x3)>::arity == 1);

    static_assert(std::is_same_v<FnTraits<decltype(l3)>::Type, int(double)>);
    static_assert(std::is_same_v<FnTraits<decltype(l3)>::Return, int>);
    static_assert(std::is_same_v<FnTraits<decltype(l3)>::Arg<0>, double>);
    static_assert(FnTraits<decltype(l3)>::arity == 1);
}
