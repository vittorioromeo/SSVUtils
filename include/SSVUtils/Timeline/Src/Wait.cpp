// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Timeline/Inc/Timeline.hpp"
#include "SSVUtils/Timeline/Inc/Wait.hpp"

namespace ssvu
{
SSVU_INLINE void Wait::update(FT mFT)
{
    timeline.ready = false;

    currentTime -= mFT;
    if(currentTime > 0) return;

    timeline.remainder = currentTime;
    timeline.next();
    reset();
}

SSVU_INLINE void Wait::reset()
{
    currentTime = time;
}
SSVU_INLINE Wait::Wait(Timeline& mTimeline, FT mTime) noexcept
    : Command{mTimeline}, time{mTime}, currentTime{mTime}
{
    SSVU_ASSERT(time >= 0);
}
} // namespace ssvu
