// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <algorithm>
#include "SSVUtils/String/Utils.h"

using namespace std;

namespace ssvu
{
	unsigned int getCharCount(const string& mString, const char& mChar) { return count(begin(mString), end(mString), mChar); }
	unsigned int getNewLinesCount(const string& mString) { return getCharCount(mString, '\n'); }
	void replace(string& mString, const string& mFrom, const string& mTo)
	{
		size_t startPos{mString.find(mFrom)};
		if(startPos == string::npos) return;
		mString.replace(startPos, mFrom.length(), mTo);
	}
	void replaceAll(string& mString, const string& mFrom, const string& mTo)
	{
		size_t startPos{mString.find(mFrom)};
		while(startPos != string::npos)
		{
			mString.replace(startPos, mFrom.length(), mTo);
			startPos = mString.find(mFrom);
		}
	}
	bool startsWith(const string& mString, const string& mStart)
	{
		return mStart.size() <= mString.size() && mString.compare(0, mStart.size(), mStart) == 0;
	}
	bool endsWith(const string& mString, const string &mEnding)
	{
		if(mString.length() >= mEnding.length()) return (0 == mString.compare(mString.length() - mEnding.length(), mEnding.length(), mEnding));
		return false;
	}
	string toLower(string mString) { transform(begin(mString), end(mString), begin(mString), ::tolower); return mString; }
	string getReplaced(string mString, const string& mFrom, const string& mTo) { replace(mString, mFrom, mTo); return mString; }
	string getReplacedAll(string mString, const string& mFrom, const string& mTo) { replaceAll(mString, mFrom, mTo); return mString; }
}
