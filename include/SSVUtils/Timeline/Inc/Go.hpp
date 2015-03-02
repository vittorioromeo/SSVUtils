// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_TIMELINE_GO
#define SSVU_IMPL_TIMELINE_GO

namespace ssvu
{
	namespace Impl
	{
		template<typename T> class GoImpl final : public Command
		{
			protected:
				T target;
				SizeT times, currentTimes;

				inline void update(FT) override
				{
					if(currentTimes == 0)		{ timeline.next(); }
					else if(currentTimes > 0)	{ --currentTimes; timeline.jumpTo(target); }
					else						{ timeline.jumpTo(target); }
				}
				inline void reset() override { currentTimes = times; }

			public:
				inline GoImpl(Timeline& mTimeline, T mTarget, SizeT mTimes = -1) noexcept : Command{mTimeline}, target(mTarget), times{mTimes}, currentTimes{mTimes} { }
		};
	}

	using Go = Impl::GoImpl<Timeline::Idx>;
}

#endif
