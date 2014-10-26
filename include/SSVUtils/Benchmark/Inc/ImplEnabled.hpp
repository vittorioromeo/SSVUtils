// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_BENCHMARK_INTERNAL_IMPL_ENABLED
#define SSVU_BENCHMARK_INTERNAL_IMPL_ENABLED

#include <string>
#include "SSVUtils/Benchmark/Inc/Data.hpp"

namespace ssvu
{
	namespace Benchmark
	{
		namespace Internal
		{
			/// @brief Returns a reference to the thread_local benchmark stack.
			auto& getStack() noexcept;

			/// @brief Returns a reference to a static  thread_local `bool` used to keep track if there was more than one benchmark on the stack.
			auto& hadNested() noexcept;

			/// @brief Returns a reference to the thread_local benchmark group map.
			auto& getGroupMap() noexcept;

			/// @brief Returns a reference to the top of the stack. Assumes that the stack is not empty.
			auto& getLastDataRef();

			/// @brief Creates and pushes a new benchmark on the stack. If it's nested, sets `hasNested()` to true.
			void start(std::string mTitle);

			/// @brief Pops and returns the top of the stack.
			Data getEndData();

			/// @brief Pops the top of the stack. Logs the benchmark on `ssvu::lo()`.
			/// @details Automatically adds a newline between "groups" of nested benchmarks.
			void endLo();

			// Implementations of the benchmark group functions.
			void groupReset(const std::string& mGroup);
			void groupResume(const std::string& mGroup);
			void groupPause(const std::string& mGroup);
			void groupEndLo(const std::string& mGroup);
		}
	}
}

#endif
