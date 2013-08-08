// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <algorithm>
#include "SSVUtils/String/Utils.h"
#include "SSVUtils/Global/Typedefs.h"

using namespace std;

namespace ssvu
{
	unsigned int getCharCount(const string& mString, const char& mChar) { return count(begin(mString), end(mString), mChar); }
	unsigned int getNewLinesCount(const string& mString) { return getCharCount(mString, '\n'); }
	void replace(string& mString, const string& mFrom, const string& mTo)
	{
		size_t startPos{mString.find(mFrom)};
		if(startPos == string::npos) return;
		mString.replace(startPos, mFrom.size(), mTo);
	}
	void replaceAll(string& mString, const string& mFrom, const string& mTo)
	{
		size_t startPos{mString.find(mFrom)};
		while(startPos != string::npos)
		{
			mString.replace(startPos, mFrom.size(), mTo);
			startPos = mString.find(mFrom);
		}
	}
	bool beginsWith(const string& mString, const string& mStart)
	{
		return mStart.size() <= mString.size() && mString.compare(0, mStart.size(), mStart) == 0;
	}
	bool endsWith(const string& mString, const string &mEnding)
	{
		if(mString.size() >= mEnding.size()) return (0 == mString.compare(mString.size() - mEnding.size(), mEnding.size(), mEnding));
		return false;
	}
	string toLower(string mString) { transform(begin(mString), end(mString), begin(mString), ::tolower); return mString; }
	string getReplaced(string mString, const string& mFrom, const string& mTo) { replace(mString, mFrom, mTo); return mString; }
	string getReplacedAll(string mString, const string& mFrom, const string& mTo) { replaceAll(mString, mFrom, mTo); return mString; }

	size_t getLevenshteinDistance(const string& mA, const string& mB)
	{
		const auto& m(mA.size());
		const auto& n(mB.size());

		if(m == 0) return n;
		if(n == 0) return m;

		auto costs(Uptr<size_t[]>(new size_t[n + 1]));
		for(size_t k{0}; k <= n; ++k) costs[k] = k;

		size_t i{0};
		for(auto it1(begin(mA)); it1 != end(mA); ++it1, ++i)
		{
			costs[0] = i + 1;
			auto corner(i);

			size_t j{0};
			for(auto it2(begin(mB)); it2 != end(mB); ++it2, ++j)
			{
				auto upper(costs[j + 1]);
				if(*it1 == *it2) costs[j + 1] = corner;
				else
				{
					const auto& t(upper < corner ? upper : corner);
					costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
				}

				corner = upper;
			}
		}
		return costs[n];
	}
}
