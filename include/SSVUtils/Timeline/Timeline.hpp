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

		public:
			using Idx = int;
			static constexpr Idx nullIdx{-1};

		private:
			PolyRecycler<Command> recycler;
			std::vector<decltype(recycler)::PtrType> commands;
			Idx cIdx{nullIdx};
			FT remainder{0.f};
			bool ready{true}, finished{false};

			inline bool isCIdxNull() const noexcept { return cIdx == nullIdx; }

			template<typename T, typename... TArgs> inline Idx insertImpl(Idx mIdx, TArgs&&... mArgs)
			{
				commands.emplace(std::begin(commands) + mIdx, recycler.create<T>(*this, fwd<TArgs>(mArgs)...));

				// If the current command is null, set it to the newly added one
				if(isCIdxNull()) cIdx = mIdx;

				return mIdx;
			}

			inline void next()
			{
				if(isCIdxNull()) return;

				++cIdx;
				auto itr(std::begin(commands) + cIdx);
				if(itr == std::end(commands)) cIdx = nullIdx;
			}

		public:
			inline Timeline(bool mStart = true) noexcept { if(!mStart) stop(); }

			template<typename T, typename... TArgs> inline auto append(TArgs&&... mArgs)			{ return insertImpl<T>(commands.size(), fwd<TArgs>(mArgs)...); }
			template<typename T, typename... TArgs> inline auto insert(Idx mIdx, TArgs&&... mArgs)	{ return insertImpl<T>(mIdx, fwd<TArgs>(mArgs)...); }

			inline void jumpTo(Idx mIdx) noexcept	{ cIdx = mIdx; }
			inline void start()	noexcept			{ finished = false; ready = true; }
			inline void clear()	noexcept			{ commands.clear(); cIdx = nullIdx; finished = true; }
			inline void stop() noexcept				{ finished = true; ready = false; }

			inline void update(FT mFT)
			{
				if(finished) return;
				ready = true;

				do
				{
					if(isCIdxNull()) { stop(); break; }
					commands[cIdx]->update(mFT + remainder);
					remainder = 0.f;
				} while(ready);
			}
			inline void reset()
			{
				start();
				for(const auto& c : commands) c->reset();
				cIdx = commands.empty() ? nullIdx : 0;
			}

			inline auto getSize() const noexcept	{ return commands.size(); }
			inline bool isFinished() const noexcept	{ return finished; }
			inline int getCurrentIndex() const
			{
				if(isCIdxNull()) return 0;
				return cIdx < static_cast<Idx>(commands.size()) ? cIdx : nullIdx;
			}
	};
}

#include "SSVUtils/Timeline/Do.hpp"
#include "SSVUtils/Timeline/Wait.hpp"
#include "SSVUtils/Timeline/Go.hpp"
#include "SSVUtils/Timeline/TimelineManager.hpp"

#endif
