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

	void Timeline::append(Command* mCommandPtr)
	{
		commandPtrs.push_back(mCommandPtr);
		if(currentCommandPtr == nullptr) currentCommandPtr = mCommandPtr;
	}
	void Timeline::insert(int mIndex, Command* mCommandPtr)
	{
		commandPtrs.insert(commandPtrs.begin() + mIndex, mCommandPtr);
		if(currentCommandPtr == nullptr) currentCommandPtr = mCommandPtr;
	}
	void Timeline::del(Command* mCommandPtr)
	{
		eraseRemove(commandPtrs, mCommandPtr);
		delete mCommandPtr;
	}

	void Timeline::update(float mFrameTime)
	{
		if(finished) return;
		ready = true;

		do
		{
			if(currentCommandPtr == nullptr)
			{
				finished = true;
				ready = false;
				break;
			}
			currentCommandPtr->update(mFrameTime);
		} while(ready);
	}
	void Timeline::jumpTo(int mIndex) { currentCommandPtr = commandPtrs[mIndex]; }

	void Timeline::reset()
	{
		start();
		for(const auto& commandPtr : commandPtrs) commandPtr->reset();

		if(!commandPtrs.empty()) currentCommandPtr = commandPtrs[0];
		else currentCommandPtr = nullptr;
	}
	void Timeline::clear()
	{
		currentCommandPtr = nullptr;
		for(const auto& commandPtr : commandPtrs) delete commandPtr;
		commandPtrs.clear();
		finished = true;
	}

	void Timeline::start()
	{
		finished = false;
		ready = true;
	}

	void Timeline::next()
	{
		if(currentCommandPtr == nullptr) return;

		auto iter = find(commandPtrs.begin(), commandPtrs.end(), currentCommandPtr);
		if(iter == commandPtrs.end() - 1)
		{
			currentCommandPtr = nullptr; // no more commands
			return;
		}
		else if(iter < commandPtrs.end() - 1 && iter >= commandPtrs.begin())
		{
			iter++;
			currentCommandPtr = *iter;
		}
	}

	bool Timeline::isFinished() { return finished; }
	int Timeline::getSize() { return commandPtrs.size(); }
	int Timeline::getCurrentIndex()
	{
		if(currentCommandPtr == nullptr) return 0;
		unsigned int pos(find(begin(commandPtrs), end(commandPtrs), currentCommandPtr) - begin(commandPtrs));
		return pos < commandPtrs.size() ? pos : -1;
	}
}
