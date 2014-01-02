// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_TIMELINE_COMMAND
#define SSVU_TIMELINE_COMMAND

#include "SSVUtils/MemoryManager/MemoryManager.hpp"

namespace ssvu
{
	class Timeline;

	class Command : public MemoryManageable
	{
		friend class Timeline;

		protected:
			Timeline& timeline;

		public:
			Command(Timeline& mTimeline) noexcept : timeline(mTimeline) { }
			inline virtual ~Command()		{ }
			inline virtual void update(FT)	{ }
			inline virtual void reset()		{ }
	};
}

#endif
