// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Timeline/Inc/Timeline.hpp"
#include "SSVUtils/Timeline/Inc/Command.hpp"
#include "SSVUtils/Timeline/Inc/Do.hpp"

namespace ssvu
{
SSVU_INLINE Do::Do(
    Timeline& mTimeline, const std::function<void()>& mAction) noexcept
    : Command{mTimeline}, action{mAction}
{
}
SSVU_INLINE void Do::update(FT)
{
    action();
    timeline.next();
}
} // namespace ssvu
