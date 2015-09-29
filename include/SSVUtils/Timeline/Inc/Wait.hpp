// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_TIMELINE_WAIT
#define SSVU_IMPL_TIMELINE_WAIT

#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Timeline/Inc/Timeline.hpp"
#include "SSVUtils/Timeline/Inc/Command.hpp"

namespace ssvu
{
    class Wait final : public Command
    {
    private:
        FT time, currentTime;

    protected:
        void update(FT mFT) override;
        void reset() override;

    public:
        inline Wait(Timeline& mTimeline, FT mTime) noexcept;
    };

    namespace Impl
    {
        template <bool TWhile>
        class WaitLoop final : public Command
        {
        private:
            Predicate predicate;

        protected:
            inline void update(FT) override
            {
                timeline.ready = false;
                if(predicate() != TWhile) timeline.next();
            }

        public:
            inline WaitLoop(Timeline& mTimeline, const Predicate& mPredicate)
                : Command{mTimeline}, predicate{mPredicate}
            {
            }
        };
    }

    using WaitWhile = Impl::WaitLoop<true>;
    using WaitUntil = Impl::WaitLoop<false>;
}

#endif
