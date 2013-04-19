// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UTILS
#define SSVU_UTILS

#include <time.h>
#include "SSVUtils/Timeline/Timeline.h"

namespace ssvu
{
	void clearAndResetTimeline(Timeline& mTimeline);
	void setRandomSeed(unsigned int mSeed = unsigned(time(NULL)));
}

#endif
