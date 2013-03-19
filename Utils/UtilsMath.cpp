// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <algorithm>
#include "Utils/UtilsMath.h"

using namespace std;

namespace ssvu
{
	namespace Utils
	{
		int getRnd(int mMin, int mMax) { double x{rand()/static_cast<double>(RAND_MAX)}; return mMin + static_cast<int>(x * (mMax - mMin)); }

		float getSaturated(float mValue) { return max(0.0f, min(1.0f, mValue)); }
		float getSmootherStep(float edge0, float edge1, float x)
		{
			x = getSaturated((x - edge0)/(edge1 - edge0));
			return x*x*x*(x*(x*6 - 15) + 10);
		}
	}
}

