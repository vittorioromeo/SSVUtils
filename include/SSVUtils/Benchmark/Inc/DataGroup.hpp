// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_BENCHMARK_DATAGROUP
#define SSVU_IMPL_BENCHMARK_DATAGROUP

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Benchmark/Inc/Benchmark.hpp"

namespace ssvu
{
	namespace Benchmark
	{
		namespace Internal
		{
			/// @brief Benchmark group data structure storing a time point and a duration.
			struct DataGroup : public DataBase<DataGroup>
			{
				TP tp;
				Duration duration;

				void reset() noexcept;
				void resume() noexcept;
				void pause() noexcept;

				/// @brief Returns the elapsed time as a std::chrono::milliseconds.
				Duration getDuration() const noexcept;
			};
		}
	}
}

#endif
