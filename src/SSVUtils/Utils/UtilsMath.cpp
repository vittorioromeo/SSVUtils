// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <algorithm>
#include "SSVUtils/Utils/UtilsMath.h"

using namespace std;

namespace ssvu
{
	int getRnd(int mMin, int mMax) { double x{rand() / static_cast<double>(RAND_MAX)}; return mMin + static_cast<int>(x * (mMax - mMin)); }
}
