// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TIMELINE_WAIT
#define SSVU_TIMELINE_WAIT

namespace ssvu
{
	class Wait final : public Command
	{
		private:
			FT time, currentTime;

		protected:
			inline void update(FT mFT) override
			{
				timeline.ready = false;

				currentTime -= mFT;
				if(currentTime > 0) return;

				timeline.remainder = currentTime;
				timeline.next(); reset();
			}
			inline void reset() override { currentTime = time; }

		public:
			inline Wait(Timeline& mTimeline, FT mTime) noexcept : Command{mTimeline}, time{mTime}, currentTime{mTime} { SSVU_ASSERT(time >= 0); }
	};

	namespace Internal
	{
		template<bool TWhile> class WaitLoop final : public Command
		{
			private:
				Predicate predicate;

			protected:
				inline void update(FT) override { timeline.ready = false; if(predicate() != TWhile) timeline.next(); }

			public:
				inline WaitLoop(Timeline& mTimeline, const Predicate& mPredicate) : Command{mTimeline}, predicate{mPredicate} { }
		};
	}

	using WaitWhile = Internal::WaitLoop<true>;
	using WaitUntil = Internal::WaitLoop<false>;
}

#endif
