// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_LOG
#define SSVU_LOG

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <ostream>
#include "SSVUtils/String/Utils.h"

namespace ssvu
{
	/*!
	 *
	 * @brief Gets all the previously entered log entries.
	 *
	 * @return Returns a reference to the internal log std::vector<string>.
	 *
	 */
	std::vector<std::string>& getLogEntries();

	/*!
	 *
	 * @brief Creates a new log entry and sends it to std::cout.
	 *
	 * Logging introduces a minimal runtime overhead - you can disable logging by defining SSVU_DISABLE_LOG.
	 *
	 * @tparam T Type of the value to log.
	 * @param mValue Value to log (gets internally converted to string).
	 * @param mTitle Title of the log entry (will be displayed between square brackets).
	 *
	 */
	template<class T> void log(const T& mValue, const std::string& mTitle = "")
	{
		#ifndef SSVU_LOG_DISABLE
			std::ostringstream entryStream;

			if(mTitle != "") entryStream << "[" << mTitle << "] ";
			entryStream << toStr(mValue) << std::endl;

			std::string str{entryStream.str()};
			std::cout << str;
			getLogEntries().push_back(str);
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
	void saveLogToFile(const std::string& mPath);
}

#endif
