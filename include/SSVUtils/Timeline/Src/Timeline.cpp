// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/MemoryManager/MemoryManager.hpp"
#include "SSVUtils/Timeline/Inc/Command.hpp"
#include "SSVUtils/Timeline/Inc/Timeline.hpp"

namespace ssvu
{
    SSVU_INLINE bool Timeline::isCIdxNull() const noexcept
    {
        return cIdx == nullIdx;
    }

    SSVU_INLINE void Timeline::next()
    {
        if(isCIdxNull()) return;

        ++cIdx;
        auto itr(std::begin(commands) + cIdx);
        if(itr == std::end(commands)) cIdx = nullIdx;
    }

    SSVU_INLINE Timeline::Timeline(bool mStart) noexcept
    {
        if(!mStart) stop();
    }

    SSVU_INLINE void Timeline::jumpTo(Idx mIdx) noexcept { cIdx = mIdx; }
    SSVU_INLINE void Timeline::start() noexcept
    {
        finished = false;
        ready = true;
    }
    SSVU_INLINE void Timeline::clear() noexcept
    {
        commands.clear();
        cIdx = nullIdx;
        finished = true;
    }
    SSVU_INLINE void Timeline::stop() noexcept
    {
        finished = true;
        ready = false;
    }

    SSVU_INLINE void Timeline::update(FT mFT)
    {
        if(finished) return;
        ready = true;

        do
        {
            if(isCIdxNull()) {
                stop();
                break;
            }
            commands[cIdx]->update(mFT + remainder);
            remainder = 0.f;
        } while(ready);
    }

    SSVU_INLINE void Timeline::reset()
    {
        start();
        for(const auto& c : commands) c->reset();
        cIdx = commands.empty() ? nullIdx : 0;
    }

    SSVU_INLINE auto Timeline::getSize() const noexcept
    {
        return commands.size();
    }
    SSVU_INLINE bool Timeline::isFinished() const noexcept { return finished; }
    SSVU_INLINE int Timeline::getCurrentIndex() const
    {
        if(isCIdxNull()) return 0;
        return cIdx < toNum<Idx>(commands.size()) ? cIdx : nullIdx;
    }
}
