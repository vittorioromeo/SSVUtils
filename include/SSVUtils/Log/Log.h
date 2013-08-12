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
	namespace FileSystem
	{
		class Path;
	}

	std::ostringstream& getLogStream();

	namespace Internal
	{
		using CoutType = std::basic_ostream<char, std::char_traits<char>>;
		using StdEndLine = CoutType&(CoutType&);

		struct LOut	{ };
		struct LTitle { std::string str; };

		template<typename T> inline LOut& operator<<(LOut& mLOut, const T& mValue)
		{
			#ifndef SSVU_LOG_DISABLE
				std::cout << mValue; getLogStream() << mValue;
				return mLOut;
			#endif
		}
		template<> inline LOut& operator<<<LTitle>(LOut& mLOut, const LTitle& mValue)
		{
			#ifndef SSVU_LOG_DISABLE
				std::cout << std::left << std::setw(38) << mValue.str;
				getLogStream() << std::left << std::setw(38) << mValue.str;
				return mLOut;
			#endif
		}
		inline LOut& operator<<(LOut& mLOut, StdEndLine manip)
		{
			#ifndef SSVU_LOG_DISABLE
				manip(std::cout); manip(getLogStream()); return mLOut;
			#endif
		}
	}

	extern Internal::LOut lo;
	template<typename T> inline Internal::LTitle lt(const T& mValue)
	{
		#ifndef SSVU_LOG_DISABLE
			Internal::LTitle result; result.str = "[" + toStr(mValue) + "] "; return result;
		#endif
	}

	/*!
	 *
	 * @brief Starts the benchmark timer.
	 *
	 */
	void startBenchmark();

	/*!
	 *
	 * @brief Ends the benchmark timer and returns the elapsed time.
	 *
	 * @return Returns the elapsed time as a string.
	 *
	 */
	std::string endBenchmark();

	/*!
	 *
	 * @brief Saves all the log entries to a file.
	 *
	 * @param mPath File path (file will be created if it doesn't exist).
	 *
	 */
	extern void saveLogToFile(const FileSystem::Path& mPath);
}

#endif

// TODO: refactoring, cleanup and docs
