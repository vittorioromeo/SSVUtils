// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <limits>
#include "SSVUtils/String/Internal/SplitHelper.h"

using namespace std;

namespace ssvu
{
	namespace Internal
	{
		StringSize findFirstOf(const string& mString, const vector<string>& mSeparator, StringSize& mLength)
		{
			StringSize result{numeric_limits<StringSize>::max()};
			for(const auto& s : mSeparator)
			{
				StringSize temp{mString.find(s)};
				if(temp < result) { result = temp; mLength = s.size(); }
			}
			return result;
		}
	}
}
