// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVS_TIMELINE_MANAGER
#define SSVS_TIMELINE_MANAGER

namespace ssvu
{
	class TimelineManager
	{
		private:
			MonoManager<Timeline> timelines;

		public:
			inline auto& create() { return timelines.create(); }
			inline void update(FT mFT)
			{
				timelines.refresh();
				for(const auto& t : timelines) { t->update(mFT); if(t->isFinished()) timelines.del(*t); }
			}
			inline void clear() noexcept { timelines.clear(); }
	};
}

#endif
