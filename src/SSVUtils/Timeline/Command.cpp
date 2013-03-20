// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Timeline/Command.h"

namespace ssvu
{
	Command::Command(Timeline& mTimeline) : timeline(mTimeline) { }
	Command::~Command() { }
	void Command::update(float) { }
	void Command::reset() { }
}
