// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TIMELINE_DO
#define SSVU_TIMELINE_DO

namespace ssvu
{
	class Do final : public Command
	{
		protected:
			Action action;
			inline void update(FT) override { action(); timeline.next(); }

		public:
			inline Do(Timeline& mTimeline, const Action& mAction) noexcept : Command{mTimeline}, action{mAction} { }
	};
}

#endif
