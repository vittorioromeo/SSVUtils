// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TIMELINE
#define SSVU_TIMELINE

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/MemoryManager/MemoryManager.hpp"
#include "SSVUtils/Timeline/Command.hpp"

namespace ssvu
{
	namespace Internal
	{
		template<bool> class WaitLoop;
		template<typename> class GoImpl;
	}

	class Timeline : public MemoryManageable
	{
		friend class Do;
		friend class Wait;
		template<bool> friend class Internal::WaitLoop;
		template<typename> friend class Internal::GoImpl;

		private:
			MemoryManager<Command> commandManager;
			std::vector<Command*> commands;
			Command* currentCommand{nullptr};
			bool ready{true}, finished{false};
			FT remainder{0.f};

			template<typename T> inline T& insertImpl(std::size_t mIdx, T& mCommand)
			{
				commands.insert(std::begin(commands) + mIdx, &mCommand);
				if(currentCommand == nullptr) currentCommand = &mCommand;
				return mCommand;
			}
			inline void next()
			{
				if(currentCommand == nullptr) return;
				auto itr(find(commands, currentCommand));

				if(itr == std::end(commands) - 1) { currentCommand = nullptr; return; } // no more commands, return
				if(itr < std::end(commands) - 1 && itr >= std::begin(commands)) currentCommand = *++itr;
			}

			template<typename T, typename... TArgs> inline T& create(TArgs&&... mArgs) { return commandManager.create<T>(*this, std::forward<TArgs>(mArgs)...); }

		public:
			inline Timeline(bool mStart = true) noexcept { if(!mStart) stop(); }

			template<typename T, typename... TArgs> inline T& append(TArgs&&... mArgs)						{ return insertImpl(commands.size(), create<T>(mArgs...)); }
			template<typename T, typename... TArgs> inline T& insert(unsigned int mIdx, TArgs&&... mArgs)	{ return insertImpl(mIdx, create<T>(mArgs...)); }

			inline void del(Command& mCommand)				{ eraseRemove(commands, &mCommand); commandManager.del(mCommand); }
			inline void jumpTo(std::size_t mIdx) noexcept	{ currentCommand = commands[mIdx]; }
			inline void jumpTo(Command& mCommand) noexcept	{ currentCommand = &mCommand; }
			inline void start()	noexcept					{ finished = false; ready = true; }
			inline void clear()	noexcept					{ currentCommand = nullptr; commands.clear(); finished = true; }
			inline void stop() noexcept						{ finished = true; ready = false; }

			inline void update(FT mFT)
			{
				commandManager.refresh();

				if(finished) return;
				ready = true;

				do
				{
					if(currentCommand == nullptr) { stop(); break; }
					currentCommand->update(mFT + remainder);
					remainder = 0;
				} while(ready);
			}
			inline void reset()
			{
				start();
				for(const auto& c : commands) c->reset();
				currentCommand = commands.empty() ? nullptr : commands[0];
			}

			inline std::size_t getSize() const noexcept	{ return commands.size(); }
			inline bool isFinished() const noexcept		{ return finished; }
			inline int getCurrentIndex() const
			{
				if(currentCommand == nullptr) return 0;
				auto pos(idxOf(commands, currentCommand));
				return pos < commands.size() ? pos : -1;
			}
	};
}

#include "SSVUtils/Timeline/Do.hpp"
#include "SSVUtils/Timeline/Wait.hpp"
#include "SSVUtils/Timeline/Go.hpp"
#include "SSVUtils/Timeline/TimelineManager.hpp"

#endif

// TODO: add more commands, move to SSVS?
