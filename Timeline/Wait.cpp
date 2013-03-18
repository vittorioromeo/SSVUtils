// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Wait.h"
#include "Timeline.h"

namespace ssvu
{
	Wait::Wait(Timeline& mTimeline, float mTime) : Command{mTimeline}, time{mTime}, currentTime{mTime} { }

	void Wait::update(float mFrameTime)
	{
		timeline.ready = false;

		currentTime -= mFrameTime;
		if(currentTime - mFrameTime > mFrameTime) return;

		timeline.next();
		reset();
	}
	void Wait::reset() { currentTime = time; }
}
