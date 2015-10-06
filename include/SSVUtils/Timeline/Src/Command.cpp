// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Timeline/Inc/Timeline.hpp"

namespace ssvu
{
    SSVU_INLINE Command::Command(Timeline& mTimeline) noexcept
        : timeline(mTimeline)
    {
    }
    SSVU_INLINE Command::~Command() {}
    SSVU_INLINE void Command::update(FT) {}
    SSVU_INLINE void Command::reset() {}
}
