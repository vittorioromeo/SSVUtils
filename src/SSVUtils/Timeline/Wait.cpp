// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Timeline/Wait.h"
#include "SSVUtils/Timeline/Timeline.h"

namespace ssvu
{
	Wait::Wait(Timeline& mTimeline, float mTime) : Command{mTimeline}, time{mTime}, currentTime{mTime} { }

	void Wait::update(float mFrameTime)
	{
		timeline.ready = false;

		currentTime -= mFrameTime;
		if(currentTime > 0) return;

		timeline.remainder = currentTime;
		timeline.next();
		reset();
	}
	void Wait::reset() { currentTime = time; }
}
