// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TIMELINE_COMMAND
#define SSVU_TIMELINE_COMMAND

#include "SSVUtils/MemoryManager/MemoryManager.h"

namespace ssvu
{
	class Timeline;

	class Command : public MemoryManageable
	{
		friend class Timeline;

		protected:
			Timeline& timeline;
			unsigned int index;

		public:
			Command(Timeline& mTimeline) : timeline(mTimeline) { }
			inline virtual ~Command()			{ }
			inline virtual void update(float)	{ }
			inline virtual void reset()			{ }

			inline unsigned int getIdx() const noexcept { return index; }
	};
}

#endif
