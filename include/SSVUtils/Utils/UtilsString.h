// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_UTILS_STRING
#define SSVU_UTILS_STRING

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

namespace ssvu
{
	template<typename T> std::string toStr(const T &mValue) { std::ostringstream oss; oss << mValue; return std::string(oss.str()); }
	int getNewLinesCount(const std::string& mString);
	void replace(std::string& mString, const std::string& mFrom, const std::string& mTo);
	void replaceAll(std::string& mString, const std::string& mFrom, const std::string& mTo);
	std::string getReplaced(const std::string& mString, const std::string& mFrom, const std::string& mTo);
	std::string getReplacedAll(const std::string& mString, const std::string& mFrom, const std::string& mTo);
	bool startsWith(const std::string& mString, const std::string& mStart);
	bool endsWith(const std::string &mString, const std::string &mEnding);
	std::string toLower(const std::string &mString);
	std::vector<std::string> split(const std::string& mString, char mSplitter, bool mKeepSplitter = false);
}

#endif
