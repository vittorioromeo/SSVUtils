// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TIMELINE_WAIT
#define SSVU_TIMELINE_WAIT

#include "SSVUtils/Timeline/Command.h"

namespace ssvu
{
	class Wait : public Command
	{
		protected:
			float time, currentTime;
			void update(float mFrameTime) override;
			void reset() override;

		public:
			Wait(Timeline& mTimeline, float mTime);
	};

}
#endif
