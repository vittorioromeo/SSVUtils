// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_RANGE
#define SSVU_TESTS_RANGE

SSVUT_TEST(RangeTests)
{
	using namespace ssvu;

	int valsr[]{6, 5, 4, 3, 2, 1};

	std::array<int, 6> ca{1, 2, 3, 4, 5, 6};
	std::vector<int> cv{1, 2, 3, 4, 5, 6};
	std::list<int> cl{1, 2, 3, 4, 5, 6};

	#define EXEC_TEST(mC) \
		{ \
			int k{0}; \
			for(const auto& i : asRange(mC)) k += i; \
			SSVUT_EXPECT(k == 21); k = 0; \
			for(auto& i : asRange(mC)) k += i; \
			SSVUT_EXPECT(k == 21); k = 0; \
			for(const auto& i : asRangeReverse(mC)) SSVUT_EXPECT(i == valsr[k++]); \
			k = 0; \
			for(auto& i : asRangeReverse(mC)) SSVUT_EXPECT(i == valsr[k++]); \
		}

	EXEC_TEST(ca);
	EXEC_TEST(cv);
	EXEC_TEST(cl);

	#undef EXEC_TEST
}

SSVUT_TEST(RangeCastTests)
{
	using namespace ssvu;

	struct XTBase
	{
		inline ~XTBase() { }
	};
	struct XTDer : public XTBase
	{
		int n{1};
		inline int f(int mK) const { return mK + n; }
	};

	std::vector<UPtr<XTBase>> v;
	v.emplace_back(makeUPtr<XTDer>());
	v.emplace_back(makeUPtr<XTDer>());
	v.emplace_back(makeUPtr<XTDer>());
	v.emplace_back(makeUPtr<XTDer>());
	v.emplace_back(makeUPtr<XTDer>());

	for(const auto& i : asRangeCastPtr<const XTDer&>(v)) { SSVUT_EXPECT(i.f(2) == 3); }
	for(auto& i : asRangeCastPtr<XTDer&>(v)) { i.n = 3; SSVUT_EXPECT(i.f(2) == 5); }



	std::list<UPtr<XTBase>> l;
	l.emplace_back(makeUPtr<XTDer>());
	l.emplace_back(makeUPtr<XTDer>());
	l.emplace_back(makeUPtr<XTDer>());
	l.emplace_back(makeUPtr<XTDer>());
	l.emplace_back(makeUPtr<XTDer>());

	for(const auto& i : asRangeCastPtr<const XTDer&>(l)) { SSVUT_EXPECT(i.f(2) == 3); }
	for(auto& i : asRangeCastPtr<XTDer&>(l)) { i.n = 3; SSVUT_EXPECT(i.f(2) == 5); }
}


#endif

