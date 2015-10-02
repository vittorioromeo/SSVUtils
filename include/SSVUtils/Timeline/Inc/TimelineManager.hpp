// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVS_IMPL_TIMELINE_MANAGER
#define SSVS_IMPL_TIMELINE_MANAGER

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/MemoryManager/MemoryManager.hpp"
#include "SSVUtils/Timeline/Inc/Timeline.hpp"

namespace ssvu
{
class TimelineManager
{
private:
    MonoManager<Timeline> timelines;

public:
    auto& create();
    void update(FT mFT);
    void clear() noexcept;
};
}

#endif
