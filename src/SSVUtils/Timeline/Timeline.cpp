// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Timeline/Timeline.h"
#include "SSVUtils/Timeline/Command.h"
#include "SSVUtils/Utils/UtilsContainers.h"

using namespace std;

namespace ssvu
{
	void Timeline::insert(unsigned int mIndex, Command& mCommand)
	{
		commands.insert(begin(commands) + mIndex, &mCommand);
		if(currentCommand == nullptr) currentCommand = &mCommand;
	}
	void Timeline::append(Command& mCommand) { insert(commands.size(), mCommand); }
	void Timeline::del(Command& mCommand) { eraseRemove(commands, &mCommand); commandManager.del(mCommand); }

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
	void Timeline::jumpTo(unsigned int mIndex) { currentCommand = commands[mIndex]; }

	void Timeline::reset()
	{
		start();
		for(const auto& c : commands) c->reset();
		currentCommand = commands.empty() ? nullptr : commands[0];
	}
	void Timeline::clear()
	{
		currentCommand = nullptr;
		commands.clear();
		finished = true;
	}

	void Timeline::next()
	{
		if(currentCommand == nullptr) return;

		auto itr(find(begin(commands), end(commands), currentCommand));
		if(itr == commands.end() - 1)
		{
			currentCommand = nullptr; // no more commands
			return;
		}
		else if(itr < end(commands) - 1 && itr >= begin(commands))
		{
			++itr;
			currentCommand = *itr;
		}
	}

	int Timeline::getCurrentIndex()
	{
		if(currentCommand == nullptr) return 0;
		unsigned int pos(find(begin(commands), end(commands), currentCommand) - begin(commands));
		return pos < commands.size() ? pos : -1;
	}
}
