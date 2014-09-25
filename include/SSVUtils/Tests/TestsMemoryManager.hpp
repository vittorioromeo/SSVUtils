// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TESTS_TESTSMEMORYMANAGER
#define SSVU_TESTS_TESTSMEMORYMANAGER

SSVUT_TEST(MemoryManagerTests)
{
	volatile int cc{0};
	volatile int dc{0};

	struct TMMItem
	{
		volatile int& dcRef;
		inline TMMItem(volatile int& mCC, volatile int& mDC) : dcRef(mDC) { ++mCC; }
		inline virtual ~TMMItem() { ++dcRef; }
	};

	struct TMMItemS : public TMMItem { char stuff[2]; using TMMItem::TMMItem; };
	struct TMMItemB : public TMMItem { char stuff[20]; using TMMItem::TMMItem; };

	{
		SSVUT_EXPECT(cc == 0 && dc == 0);

		ssvu::MonoRecycler<TMMItem> mr;

		{
			auto i1 = mr.create(cc, dc);
			auto i2 = mr.create(cc, dc);
			mr.create(cc, dc); // dies immediately

			SSVUT_EXPECT(cc == 3 && dc == 1);
		}

		SSVUT_EXPECT(cc == 3 && dc == 3);
	}

	cc = 0; dc = 0;

	{
		SSVUT_EXPECT(cc == 0 && dc == 0);

		ssvu::PolyRecycler<TMMItem> pr;

		{
			auto i1 = pr.create<TMMItemS>(cc, dc);
			auto i2 = pr.create<TMMItemB>(cc, dc);
			pr.create<TMMItemS>(cc, dc);
			pr.create<TMMItemB>(cc, dc);

			SSVUT_EXPECT(cc == 4 && dc == 2);
		}

		SSVUT_EXPECT(cc == 4 && dc == 4);

		{
			auto i1 = pr.create<TMMItemS>(cc, dc);
			auto i2 = pr.create<TMMItemB>(cc, dc);
			pr.create<TMMItemS>(cc, dc);
			pr.create<TMMItemB>(cc, dc);

			SSVUT_EXPECT(cc == 8 && dc == 6);
		}

		SSVUT_EXPECT(cc == 8 && dc == 8);
	}

	cc = 0; dc = 0;

	{
		SSVUT_EXPECT(cc == 0 && dc == 0);

		ssvu::MonoManager<TMMItem> mm;

		{
			auto& i1 = mm.create(cc, dc);
			SSVUT_EXPECT(cc == 1 && dc == 0);

			auto& i2 = mm.create(cc, dc);
			SSVUT_EXPECT(cc == 2 && dc == 0);

			mm.create(cc, dc);
			SSVUT_EXPECT(cc == 3 && dc == 0);
		}

		mm.refresh();
		SSVUT_EXPECT(cc == 3 && dc == 0);

		for(auto& i : mm) mm.del(*i);
		SSVUT_EXPECT(cc == 3 && dc == 0);

		mm.refresh();
		SSVUT_EXPECT(cc == 3 && dc == 3);
	}

	cc = 0; dc = 0;

	{
		SSVUT_EXPECT(cc == 0 && dc == 0);

		ssvu::PolyManager<TMMItem> mm;

		{
			auto& i1 = mm.create<TMMItemS>(cc, dc);
			SSVUT_EXPECT(cc == 1 && dc == 0);

			auto& i2 = mm.create<TMMItemB>(cc, dc);
			SSVUT_EXPECT(cc == 2 && dc == 0);

			mm.create<TMMItemS>(cc, dc);
			SSVUT_EXPECT(cc == 3 && dc == 0);

			mm.create<TMMItemB>(cc, dc);
			SSVUT_EXPECT(cc == 4 && dc == 0);
		}

		mm.refresh();
		SSVUT_EXPECT(cc == 4 && dc == 0);

		for(auto& i : mm) mm.del(*i);
		SSVUT_EXPECT(cc == 4 && dc == 0);

		mm.refresh();
		SSVUT_EXPECT(cc == 4 && dc == 4);
	}
}

#endif
