// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "Utils/UtilsString.h"

using namespace std;

namespace ssvu
{
	namespace Utils
	{
		int countNewLines(const string& mString) { int result{0}; for(auto c : mString) if(c == '\n') result++; return result; }
		string replace(const string& mString, const string& mFrom, const string& mTo)
		{
			string result{mString};
			size_t start_pos = result.find(mFrom);
			if(start_pos == string::npos) return result;
			result.replace(start_pos, mFrom.length(), mTo);
			return result;
		}
		string replaceAll(const string& mString, const string& mFrom, const string& mTo)
		{
			string result{mString};
			size_t start_pos = result.find(mFrom);
			while(start_pos != string::npos)
			{
				result.replace(start_pos, mFrom.length(), mTo);
				start_pos = result.find(mFrom);
			}
			return result;
		}
		bool startsWith(const string& mString, const string& mStart)
		{
			return mStart.size() <= mString.size() && mString.compare(0, mStart.size(), mStart) == 0;
		}
		bool endsWith(const string &mString, const string &mEnding)
		{
			if(mString.length() >= mEnding.length()) return (0 == mString.compare(mString.length() - mEnding.length(), mEnding.length(), mEnding));
			return false;
		}
		string toLower(const string &mString) { string result{mString}; transform(begin(result), end(result), begin(result), ::tolower); return result; }
		vector<string> split(const string& mString, char mSplitter, bool mKeepSplitter)
		{
			vector<string> result;
			unsigned int i{0}, j{0}, length{mString.length()};

			for(i = 0; i < length + 1; ++i)
				if(mString[i] == mSplitter || i == length)
				{
					string subString{mString.substr(j, i - j)};
					if(!subString.empty()) result.push_back(subString);
					j = i;
					if(!mKeepSplitter) ++j;
				}

			return result;
		}
	}
}

