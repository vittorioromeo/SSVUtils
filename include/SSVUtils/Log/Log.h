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
#include "SSVUtils/Utils/UtilsString.h"

namespace ssvu
{
	std::vector<std::string>& getLogEntries();
	template<class T> void log(T mValue, std::string mTitle = "")
	{
		#ifndef SSVS_RELEASE
			std::ostringstream entryStream;

			if(mTitle != "") entryStream << "[" << mTitle << "] ";
			entryStream << toStr(mValue) << std::endl;

			std::cout << entryStream.str();
			getLogEntries().push_back(entryStream.str());
		#endif
	}

	void startBenchmark();
	std::string endBenchmark();

	void saveLogToFile(const std::string& mPath);
}

#endif
