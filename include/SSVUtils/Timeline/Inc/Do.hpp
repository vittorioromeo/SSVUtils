// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_TIMELINE_DO
#define SSVU_IMPL_TIMELINE_DO

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Timeline/Inc/Command.hpp"

namespace ssvu
{
class Do final : public Command
{
protected:
    Action action;
    void update(FT) override;

public:
    Do(Timeline& mTimeline, const Action& mAction) noexcept;
};
}

#endif
