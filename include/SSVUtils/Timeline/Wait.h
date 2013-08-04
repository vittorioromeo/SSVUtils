// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TIMELINE_WAIT
#define SSVU_TIMELINE_WAIT

#include "SSVUtils/Timeline/Command.h"
#include "SSVUtils/Timeline/Timeline.h"

namespace ssvu
{
	class Wait : public Command
	{
		protected:
			float time, currentTime;

			inline void update(float mFrameTime) override
			{
				timeline.ready = false;

				currentTime -= mFrameTime;
				if(currentTime > 0) return;

				timeline.remainder = currentTime;
				timeline.next(); reset();
			}
			inline void reset() override { currentTime = time; }

		public:
			Wait(Timeline& mTimeline, float mTime) : Command{mTimeline}, time{mTime}, currentTime{mTime} { }
	};

}

#endif
