// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Timeline/Inc/Timeline.hpp"
#include "SSVUtils/Timeline/Inc/TimelineManager.hpp"

namespace ssvu
{
	SSVU_INLINE void TimelineManager::update(FT mFT)
	{
		timelines.refresh();

		for(const auto& t : timelines)
		{
			t->update(mFT);
			if(t->isFinished()) timelines.del(*t);
		}
	}
	SSVU_INLINE auto& TimelineManager::create() { return timelines.create(); }
	SSVU_INLINE void TimelineManager::clear() noexcept { timelines.clear(); }
}
