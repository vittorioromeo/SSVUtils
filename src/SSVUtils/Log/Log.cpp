// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <chrono>
#include "SSVUtils/Log/Log.h"
#include "SSVUtils/FileSystem/FileSystem.h"

using namespace std;
using namespace std::chrono;

namespace ssvu
{
	static ostringstream logStream;
	static time_point<high_resolution_clock> start, end;
	Internal::LOut lo{};

	ostringstream& getLogStream() { return logStream; }

	void startBenchmark()
	{
		#ifndef SSVS_DISABLE_LOG
			start = high_resolution_clock::now();
		#endif
	}
	string endBenchmark()
	{
		#ifndef SSVS_DISABLE_LOG
			end = high_resolution_clock::now();
			auto elapsed = duration_cast<milliseconds>(end-start).count();
			return toStr(elapsed) + toStr(" ms");
		#endif
	}
	void saveLogToFile(const FileSystem::Path& mPath)
	{
		#ifndef SSVS_DISABLE_LOG
			ofstream o; o.open(mPath); o << getLogStream().str(); o.flush(); o.close();
		#endif
	}
}

