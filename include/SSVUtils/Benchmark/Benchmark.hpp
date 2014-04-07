// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_BENCHMARK
#define SSVU_BENCHMARK

#include "SSVUtils/Core/Core.hpp"

namespace ssvu
{
	namespace Benchmark
	{
		namespace Internal
		{
			using TP = std::chrono::time_point<HRClock>;
			using Duration = std::chrono::milliseconds;

			struct Data
			{
				TP tp;
				std::string name;

				inline Data() = default;
				inline Data(const TP& mTP, std::string mName) : tp{mTP}, name{std::move(mName)} { }
			};

			using Stack = std::vector<Data>;

			inline Stack& getStack() noexcept
			{
				static Stack result;
				return result;
			}

			inline Data& getLastData()
			{
				SSVU_ASSERT(!Internal::getStack().empty());
				return getStack().back();
			}
		}

		/// @brief Starts the benchmark timer.
		inline void start(std::string mTitle = "")
		{
			#ifndef SSVU_BENCHMARK_DISABLE
				Internal::getStack().emplace_back(HRClock::now(), std::move(mTitle));
			#endif
		}

		/// @brief Ends the current benchmark timer and returns the data.
		/// @return Returns the elapsed time as a std::chrono::milliseconds.
		inline Internal::Data getEndData()
		{
			#ifndef SSVU_BENCHMARK_DISABLE
				auto last(Internal::getLastData());
				Internal::getStack().pop_back();
				return last;
			#endif

			return Internal::Data{};
		}

		/// @brief Ends the current benchmark timer and returns the elapsed time.
		/// @return Returns the elapsed time as a std::chrono::milliseconds.
		inline Internal::Duration getEndDuration()
		{
			#ifndef SSVU_BENCHMARK_DISABLE
				return std::chrono::duration_cast<Internal::Duration>(HRClock::now() - getEndData().tp);
			#endif

			return Internal::Duration{0};
		}

		/// @brief Ends the current benchmark timer and returns the elapsed time.
		/// @return Returns the elapsed time as a string.
		inline std::string getEndString()
		{
			#ifndef SSVU_BENCHMARK_DISABLE
				return toStr(getEndDuration().count()) + " ms";
			#endif

			return "";
		}

		/// @brief Ends the current benchmark timer and logs the elapsed time.
		inline void endLo()
		{
			#ifndef SSVU_BENCHMARK_DISABLE
				const auto& last(Internal::getLastData());
				lo("Benchmark #" + toStr(Internal::getStack().size()) + " - <" + last.name + ">") << getEndString() << std::endl;
			#endif
		}
	}
}

#endif
