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
#include "SSVUtils/String/Utils.hpp"

namespace ssvu
{
	namespace FileSystem { class Path; }

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
					std::cout << std::left << std::setw(38) <<  "[" + mLOut.title + "] ";
					getLogStream() << std::left << std::setw(38) <<  "[" + mLOut.title + "] ";
					mLOut.title = "";
				}

				std::cout << mValue; getLogStream() << mValue;
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
	}

	// TODO: docs
	inline Internal::LOut& lo(const std::string& mTitle = "")
	{
		static Internal::LOut loInstance;
		loInstance.title = mTitle;
		return loInstance;
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

// TODO: include hell (if I remove line 116 everything breaks!)
#include "SSVUtils/FileSystem/FileSystem.hpp"
#include "SSVUtils/FileSystem/Path.hpp"

inline void ssvu::saveLogToFile(const ssvu::FileSystem::Path& mPath)
{
	#ifndef SSVU_LOG_DISABLE
		std::ofstream o; o.open(mPath); o << ssvu::getLogStream().str(); o.flush(); o.close();
	#endif
}

#endif
