// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_TIMELINE_COMMAND
#define SSVU_IMPL_TIMELINE_COMMAND

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
    class Timeline;

    class Command
    {
        friend Timeline;

    protected:
        Timeline& timeline;

    public:
        Command(Timeline& mTimeline) noexcept;
        virtual ~Command();
        virtual void update(FT);
        virtual void reset();
    };
}

#endif
