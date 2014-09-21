// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_BENCHMARK_INTERNAL_IMPL_ENABLED
#define SSVU_BENCHMARK_INTERNAL_IMPL_ENABLED

namespace ssvu
{
	namespace Benchmark
	{
		namespace Internal
		{
			/// @brief Returns a reference to the static benchmark stack.
			inline auto& getStack() noexcept { static std::vector<Data> result; return result; }

			/// @brief Returns a reference to a statitc `bool` used to keep track if there was more than one benchmark on the stack.
			inline auto& hadNested() noexcept { static bool result{false}; return result; }

			/// @brief Returns a reference to the top of the stack. Assumes that the stack is not empty.
			inline auto& getLastDataRef() { SSVU_ASSERT(!getStack().empty()); return getStack().back(); }

			/// @brief Creates and pushes a new benchmark on the stack. If it's nested, sets `hasNested()` to true.
			inline void start(std::string mTitle)
			{
				getStack().emplace_back(HRClock::now(), std::move(mTitle));
				if(getStack().size() > 1) hadNested() = true;
			}

			/// @brief Pops and returns the top of the stack.
			inline auto getEndData()
			{
				// Makes a copy and pops from the stack
				auto result(getLastDataRef());
				getStack().pop_back();
				return result;
			}

			/// @brief Pops the top of the stack. Logs the benchmark on `ssvu::lo()`.
			/// @details Automatically adds a newline between "groups" of nested benchmarks.
			inline void endLo()
			{
				const auto& last(getEndData());

				std::string logTitle{"Benchmark #"};

				// Add 4 dashes for every level of nesting the benchmark has.
				if(getStack().size() > 0)
				{
					for(auto i(0u); i < getStack().size(); ++i) logTitle += "----";
					logTitle += "> ";
				}
				else logTitle += "^ ";

				lo(logTitle + last.name) << last.getString() << "\n";

				// If the benchmark was nested, add a newline
				if(getStack().empty() && hadNested())
				{
					hadNested() = false;
					lo() << "\n";
				}

				lo().flush();
			}

			/// @brief Returns a reference to the static benchmark group map.
			inline auto& getGroupMap() noexcept { static std::map<std::string, DataGroup> result; return result; }

			// Implementations of the benchmark group functions.
			inline void groupReset(const std::string& mGroup) { getGroupMap()[mGroup].reset(); }
			inline void groupResume(const std::string& mGroup) { getGroupMap()[mGroup].start(); }
			inline void groupPause(const std::string& mGroup) { getGroupMap()[mGroup].pause(); }
			inline void groupEndLo(const std::string& mGroup)
			{
				std::string logTitle{"BenchmarkGroup #"};
				lo(logTitle + mGroup) << getGroupMap()[mGroup].getString() << "\n";
				lo().flush();

				getGroupMap()[mGroup].reset();
			}
		}
	}
}

#endif
