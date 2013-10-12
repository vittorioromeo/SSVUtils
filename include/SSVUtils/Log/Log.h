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
#include "SSVUtils/String/Utils.h"

namespace ssvu
{
	namespace FileSystem { class Path; }

	/// @brief Returns a reference to the log stream.
	std::ostringstream& getLogStream();

	namespace Internal
	{
		using CoutType = std::basic_ostream<char, std::char_traits<char>>;
		using StdEndLine = CoutType&(CoutType&);

		struct LOut
		{
			std::string lastTitle;
			template<typename T> inline LOut& operator()(const T& mValue)
			{
				#ifndef SSVU_LOG_DISABLE
					lastTitle = "[" + toStr(mValue) + "] ";
					std::cout << std::left << std::setw(38) << lastTitle;
					getLogStream() << std::left << std::setw(38) << lastTitle;
				#endif

				return *this;
			}
		};

		template<typename T> inline LOut& operator<<(LOut& mLOut, const T& mValue)
		{
			#ifndef SSVU_LOG_DISABLE
				std::cout << mValue; getLogStream() << mValue;
			#endif

			return mLOut;
		}
		template<> inline LOut& operator<<<LOut>(LOut& mLOut, const LOut& mValue)
		{
			#ifndef SSVU_LOG_DISABLE
				std::cout << mValue.lastTitle; getLogStream() << mValue.lastTitle;
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

	/// @brief Global log stream instance. Used like std::cout.
	static Internal::LOut lo;

	/// @brief Starts the benchmark timer.
	void startBenchmark();

	/// @brief Ends the benchmark timer and returns the elapsed time.
	/// @return Returns the elapsed time as a string.
	std::string endBenchmark();

	/// @brief Saves all the log entries to a file.
	/// @param mPath File path (file will be created if it doesn't exist).
	extern void saveLogToFile(const FileSystem::Path& mPath);
}

#include "SSVUtils/Log/Log.inl"

#endif
