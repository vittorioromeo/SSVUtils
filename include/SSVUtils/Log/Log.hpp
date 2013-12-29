// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_LOG
#define SSVU_LOG

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ostream>
#include <fstream>
#include <chrono>
#include <unordered_map>
#include "SSVUtils/String/Utils.hpp"
#include "SSVUtils/Utils/Console.hpp"
#include "SSVUtils/String/Stringifier.hpp"

namespace ssvu
{
	namespace FileSystem { class Path; }

	namespace Internal
	{
		inline const std::string& getUniqueColor(const std::string& mStr)
		{
			static int lastColorIdx{2};
			static std::unordered_map<std::string, Console::Color> map;

			if(map.count(mStr) == 0) map[mStr] = Console::Color(getWrapIdx(lastColorIdx++, 2, 7));
			return Console::setColorFG(map[mStr]);
		}
	}

	/// @brief Returns a reference to the log stream.
	inline std::ostringstream& getLogStream() noexcept	{ static std::ostringstream logStream; return logStream; }

	namespace Internal
	{
		using TimePointHR = std::chrono::time_point<HRClock>;
		using CoutType = std::basic_ostream<char, std::char_traits<char>>;
		using StdEndLine = CoutType&(CoutType&);

		inline TimePointHR& getBenchStart() noexcept	{ static TimePointHR benchStart; return benchStart; }
		inline TimePointHR& getBenchEnd() noexcept		{ static TimePointHR benchEnd; return benchEnd; }

		struct LOut
		{
			std::string title;
			inline void flush() const { std::cout.flush(); getLogStream().flush(); }
		};

		template<typename T> inline LOut& operator<<(LOut& mLOut, const T& mValue)
		{
			#ifndef SSVU_LOG_DISABLE
				if(mLOut.title != "")
				{
					const auto& tStr("[" + mLOut.title + "] ");
					std::cout	<< Internal::getUniqueColor(mLOut.title) << Console::setStyle(Console::Style::Bold) << std::left << std::setw(38) << tStr;
					getLogStream() << std::left << std::setw(38) << tStr;
					mLOut.title = "";
				}

				std::cout << Console::resetFmt();
				Stringifier<T>::template impl<true>(std::cout, mValue);
				std::cout << Console::resetFmt();

				Stringifier<T>::template impl<false>(getLogStream(), mValue);
			#endif

			return mLOut;
		}
		inline LOut& operator<<(LOut& mLOut, StdEndLine manip)
		{
			#ifndef SSVU_LOG_DISABLE
				manip(std::cout); manip(getLogStream());
			#endif

			return mLOut;
		}

		inline LOut& getLOutInstance() noexcept { static LOut loInstance; return loInstance; }
	}

	/// @brief Returns a reference to the "log stream" singleton. (no title)
	inline Internal::LOut& lo() noexcept { return Internal::getLOutInstance(); }

	/// @brief Returns a reference to the "log stream" singleton. (sets title)
	/// @param mTitle Title of the next log message.
	template<typename T> inline Internal::LOut& lo(const T& mTitle)
	{
		lo().title = toStr(mTitle);
		return lo();
	}

	/// @brief Starts the benchmark timer.
	inline void startBenchmark()
	{
		#ifndef SSVU_LOG_DISABLE
			Internal::getBenchStart() = HRClock::now();
		#endif
	}

	/// @brief Ends the benchmark timer and returns the elapsed time.
	/// @return Returns the elapsed time as a std::chrono::milliseconds.
	inline std::chrono::milliseconds endBenchmarkAsMs()
	{
		#ifndef SSVU_LOG_DISABLE
			Internal::getBenchEnd() = HRClock::now();
			return std::chrono::duration_cast<std::chrono::milliseconds>(Internal::getBenchEnd() - Internal::getBenchStart());
		#endif

		return std::chrono::milliseconds(0);
	}

	/// @brief Ends the benchmark timer and returns the elapsed time.
	/// @return Returns the elapsed time as a string.
	inline std::string endBenchmark()
	{
		#ifndef SSVU_LOG_DISABLE
			return toStr(endBenchmarkAsMs().count()) + toStr(" ms");
		#endif

		return "";
	}

	/// @brief Saves all the log entries to a file.
	/// @param mPath File path (file will be created if it doesn't exist).
	void saveLogToFile(const FileSystem::Path& mPath);
}

#include "SSVUtils/FileSystem/FileSystem.hpp"

inline void ssvu::saveLogToFile(const ssvufs::Path& mPath)
{
	#ifndef SSVU_LOG_DISABLE
		std::ofstream o; o.open(mPath); o << ssvu::getLogStream().str(); o.flush(); o.close();
	#endif
}

#endif
