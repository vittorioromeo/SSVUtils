// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Do.h"
#include "Timeline.h"

using namespace std;

namespace ssvu
{
	Do::Do(Timeline& mTimeline, Action mAction) : Command{mTimeline}, action{mAction} { }
	void Do::update(float) { action(); timeline.next(); }
}
