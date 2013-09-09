// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_LOG_INL
#define SSVU_LOG_INL

#include <chrono>
#include "SSVUtils/Log/Log.h"
#include "SSVUtils/FileSystem/FileSystem.h"

namespace ssvu
{
	namespace Internal
	{
		static std::ostringstream logStream;
		static std::chrono::time_point<std::chrono::high_resolution_clock> benchStart, benchEnd;
	}

	inline std::ostringstream& getLogStream() { return Internal::logStream; }

	inline void startBenchmark()
	{
		#ifndef SSVU_LOG_DISABLE
			Internal::benchStart = std::chrono::high_resolution_clock::now();
		#endif
	}
	inline std::string endBenchmark()
	{
		#ifndef SSVU_LOG_DISABLE
			Internal::benchEnd = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(Internal::benchEnd - Internal::benchStart).count();
			return toStr(elapsed) + toStr(" ms");
		#endif

		return "";
	}
	inline void saveLogToFile(const FileSystem::Path& mPath)
	{
		#ifndef SSVU_LOG_DISABLE
			std::ofstream o; o.open(mPath); o << getLogStream().str(); o.flush(); o.close();
		#endif
	}
}

#endif
