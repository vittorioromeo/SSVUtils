// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Timeline/Timeline.h"
#include "SSVUtils/Timeline/Command.h"

using namespace std;

namespace ssvu
{
	Command& Timeline::insert(unsigned int mIndex, Command& mCommand)
	{
		commands.insert(begin(commands) + mIndex, &mCommand);
		if(currentCommand == nullptr) currentCommand = &mCommand;
		return mCommand;
	}

	void Timeline::update(float mFrameTime)
	{
		commandManager.refresh();

		if(finished) return;
		ready = true;

		do
		{
			if(currentCommand == nullptr) { finished = true; ready = false; break; }
			currentCommand->update(mFrameTime + remainder);
			remainder = 0;
		} while(ready);
	}

	void Timeline::reset()
	{
		start();
		for(const auto& c : commands) c->reset();
		currentCommand = commands.empty() ? nullptr : commands[0];
	}

	void Timeline::next()
	{
		if(currentCommand == nullptr) return;
		auto itr(find(commands, currentCommand));

		if(itr == end(commands) - 1) { currentCommand = nullptr; return; } // no more commands, return
		if(itr < end(commands) - 1 && itr >= begin(commands)) currentCommand = *++itr;
	}
}
