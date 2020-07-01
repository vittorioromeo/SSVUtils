// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_TIMELINE_GO
#define SSVU_IMPL_TIMELINE_GO

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Timeline/Inc/Timeline.hpp"
#include "SSVUtils/Timeline/Inc/Command.hpp"

namespace ssvu
{
namespace Impl
{
template <typename T>
class GoImpl final : public Command
{
protected:
    T target;
    std::size_t times, currentTimes;

    inline void update(FT) override
    {
        if(currentTimes == 0)
        {
            timeline.next();
        }
        else if(currentTimes > 0)
        {
            --currentTimes;
            timeline.jumpTo(target);
        }
        else
        {
            timeline.jumpTo(target);
        }
    }
    inline void reset() override
    {
        currentTimes = times;
    }

public:
    inline GoImpl(
        Timeline& mTimeline, T mTarget, std::size_t mTimes = -1) noexcept
        : Command{mTimeline},
          target(mTarget), times{mTimes}, currentTimes{mTimes}
    {
    }
};
} // namespace Impl

using Go = Impl::GoImpl<Timeline::Idx>;
} // namespace ssvu

#endif
