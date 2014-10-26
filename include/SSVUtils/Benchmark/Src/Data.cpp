// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Benchmark/Inc/Benchmark.hpp"
#include "SSVUtils/Benchmark/Inc/Data.hpp"

namespace ssvu
{
	namespace Benchmark
	{
		namespace Internal
		{
			SSVU_INLINE Data::Data(const TP& mTP, std::string mName) : tp{mTP}, name{std::move(mName)}
			{

			}

			SSVU_INLINE Duration Data::getDuration() const noexcept
			{
				return std::chrono::duration_cast<Duration>(HRClock::now() - tp);
			}
		}
	}
}
