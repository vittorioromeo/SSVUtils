// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_TICKER
#define SSVU_IMPL_TICKER

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	class Ticker
	{
		private:
			FT target, current{0.f}, total{0.f};
			bool running{true}, loop{true};
			SizeT ticks{0};

		public:
			Ticker(FT mTarget, bool mRunning = true) noexcept;

			bool update(FT) noexcept;
			bool update(FT mFT, FT mTarget) noexcept;

			void pause() noexcept;
			void resume() noexcept;
			void stop() noexcept;
			void restart() noexcept;
			void restart(FT mTarget) noexcept;

			void resetCurrent() noexcept;
			void resetTicks() noexcept;
			void resetTotal() noexcept;
			void resetAll() noexcept;

			void setLoop(bool mX) noexcept;

			bool getLoop() const noexcept;
			bool isRunning() const noexcept;
			FT getTarget() const noexcept;
			FT getCurrent() const noexcept;
			FT getTotal() const noexcept;
			SizeT getTicks() const noexcept;

			template<typename T = FT> inline T getTotalSecs() const noexcept	{ return static_cast<T>(getFTToSeconds(total)); }
			template<typename T = FT> inline T getCurrentSecs() const noexcept	{ return static_cast<T>(getFTToSeconds(current)); }
	};
}

#endif
