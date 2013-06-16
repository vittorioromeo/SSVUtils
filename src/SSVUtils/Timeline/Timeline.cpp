// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Timeline/Timeline.h"
#include "SSVUtils/Timeline/Command.h"
#include "SSVUtils/Utils/UtilsContainers.h"

using namespace std;

namespace ssvu
{
	Timeline::~Timeline() { clear(); }

	void Timeline::append(Command& mCommand)
	{
		commands.push_back(&mCommand);
		if(currentCommand == nullptr) currentCommand = &mCommand;
	}
	void Timeline::insert(int mIndex, Command& mCommand)
	{
		commands.insert(begin(commands) + mIndex, &mCommand);
		if(currentCommand == nullptr) currentCommand = &mCommand;
	}
	void Timeline::del(Command& mCommand) { eraseRemove(commands, &mCommand); commandManager.del(&mCommand); }

	void Timeline::update(float mFrameTime)
	{
		commandManager.cleanUp();

		if(finished) return;
		ready = true;

		do
		{
			if(currentCommand == nullptr)
			{
				finished = true;
				ready = false;
				break;
			}
			currentCommand->update(mFrameTime);
		} while(ready);
	}
	void Timeline::jumpTo(unsigned int mIndex) { currentCommand = commands[mIndex]; }

	void Timeline::reset()
	{
		start();
		for(const auto& c : commands) c->reset();

		if(!commands.empty()) currentCommand = commands[0];
		else currentCommand = nullptr;
	}
	void Timeline::clear()
	{
		currentCommand = nullptr;
		for(const auto& c : commands) commandManager.del(c);
		commands.clear();
		finished = true;
	}

	void Timeline::start() { finished = false; ready = true; }

	void Timeline::next()
	{
		if(currentCommand == nullptr) return;

		auto iter(find(begin(commands), end(commands), currentCommand));
		if(iter == commands.end() - 1)
		{
			currentCommand = nullptr; // no more commands
			return;
		}
		else if(iter < end(commands) - 1 && iter >= begin(commands))
		{
			iter++;
			currentCommand = *iter;
		}
	}

	bool Timeline::isFinished() { return finished; }
	int Timeline::getSize() { return commands.size(); }
	int Timeline::getCurrentIndex()
	{
		if(currentCommand == nullptr) return 0;
		unsigned int pos(find(begin(commands), end(commands), currentCommand) - begin(commands));
		return pos < commands.size() ? pos : -1;
	}
}
