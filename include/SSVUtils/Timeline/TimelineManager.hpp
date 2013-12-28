// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVS_TIMELINE_MANAGER
#define SSVS_TIMELINE_MANAGER

#include "SSVUtils/Timeline/Timeline.hpp"
#include "SSVUtils/MemoryManager/MemoryManager.hpp"

namespace ssvu
{
	class TimelineManager
	{
		private:
			MemoryManager<Timeline> timelines;

		public:
			inline Timeline& create() { return timelines.create(); }
			inline void update(FT mFT)
			{
				timelines.refresh();
				for(const auto& t : timelines) { t->update(mFT); if(t->isFinished()) timelines.del(*t); }
			}
			inline void clear() noexcept { timelines.clear(); }
	};
}

#endif
