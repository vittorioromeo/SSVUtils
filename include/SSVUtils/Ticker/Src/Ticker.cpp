// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Ticker/Inc/Ticker.hpp"

namespace ssvu
{
	SSVU_INLINE Ticker::Ticker(FT mTarget, bool mRunning) noexcept : target{mTarget}, running{mRunning} { }

	SSVU_INLINE bool Ticker::update(FT mFT) noexcept
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
	SSVU_INLINE bool Ticker::update(FT mFT, FT mTarget) noexcept { target = mTarget; return update(mFT); }

	SSVU_INLINE void Ticker::pause() noexcept				{ running = false; }
	SSVU_INLINE void Ticker::resume() noexcept				{ running = true; }
	SSVU_INLINE void Ticker::stop() noexcept				{ resetCurrent(); pause(); }
	SSVU_INLINE void Ticker::restart() noexcept				{ resetCurrent(); resume(); }
	SSVU_INLINE void Ticker::restart(FT mTarget) noexcept	{ target = mTarget; restart(); }

	SSVU_INLINE void Ticker::resetCurrent() noexcept	{ current = 0.f; }
	SSVU_INLINE void Ticker::resetTicks() noexcept		{ ticks = 0; }
	SSVU_INLINE void Ticker::resetTotal() noexcept		{ total = 0.f; }
	SSVU_INLINE void Ticker::resetAll() noexcept		{ resetCurrent(); resetTicks(); resetTotal(); }

	SSVU_INLINE void Ticker::setLoop(bool mX) noexcept	{ loop = mX; }

	SSVU_INLINE bool Ticker::getLoop() const noexcept	{ return loop; }
	SSVU_INLINE bool Ticker::isRunning() const noexcept	{ return running; }
	SSVU_INLINE FT Ticker::getTarget() const noexcept	{ return target; }
	SSVU_INLINE FT Ticker::getCurrent() const noexcept	{ return current; }
	SSVU_INLINE FT Ticker::getTotal() const noexcept	{ return total; }
	SSVU_INLINE SizeT Ticker::getTicks() const noexcept	{ return ticks; }
}
