// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVUtils/Internal/API.hpp"
#include "SSVUtils/Core/Core.hpp"
#include "SSVUtils/Benchmark/Inc/Benchmark.hpp"
#include "SSVUtils/Benchmark/Inc/ImplEnabled.hpp"

namespace ssvu
{
	namespace Benchmark
	{
		namespace Internal
		{
			SSVU_INLINE auto& getStack() noexcept
			{
				thread_local std::vector<Data> result;
				return result;
			}

			SSVU_INLINE auto& hadNested() noexcept
			{
				thread_local bool result{false};
				return result;
			}

			SSVU_INLINE auto& getGroupMap() noexcept
			{
				thread_local std::map<std::string, DataGroup> result;
				return result;
			}

			SSVU_INLINE auto& getLastDataRef()
			{
				SSVU_ASSERT(!getStack().empty());
				return getStack().back();
			}

			SSVU_INLINE void start(std::string mTitle)
			{
				getStack().emplace_back(HRClock::now(), std::move(mTitle));
				if(getStack().size() > 1) hadNested() = true;
			}

			SSVU_INLINE Data getEndData()
			{
				// Makes a copy and pops from the stack
				auto result(getLastDataRef());
				getStack().pop_back();
				return result;
			}

			SSVU_INLINE void endLo()
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

			SSVU_INLINE void groupReset(const std::string& mGroup)	{ getGroupMap()[mGroup].reset(); }
			SSVU_INLINE void groupResume(const std::string& mGroup)	{ getGroupMap()[mGroup].resume(); }
			SSVU_INLINE void groupPause(const std::string& mGroup)	{ getGroupMap()[mGroup].pause(); }

			SSVU_INLINE void groupEndLo(const std::string& mGroup)
			{
				std::string logTitle{"BenchmarkGroup #"};
				lo(logTitle + mGroup) << getGroupMap()[mGroup].getString() << "\n";
				lo().flush();

				getGroupMap()[mGroup].reset();
			}
		}
	}
}
