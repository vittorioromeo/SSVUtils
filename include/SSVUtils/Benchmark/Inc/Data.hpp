// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_IMPL_BENCHMARK_DATA
#define SSVU_IMPL_BENCHMARK_DATA

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Benchmark/Inc/Typedefs.hpp"
#include "SSVUtils/Benchmark/Inc/DataBase.hpp"

namespace ssvu
{
	namespace Benchmark
	{
		namespace Internal
		{
			/// @brief Benchmark data structure storing a time point and a name.
			struct Data : public DataBase<Data>
			{
				TP tp;
				std::string name;

				Data() = default;
				Data(const TP& mTP, std::string mName);

				/// @brief Returns the elapsed time as a std::chrono::milliseconds.
				Duration getDuration() const noexcept;
			};
		}
	}
}

#endif
