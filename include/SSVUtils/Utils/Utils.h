// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UTILS
#define SSVU_UTILS

#include <time.h>
#include "SSVUtils/Timeline/Timeline.h"

namespace ssvu
{
	/*!
	 *
	 * @brief Sets the random seed (uses srand);
	 *
	 * @param mSeed Seed to use. Default is "time(NULL)".
	 *
	 */
	void setRandomSeed(unsigned int mSeed = unsigned(time(NULL)));
}

#endif
