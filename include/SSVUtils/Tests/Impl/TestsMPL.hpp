// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSMPL
#define SSVU_TESTS_TESTSMPL

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Test/Test.hpp"

SSVUT_TEST(MPLTests)
{
	using namespace std;
	using namespace ssvu;
	using namespace ssvu::MPL;

	struct PT0 { };
	struct PT1 { };
	struct PT2 { };
	struct PT3 { };
	struct PT4 { };

	SSVU_ASSERT_STATIC_NM(getMin<int>(1, 5, 100, -20, -5, -100) == -100);
	SSVU_ASSERT_STATIC_NM(getMin<int>(0, 1) == 0);
	SSVU_ASSERT_STATIC_NM(getMin<int>(1) == 1);

	SSVU_ASSERT_STATIC_NM(getMax<int>(1, 5, 100, -20, -5, -100) == 100);
	SSVU_ASSERT_STATIC_NM(getMax<int>(0, 1) == 1);
	SSVU_ASSERT_STATIC_NM(getMax<int>(1) == 1);

	SSVU_ASSERT_STATIC_NM(isSame<MPL::Impl::VAHead<PT0, PT1, PT2>, PT0>());
	SSVU_ASSERT_STATIC_NM(isSame<MPL::Impl::VATail<PT0, PT1, PT2>, PT2>());

	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1, PT2>::Head, PT0>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1, PT2>::Tail, PT2>());
	SSVU_ASSERT_STATIC_NM(isSame<List<>::Head, Null>());
	SSVU_ASSERT_STATIC_NM(isSame<List<>::Tail, Null>());

	SSVU_ASSERT_STATIC_NM(isSame<List<>::AsTuple, Tpl<>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0>::AsTuple, Tpl<PT0>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1>::AsTuple, Tpl<PT0, PT1>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1, PT0>::AsTuple, Tpl<PT0, PT1, PT0>>());

	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1, PT2>::At<0>, PT0>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1, PT2>::At<1>, PT1>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1, PT2>::At<2>, PT2>());

	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1, PT2>::At<-1>, PT2>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1, PT2>::At<-2>, PT1>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1, PT2>::At<-3>, PT0>());

	SSVU_ASSERT_STATIC_NM(isSame<List<>::PushBack<PT0>, List<PT0>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0>::PushBack<PT1>, List<PT0, PT1>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1>::PushBack<PT2>, List<PT0, PT1, PT2>>());

	SSVU_ASSERT_STATIC_NM(isSame<List<>::PushFront<PT0>, List<PT0>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0>::PushFront<PT1>, List<PT1, PT0>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT1, PT0>::PushFront<PT2>, List<PT2, PT1, PT0>>());

	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1, PT2>::PopBack, List<PT0, PT1>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1>::PopBack, List<PT0>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0>::PopBack, List<>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<>::PopBack, List<>>());

	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1, PT2>::PopFront, List<PT1, PT2>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT1, PT2>::PopFront, List<PT2>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT2>::PopFront, List<>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<>::PopFront, List<>>());

	//						         0      1     2      3    4
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1, PT2, PT1, PT0>::SubList<0, 0>, List<>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1, PT2, PT1, PT0>::SubList<0, 1>, List<PT0>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1, PT2, PT1, PT0>::SubList<0, 3>, List<PT0, PT1, PT2>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1, PT2, PT1, PT0>::SubList<2, 4>, List<PT2, PT1>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1, PT2, PT1, PT0>::SubList<0, 999>, List<PT0, PT1, PT2, PT1, PT0>>());

	SSVU_ASSERT_STATIC_NM(List<PT0, PT1, PT2, PT1, PT0>::has<PT0>());
	SSVU_ASSERT_STATIC_NM(List<PT0, PT1, PT2, PT1, PT0>::has<PT1>());
	SSVU_ASSERT_STATIC_NM(List<PT0, PT1, PT2, PT1, PT0>::has<PT2>());
	SSVU_ASSERT_STATIC_NM(!List<PT0, PT1, PT2, PT1, PT0>::has<PT3>());
	SSVU_ASSERT_STATIC_NM(!List<PT0, PT1, PT2, PT1, PT0>::has<PT4>());
	SSVU_ASSERT_STATIC_NM(!List<>::has<PT0>());
	SSVU_ASSERT_STATIC_NM(!List<>::has<PT1>());

	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1>::Append<List<PT0, PT1>>, List<PT0, PT1, PT0, PT1>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1>::Append<List<>>, List<PT0, PT1>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<>::Append<List<PT0, PT1>>, List<PT0, PT1>>());

	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1>::Prepend<List<PT2, PT1>>, List<PT2, PT1, PT0, PT1>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<PT0, PT1>::Prepend<List<>>, List<PT0, PT1>>());
	SSVU_ASSERT_STATIC_NM(isSame<List<>::Prepend<List<PT0, PT1>>, List<PT0, PT1>>());

}

#endif
