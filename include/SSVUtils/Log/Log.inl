// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_LOG_INL
#define SSVU_LOG_INL

#include "SSVUtils/FileSystem/FileSystem.hpp"
#include "SSVUtils/Utils/Math.hpp"

inline void ssvu::saveLogToFile(const ssvufs::Path& mPath)
{
	#ifndef SSVU_LOG_DISABLE
		std::ofstream o; o.open(mPath); o << ssvu::getLogStream().str(); o.flush(); o.close();
	#endif
}

inline const std::string& ssvu::Internal::getUniqueColor(const std::string& mStr)
{
	static int lastColorIdx{2};
	static std::unordered_map<std::string, Console::Color> map;

	if(map.count(mStr) == 0) map[mStr] = Console::Color(getWrapIdx(lastColorIdx++, 2, 7));
	return Console::setColorFG(map[mStr]);
}

#endif
