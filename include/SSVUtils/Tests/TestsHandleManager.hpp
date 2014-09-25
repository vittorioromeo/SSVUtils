// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSHANDLEMANAGER
#define SSVU_TESTS_TESTSHANDLEMANAGER

SSVUT_TEST(HandleManagerMixedTests)
{
	using namespace ssvu;
	using namespace std;

	int cc{0}, dd{0};

	struct OTest
	{
		std::string s;
		int& rCC;
		int& rDD;

		OTest(int& mRCC, int& mRDD) : rCC(mRCC), rDD(mRDD) { ++rCC; }
		~OTest() { ++rDD; }
	};

	// Mixed elements
	{
		HandleVector<OTest> mgr;
		for(int k = 0; k < 2; ++k)
		{
			cc = dd = 0;

			auto a0(mgr.create(cc, dd));
			auto a1(mgr.create(cc, dd));
			auto a2(mgr.create(cc, dd));
			auto a3(mgr.create(cc, dd));
			auto a4(mgr.create(cc, dd));
			auto a5(mgr.create(cc, dd));
			auto a6(mgr.create(cc, dd));

			SSVUT_EXPECT(cc == 7);
			SSVUT_EXPECT(dd == 0);
			SSVUT_EXPECT(mgr.getSize() == 0);
			SSVUT_EXPECT(mgr.getSizeNext() == 7);

			mgr.refresh();

			SSVUT_EXPECT(cc == 7);
			SSVUT_EXPECT(dd == 0);
			SSVUT_EXPECT(mgr.getSize() == 7);
			SSVUT_EXPECT(mgr.getSizeNext() == 7);

			a0->s = "hi";
			a4->s = "ciao";
			a6->s = "bye";

			a2.destroy();
			a3.destroy();
			a5.destroy();

			SSVUT_EXPECT(cc == 7);
			SSVUT_EXPECT(dd == 0);
			SSVUT_EXPECT(mgr.getSize() == 7);
			SSVUT_EXPECT(mgr.getSizeNext() == 7);

			mgr.refresh();

			SSVUT_EXPECT(cc == 7);
			SSVUT_EXPECT(dd == 3);
			SSVUT_EXPECT(mgr.getSize() == 4);
			SSVUT_EXPECT(mgr.getSizeNext() == 4);

			SSVUT_EXPECT(a0->s == "hi");
			SSVUT_EXPECT(a4->s == "ciao");
			SSVUT_EXPECT(a6->s == "bye");

			SSVUT_EXPECT(a0.isAlive());
			SSVUT_EXPECT(a1.isAlive());
			SSVUT_EXPECT(!a2.isAlive());
			SSVUT_EXPECT(!a3.isAlive());
			SSVUT_EXPECT(a4.isAlive());
			SSVUT_EXPECT(!a5.isAlive());
			SSVUT_EXPECT(a6.isAlive());

			mgr.forEach([](OTest& mA){ mA.s += "bb"; });

			SSVUT_EXPECT(a0->s == "hibb");
			SSVUT_EXPECT(a4->s == "ciaobb");
			SSVUT_EXPECT(a6->s == "byebb");

			SSVUT_EXPECT(a0.isAlive());
			SSVUT_EXPECT(a1.isAlive());
			SSVUT_EXPECT(!a2.isAlive());
			SSVUT_EXPECT(!a3.isAlive());
			SSVUT_EXPECT(a4.isAlive());
			SSVUT_EXPECT(!a5.isAlive());
			SSVUT_EXPECT(a6.isAlive());

			auto aNew(mgr.create(cc, dd));
			aNew->s = "hehe";

			SSVUT_EXPECT(cc == 8);
			SSVUT_EXPECT(dd == 3);
			SSVUT_EXPECT(mgr.getSize() == 4);
			SSVUT_EXPECT(mgr.getSizeNext() == 5);

			mgr.refresh();

			SSVUT_EXPECT(cc == 8);
			SSVUT_EXPECT(dd == 3);
			SSVUT_EXPECT(mgr.getSize() == 5);
			SSVUT_EXPECT(mgr.getSizeNext() == 5);

			SSVUT_EXPECT(a0.isAlive());
			SSVUT_EXPECT(a1.isAlive());
			SSVUT_EXPECT(!a2.isAlive());
			SSVUT_EXPECT(!a3.isAlive());
			SSVUT_EXPECT(a4.isAlive());
			SSVUT_EXPECT(!a5.isAlive());
			SSVUT_EXPECT(a6.isAlive());
			SSVUT_EXPECT(aNew.isAlive());

			SSVUT_EXPECT(aNew->s == "hehe");

			a0.destroy();
			mgr.refresh();

			SSVUT_EXPECT(!a0.isAlive());
			SSVUT_EXPECT(a1.isAlive());
			SSVUT_EXPECT(!a2.isAlive());
			SSVUT_EXPECT(!a3.isAlive());
			SSVUT_EXPECT(a4.isAlive());
			SSVUT_EXPECT(!a5.isAlive());
			SSVUT_EXPECT(a6.isAlive());
			SSVUT_EXPECT(aNew.isAlive());

			SSVUT_EXPECT(cc == 8);
			SSVUT_EXPECT(dd == 4);
			SSVUT_EXPECT(mgr.getSize() == 4);
			SSVUT_EXPECT(mgr.getSizeNext() == 4);

			auto aSuicide(mgr.create(cc, dd));

			SSVUT_EXPECT(cc == 9);
			SSVUT_EXPECT(dd == 4);
			SSVUT_EXPECT(mgr.getSize() == 4);
			SSVUT_EXPECT(mgr.getSizeNext() == 5);

			aSuicide.destroy();

			SSVUT_EXPECT(cc == 9);
			SSVUT_EXPECT(dd == 4);
			SSVUT_EXPECT(mgr.getSize() == 4);
			SSVUT_EXPECT(mgr.getSizeNext() == 5);

			mgr.refresh();

			SSVUT_EXPECT(cc == 9);
			SSVUT_EXPECT(dd == 5);
			SSVUT_EXPECT(mgr.getSize() == 4);
			SSVUT_EXPECT(mgr.getSizeNext() == 4);

			mgr.clear();

			SSVUT_EXPECT(cc == 9);
			SSVUT_EXPECT(dd == 9);
		}
	}

	// All alive -> all dead -> all alive
	{
		HandleVector<OTest> mgr;
		for(int k = 0; k < 2; ++k)
		{
			cc = dd = 0;

			auto a0(mgr.create(cc, dd));
			auto a1(mgr.create(cc, dd));
			auto a2(mgr.create(cc, dd));
			auto a3(mgr.create(cc, dd));

			SSVUT_EXPECT(cc == 4);
			SSVUT_EXPECT(dd == 0);
			SSVUT_EXPECT(mgr.getSize() == 0);
			SSVUT_EXPECT(mgr.getSizeNext() == 4);

			SSVUT_EXPECT(a0.isAlive());
			SSVUT_EXPECT(a1.isAlive());
			SSVUT_EXPECT(a2.isAlive());
			SSVUT_EXPECT(a3.isAlive());

			mgr.refresh();

			SSVUT_EXPECT(cc == 4);
			SSVUT_EXPECT(dd == 0);
			SSVUT_EXPECT(mgr.getSize() == 4);
			SSVUT_EXPECT(mgr.getSizeNext() == 4);

			SSVUT_EXPECT(a0.isAlive());
			SSVUT_EXPECT(a1.isAlive());
			SSVUT_EXPECT(a2.isAlive());
			SSVUT_EXPECT(a3.isAlive());

			a0.destroy();
			a1.destroy();
			a2.destroy();
			a3.destroy();

			SSVUT_EXPECT(a0.isAlive());
			SSVUT_EXPECT(a1.isAlive());
			SSVUT_EXPECT(a2.isAlive());
			SSVUT_EXPECT(a3.isAlive());

			mgr.refresh();

			SSVUT_EXPECT(cc == 4);
			SSVUT_EXPECT(dd == 4);
			SSVUT_EXPECT(mgr.getSize() == 0);
			SSVUT_EXPECT(mgr.getSizeNext() == 0);

			SSVUT_EXPECT(!a0.isAlive());
			SSVUT_EXPECT(!a1.isAlive());
			SSVUT_EXPECT(!a2.isAlive());
			SSVUT_EXPECT(!a3.isAlive());

			mgr.refresh();

			SSVUT_EXPECT(cc == 4);
			SSVUT_EXPECT(dd == 4);
			SSVUT_EXPECT(mgr.getSize() == 0);
			SSVUT_EXPECT(mgr.getSizeNext() == 0);

			a0 = mgr.create(cc, dd);
			a1 = mgr.create(cc, dd);
			a2 = mgr.create(cc, dd);
			a3 = mgr.create(cc, dd);

			SSVUT_EXPECT(a0.isAlive());
			SSVUT_EXPECT(a1.isAlive());
			SSVUT_EXPECT(a2.isAlive());
			SSVUT_EXPECT(a3.isAlive());

			SSVUT_EXPECT(cc == 8);
			SSVUT_EXPECT(dd == 4);
			SSVUT_EXPECT(mgr.getSize() == 0);
			SSVUT_EXPECT(mgr.getSizeNext() == 4);

			mgr.refresh();

			SSVUT_EXPECT(a0.isAlive());
			SSVUT_EXPECT(a1.isAlive());
			SSVUT_EXPECT(a2.isAlive());
			SSVUT_EXPECT(a3.isAlive());

			SSVUT_EXPECT(cc == 8);
			SSVUT_EXPECT(dd == 4);
			SSVUT_EXPECT(mgr.getSize() == 4);
			SSVUT_EXPECT(mgr.getSizeNext() == 4);

			mgr.clear();

			SSVUT_EXPECT(!a0.isAlive());
			SSVUT_EXPECT(!a1.isAlive());
			SSVUT_EXPECT(!a2.isAlive());
			SSVUT_EXPECT(!a3.isAlive());

			SSVUT_EXPECT(cc == 8);
			SSVUT_EXPECT(dd == 8);
			SSVUT_EXPECT(mgr.getSize() == 0);
			SSVUT_EXPECT(mgr.getSizeNext() == 0);
		}
	}

	// Empty, one element
	{
		HandleVector<OTest> mgr;
		for(int k = 0; k < 2; ++k)
		{
			cc = dd = 0;

			SSVUT_EXPECT(cc == 0);
			SSVUT_EXPECT(dd == 0);
			SSVUT_EXPECT(mgr.getSize() == 0);
			SSVUT_EXPECT(mgr.getSizeNext() == 0);

			mgr.refresh();

			SSVUT_EXPECT(cc == 0);
			SSVUT_EXPECT(dd == 0);
			SSVUT_EXPECT(mgr.getSize() == 0);
			SSVUT_EXPECT(mgr.getSizeNext() == 0);

			auto a0(mgr.create(cc, dd));

			SSVUT_EXPECT(a0.isAlive());

			SSVUT_EXPECT(cc == 1);
			SSVUT_EXPECT(dd == 0);
			SSVUT_EXPECT(mgr.getSize() == 0);
			SSVUT_EXPECT(mgr.getSizeNext() == 1);

			mgr.refresh();

			SSVUT_EXPECT(a0.isAlive());

			SSVUT_EXPECT(cc == 1);
			SSVUT_EXPECT(dd == 0);
			SSVUT_EXPECT(mgr.getSize() == 1);
			SSVUT_EXPECT(mgr.getSizeNext() == 1);

			a0.destroy();

			SSVUT_EXPECT(a0.isAlive());

			SSVUT_EXPECT(cc == 1);
			SSVUT_EXPECT(dd == 0);
			SSVUT_EXPECT(mgr.getSize() == 1);
			SSVUT_EXPECT(mgr.getSizeNext() == 1);

			mgr.refresh();

			SSVUT_EXPECT(!a0.isAlive());

			SSVUT_EXPECT(cc == 1);
			SSVUT_EXPECT(dd == 1);
			SSVUT_EXPECT(mgr.getSize() == 0);
			SSVUT_EXPECT(mgr.getSizeNext() == 0);

			mgr.clear();

			SSVUT_EXPECT(!a0.isAlive());

			SSVUT_EXPECT(cc == 1);
			SSVUT_EXPECT(dd == 1);
			SSVUT_EXPECT(mgr.getSize() == 0);
			SSVUT_EXPECT(mgr.getSizeNext() == 0);
		}
	}

	// Iterator tests
	{
		HandleVector<OTest> mgr;
		auto a0 = mgr.create(cc, dd); a0->s = "a";
		auto a1 = mgr.create(cc, dd); a1->s = "b";
		auto a2 = mgr.create(cc, dd); a2->s = "c";

		{ std::string temp; for(auto a : mgr) temp += a.s;			SSVUT_EXPECT(temp == ""); }
		{ std::string temp; for(auto& a : mgr) temp += a.s;			SSVUT_EXPECT(temp == ""); }
		{ std::string temp; for(const auto& a : mgr) temp += a.s;	SSVUT_EXPECT(temp == ""); }
		{ std::string temp; for(auto itr(mgr.beginIdx()); itr != mgr.endIdx(); ++itr) temp += itr->s;		SSVUT_EXPECT(temp == ""); }
		{ std::string temp; for(auto itr(mgr.begin()); itr != mgr.endNext(); ++itr) temp += itr->s;			SSVUT_EXPECT(temp == "abc"); }
		{ std::string temp; for(auto itr(mgr.beginIdx()); itr != mgr.endIdxNext(); ++itr) temp += itr->s;	SSVUT_EXPECT(temp == "abc"); }

		mgr.refresh();

		{ std::string temp; for(auto a : mgr) temp += a.s;			SSVUT_EXPECT(temp == "abc"); }
		{ std::string temp; for(auto& a : mgr) temp += a.s;			SSVUT_EXPECT(temp == "abc"); }
		{ std::string temp; for(const auto& a : mgr) temp += a.s;	SSVUT_EXPECT(temp == "abc"); }
		{ std::string temp; for(auto itr(mgr.beginIdx()); itr != mgr.endIdx(); ++itr) temp += itr->s;		SSVUT_EXPECT(temp == "abc"); }
		{ std::string temp; for(auto itr(mgr.begin()); itr != mgr.endNext(); ++itr) temp += itr->s;			SSVUT_EXPECT(temp == "abc"); }
		{ std::string temp; for(auto itr(mgr.beginIdx()); itr != mgr.endIdxNext(); ++itr) temp += itr->s;	SSVUT_EXPECT(temp == "abc"); }

		auto a3 = mgr.create(cc, dd); a3->s = "d";

		{ std::string temp; for(auto a : mgr) temp += a.s;			SSVUT_EXPECT(temp == "abc"); }
		{ std::string temp; for(auto& a : mgr) temp += a.s;			SSVUT_EXPECT(temp == "abc"); }
		{ std::string temp; for(const auto& a : mgr) temp += a.s;	SSVUT_EXPECT(temp == "abc"); }
		{ std::string temp; for(auto itr(mgr.beginIdx()); itr != mgr.endIdx(); ++itr) temp += itr->s;		SSVUT_EXPECT(temp == "abc"); }
		{ std::string temp; for(auto itr(mgr.begin()); itr != mgr.endNext(); ++itr) temp += itr->s;			SSVUT_EXPECT(temp == "abcd"); }
		{ std::string temp; for(auto itr(mgr.beginIdx()); itr != mgr.endIdxNext(); ++itr) temp += itr->s;	SSVUT_EXPECT(temp == "abcd"); }

		mgr.refresh();

		{ std::string temp; for(auto a : mgr) temp += a.s;			SSVUT_EXPECT(temp == "abcd"); }
		{ std::string temp; for(auto& a : mgr) temp += a.s;			SSVUT_EXPECT(temp == "abcd"); }
		{ std::string temp; for(const auto& a : mgr) temp += a.s;	SSVUT_EXPECT(temp == "abcd"); }
		{ std::string temp; for(auto itr(mgr.beginIdx()); itr != mgr.endIdx(); ++itr) temp += itr->s;		SSVUT_EXPECT(temp == "abcd"); }
		{ std::string temp; for(auto itr(mgr.begin()); itr != mgr.endNext(); ++itr) temp += itr->s;			SSVUT_EXPECT(temp == "abcd"); }
		{ std::string temp; for(auto itr(mgr.beginIdx()); itr != mgr.endIdxNext(); ++itr) temp += itr->s;	SSVUT_EXPECT(temp == "abcd"); }
	}

	// AtomFromData tests
	{
		HandleVector<OTest> mgr;
		auto a0 = mgr.create(cc, dd); a0->s = "a";
		auto a1 = mgr.create(cc, dd); a1->s = "b";
		auto a2 = mgr.create(cc, dd); a2->s = "c";
		auto& d0 = *a0;
		auto& d1 = *a1;
		auto& d2 = *a2;

		SSVUT_EXPECT(mgr.getAtomFromData(d0).getData().s == "a");
		SSVUT_EXPECT(mgr.getAtomFromData(d1).getData().s == "b");
		SSVUT_EXPECT(mgr.getAtomFromData(d2).getData().s == "c");

		mgr.refresh();

		SSVUT_EXPECT(mgr.getAtomFromData(d0).getData().s == "a");
		SSVUT_EXPECT(mgr.getAtomFromData(d1).getData().s == "b");
		SSVUT_EXPECT(mgr.getAtomFromData(d2).getData().s == "c");

		mgr.reserve(100);
		// After reserving pointers may now be invalid, we need to use handles

		SSVUT_EXPECT(mgr.getAtomFromHandle(a0).getData().s == "a");
		SSVUT_EXPECT(mgr.getAtomFromHandle(a1).getData().s == "b");
		SSVUT_EXPECT(mgr.getAtomFromHandle(a2).getData().s == "c");
	}
}

#endif
