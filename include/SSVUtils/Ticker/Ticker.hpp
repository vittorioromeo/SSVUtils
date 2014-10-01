// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TICKER
#define SSVU_TICKER

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	class Ticker
	{
		private:
			FT target, current{0.f}, total{0.f};
			bool running{true}, loop{true};
			std::size_t ticks{0};

		public:
			inline Ticker(FT mTarget, bool mRunning = true) noexcept : target{mTarget}, running{mRunning} { }

			inline bool update(FT mFT) noexcept
			{
				const auto& increment(mFT * running);
				current += increment;
				total += increment;

				if(current < target) return false;

				++ticks;
				resetCurrent();
				running = loop;
				return true;
			}
			inline bool update(FT mFT, FT mTarget) noexcept { target = mTarget; return update(mFT); }

			inline void pause() noexcept				{ running = false; }
			inline void resume() noexcept				{ running = true; }
			inline void stop() noexcept					{ resetCurrent(); running = false; }
			inline void restart() noexcept				{ resetCurrent(); running = true; }
			inline void restart(FT mTarget) noexcept	{ resetCurrent(); target = mTarget; running = true; }

			inline void resetCurrent() noexcept			{ current = 0.f; }
			inline void resetTicks() noexcept			{ ticks = 0; }
			inline void resetTotal() noexcept			{ total = 0.f; }
			inline void resetAll() noexcept				{ resetCurrent(); resetTicks(); resetTotal(); }

			inline void setLoop(bool mLoop) noexcept	{ loop = mLoop; }

			inline bool getLoop() const noexcept		{ return loop; }
			inline bool isRunning() const noexcept		{ return running; }
			inline auto getTarget() const noexcept		{ return target; }
			inline auto getCurrent() const noexcept		{ return current; }
			inline auto getTotal() const noexcept		{ return total; }
			inline auto getTicks() const noexcept		{ return ticks; }

			template<typename T = FT> inline T getTotalSecs() const noexcept	{ return static_cast<T>(getFTToSeconds(total)); }
			template<typename T = FT> inline T getCurrentSecs() const noexcept	{ return static_cast<T>(getFTToSeconds(current)); }
	};
}

#endif
