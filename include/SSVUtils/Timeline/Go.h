// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TIMELINE_GO
#define SSVU_TIMELINE_GO

#include <cassert>
#include "SSVUtils/Timeline/Command.h"
#include "SSVUtils/Timeline/Timeline.h"

namespace ssvu
{
	class Go : public Command
	{
		protected:
			int targetIndex, times, currentTimes;

			inline void update(float) override
			{
				if(currentTimes == 0)		{ timeline.next(); }
				else if(currentTimes > 0)	{ --currentTimes; timeline.jumpTo(targetIndex); }
				else						{ timeline.jumpTo(targetIndex); }
			}
			inline void reset() override { currentTimes = times; }

		public:
			Go(Timeline& mTimeline, int mTargetIndex, int mTimes = -1) : Command{mTimeline}, targetIndex{mTargetIndex}, times{mTimes}, currentTimes{mTimes} { assert(targetIndex >= 0); }
	};
}

#endif
