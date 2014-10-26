// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Benchmark/Inc/DataGroup.hpp"

namespace ssvu
{
	namespace Benchmark
	{
		namespace Internal
		{
			SSVU_INLINE void DataGroup::reset() noexcept
			{
				tp = HRClock::now();
				duration = Duration{0};
			}

			SSVU_INLINE void DataGroup::resume() noexcept
			{
				tp = HRClock::now();
			}

			SSVU_INLINE void DataGroup::pause() noexcept
			{
				duration += std::chrono::duration_cast<Duration>(HRClock::now() - tp);
			}

			SSVU_INLINE Duration DataGroup::getDuration() const noexcept
			{
				return duration;
			}
		}
	}
}

