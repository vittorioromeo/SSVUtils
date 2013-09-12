// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TIMELINE_WAIT
#define SSVU_TIMELINE_WAIT

#include <cassert>
#include "SSVUtils/Timeline/Command.h"
#include "SSVUtils/Timeline/Timeline.h"

namespace ssvu
{
	class Wait : public Command
	{
		private:
			float time, currentTime;

		protected:
			inline void update(float mFrameTime) override
			{
				timeline.ready = false;

				currentTime -= mFrameTime;
				if(currentTime > 0) return;

				timeline.remainder = currentTime;
				timeline.next(); reset();
			}
			inline void reset() override { currentTime = time; }

		public:
			Wait(Timeline& mTimeline, float mTime) : Command{mTimeline}, time{mTime}, currentTime{mTime} { assert(time > 0); }
	};

	namespace Internal
	{
		template<bool TWhile> inline bool negate(bool mValue);
		template<> inline bool negate<false>(bool mValue) { return mValue; }
		template<> inline bool negate<true>(bool mValue) { return !mValue; }

		template<bool TWhile> class WaitLoop : public Command
		{
			private:	Predicate predicate;
			protected:	inline void update(float) override { timeline.ready = false; if(negate<TWhile>(predicate())) timeline.next(); }
			public:		WaitLoop(Timeline& mTimeline, const Predicate& mPredicate) : Command{mTimeline}, predicate{mPredicate} { }
		};
	}

	using WaitWhile = Internal::WaitLoop<true>;
	using WaitUntil = Internal::WaitLoop<false>;
}

#endif
