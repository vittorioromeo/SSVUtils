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
			MemoryManager<Timeline> memoryManager;

		public:
			TimelineManager() = default;

			inline Timeline& create() { return memoryManager.create(); }
			inline void update(float mFrameTime)
			{
				memoryManager.refresh();
				for(const auto& t : memoryManager) { t->update(mFrameTime); if(t->isFinished()) memoryManager.del(*t); }
			}
			inline void clear() { memoryManager.clear(); }
	};
}

#endif
