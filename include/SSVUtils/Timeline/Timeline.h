// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TIMELINE
#define SSVU_TIMELINE

#include <vector>
#include <functional>
#include "SSVUtils/MemoryManager/MemoryManager.h"
#include "SSVUtils/Timeline/Command.h"
#include "SSVUtils/Utils/UtilsContainers.h"

namespace ssvu
{
	using Action = std::function<void()>;

	class Timeline : public MemoryManageable
	{
		friend class Wait;
		friend class Do;
		friend class Go;

		private:
			MemoryManager<Command> commandManager;
			std::vector<Command*> commands;
			Command* currentCommand{nullptr};
			bool ready{true}, finished{false};
			float remainder{0.f};

			Command& insert(unsigned int mIndex, Command& mCommand);
			inline Command& append(Command& mCommand) { return insert(commands.size(), mCommand); }
			void next();

			template<typename T, typename... TArgs> inline T& create(TArgs&&... mArgs)
			{
				return commandManager.create<T>(*this, std::forward<TArgs>(mArgs)...);
			}

		public:
			template<typename T, typename... TArgs> inline T& append(TArgs&&... mArgs)						{ return static_cast<T&>(append(create<T>(mArgs...))); }
			template<typename T, typename... TArgs> inline T& insert(unsigned int mIndex, TArgs&&... mArgs)	{ return static_cast<T&>(insert(mIndex, create<T>(mArgs...))); }

			inline void del(Command& mCommand)		{ eraseRemove(commands, &mCommand); commandManager.del(mCommand); }
			inline void jumpTo(unsigned int mIndex)	{ currentCommand = commands[mIndex]; }
			inline void start()						{ finished = false; ready = true; }
			inline void clear()						{ currentCommand = nullptr; commands.clear(); finished = true; }

			void update(float mFrameTime);
			void reset();

			inline std::size_t getSize() const	{ return commands.size(); }
			inline bool isFinished() const		{ return finished; }
			inline int getCurrentIndex() const
			{
				if(currentCommand == nullptr) return 0;
				auto pos(indexOf(commands, currentCommand));
				return pos < commands.size() ? pos : -1;
			}
	};
}

#endif
