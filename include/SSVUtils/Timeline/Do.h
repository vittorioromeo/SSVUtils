// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TIMELINE_DO
#define SSVU_TIMELINE_DO

#include <functional>
#include "SSVUtils/Timeline/Command.h"
#include "SSVUtils/Timeline/Timeline.h"

namespace ssvu
{
	class Do : public Command
	{
		protected:
			Action action;
			inline void update(float) override { action(); timeline.next(); }

		public:
			Do(Timeline& mTimeline, Action mAction) : Command{mTimeline}, action{mAction} { }
	};
}

#endif
