// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVS_TIMELINE_MANAGER
#define SSVS_TIMELINE_MANAGER

#include "SSVUtils/Timeline/Timeline.h"
#include "SSVUtils/MemoryManager/MemoryManager.h"

namespace ssvu
{
	class TimelineManager
	{
		private:
			ssvu::MemoryManager<ssvu::Timeline> memoryManager;

		public:
			TimelineManager() = default;

			ssvu::Timeline& create();
			void update(float mFrameTime);
			void clear();
	};
}

#endif 
