// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Benchmark/Inc/Benchmark.hpp"
#include "SSVUtils/Benchmark/Inc/Data.hpp"

#ifndef SSVU_BENCHMARK_DISABLE
	#include "SSVUtils/Benchmark/Inc/ImplEnabled.hpp"
#else
	#include "SSVUtils/Benchmark/Inc/ImplDisabled.hpp"
#endif

namespace ssvu
{
	namespace Benchmark
	{
		SSVU_INLINE void start(std::string mTitle)
		{
			return Internal::start(std::move(mTitle));
		}

		SSVU_INLINE Internal::Data getEndData()
		{
			return Internal::getEndData();
		}

		SSVU_INLINE void endLo()
		{
			Internal::endLo();
		}

		SSVU_INLINE void groupReset(const std::string& mGroup)	{ Internal::groupReset(mGroup); }
		SSVU_INLINE void groupResume(const std::string& mGroup)	{ Internal::groupResume(mGroup); }
		SSVU_INLINE void groupPause(const std::string& mGroup)	{ Internal::groupPause(mGroup); }
		SSVU_INLINE void groupEndLo(const std::string& mGroup)	{ Internal::groupEndLo(mGroup); }
	}
}
