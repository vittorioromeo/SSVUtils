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

	class Timeline
	{
		friend class Do;
		friend class Wait;
		template<bool> friend class Internal::WaitLoop;
		template<typename> friend class Internal::GoImpl;

		private:
			PolyRecycler<Command> recycler;
			std::vector<decltype(recycler)::PtrType> commands;
			int cIdx{-1};
			FT remainder{0.f};
			bool ready{true}, finished{false};

			template<typename T, typename... TArgs> inline int insertImpl(std::size_t mIdx, TArgs&&... mArgs)
			{
				commands.emplace(std::begin(commands) + mIdx, recycler.create<T>(*this, fwd<TArgs>(mArgs)...));
				if(cIdx == -1) cIdx = mIdx;

				return mIdx;
			}

			inline void next()
			{
				if(cIdx == -1) return;
				auto itr(std::begin(commands) + cIdx);

				if(itr == std::end(commands) - 1) { cIdx = -1; return; } // no more commands, return
				if(itr < std::end(commands) - 1 && itr >= std::begin(commands)) ++cIdx;
			}

		public:
			inline Timeline(bool mStart = true) noexcept { if(!mStart) stop(); }

			template<typename T, typename... TArgs> inline auto append(TArgs&&... mArgs)					{ return insertImpl<T>(commands.size(), fwd<TArgs>(mArgs)...); }
			template<typename T, typename... TArgs> inline auto insert(std::size_t mIdx, TArgs&&... mArgs)	{ return insertImpl<T>(mIdx, fwd<TArgs>(mArgs)...); }

			//inline void del(Command& mCommand)				{ eraseRemoveIf(commands, [&mCommand](const auto& mC){ mC.get() == &mCommand; }); }
			inline void jumpTo(std::size_t mIdx) noexcept	{ cIdx = mIdx; }
			//inline void jumpTo(Command& mCommand) noexcept	{ cIdx = &mCommand; }
			inline void start()	noexcept					{ finished = false; ready = true; }
			inline void clear()	noexcept					{ cIdx = -1; commands.clear(); finished = true; }
			inline void stop() noexcept						{ finished = true; ready = false; }

			inline void update(FT mFT)
			{
				if(finished) return;
				ready = true;

				do
				{
					if(cIdx == -1) { stop(); break; }
					commands[cIdx]->update(mFT + remainder);
					remainder = 0;
				} while(ready);
			}
			inline void reset()
			{
				start();
				for(const auto& c : commands) c->reset();
				cIdx = commands.empty() ? -1 : 0;
			}

			inline auto getSize() const noexcept	{ return commands.size(); }
			inline bool isFinished() const noexcept	{ return finished; }
			inline int getCurrentIndex() const
			{
				if(cIdx == -1) return 0;
				return cIdx < static_cast<decltype(cIdx)>(commands.size()) ? cIdx : -1;
			}
	};
}

#include "SSVUtils/Timeline/Do.hpp"
#include "SSVUtils/Timeline/Wait.hpp"
#include "SSVUtils/Timeline/Go.hpp"
#include "SSVUtils/Timeline/TimelineManager.hpp"

#endif

// TODO: store index instead of pointer for current command?
// TODO: use linked list! (maybe intrusive?)
