// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TIMELINE_GO
#define SSVU_TIMELINE_GO

#include "SSVUtils/Timeline/Command.h"

namespace ssvu
{
	class Go : public Command
	{
		protected:
			int targetIndex, times, currentTimes;

			void update(float mFrameTime) override;
			void reset() override;

		public:
			Go(Timeline& mTimeline, int mTargetIndex, int mTimes);
	};
}

#endif
