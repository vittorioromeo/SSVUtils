// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_FILESYSTEM
#define SSVU_FILESYSTEM

#include <dirent.h>
#include <sys/stat.h>
#include <vector>
#include <string>
#include "SSVUtils/Log/Log.h"
#include "SSVUtils/FileSystem/Internal/FileSystemHelper.h"

namespace ssvu
{
	namespace FileSystem
	{
		template<Mode TM = Mode::RECURSIVE, Type TT = Type::ALL, Pick TP = Pick::ANY> void fill(std::vector<std::string>& mTarget, const std::string& mPath, const std::string& mDesired = "")
		{
			FillHelper<TM, TT, TP>::fill(mTarget, mPath, mDesired);
		}
		template<Mode TM = Mode::RECURSIVE, Type TT = Type::ALL, Pick TP = Pick::ANY> std::vector<std::string> get(const std::string& mPath, const std::string& mDesired = "")
		{
			std::vector<std::string> result;
			fill<TM, TT, TP>(result, mPath, mDesired);
			return result;
		}
	}
}

#endif
