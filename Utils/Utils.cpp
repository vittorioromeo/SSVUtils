// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Utils/Utils.h"

using namespace std;

namespace ssvu
{
	namespace Utils
	{
		void clearAndResetTimeline(Timeline& mTimeline) { mTimeline.clear(); mTimeline.reset(); }
	}
}
