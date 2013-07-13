// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TIMELINE
#define SSVU_TIMELINE

#include <vector>
#include <functional>
#include "SSVUtils/MemoryManager/MemoryManager.h"
#include "SSVUtils/Timeline/Command.h"

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

			void insert(unsigned int mIndex, Command& mCommand);
			void append(Command& mCommand);
			void next();

			template<typename T, typename... TArgs> T& create(TArgs&&... mArgs)
			{
				return commandManager.create<T>(*this, std::forward<TArgs>(mArgs)...);
			}

		public:
			template<typename T, typename... TArgs> T& append(TArgs&&... mArgs)
			{
				T& result(create<T>(mArgs...)); append(result); return result;
			}
			template<typename T, typename... TArgs> T& insert(unsigned int mIndex, TArgs&&... mArgs)
			{
				T& result(create<T>(mArgs...)); insert(mIndex, result); return result;
			}

			void del(Command& mCommand);

			void update(float mFrameTime);
			void jumpTo(unsigned int mIndex);

			inline void start() { finished = false; ready = true; }
			void reset();
			void clear();

			inline int getSize()		{ return commands.size(); }
			inline bool isFinished()	{ return finished; }
			int getCurrentIndex();
	};
}

#endif
