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
		StringSize findFirstOf(const std::string& mString, const std::vector<std::string>& mSeparator, StringSize& mLength)
		{
			StringSize result{std::numeric_limits<StringSize>::max()};
			for(const auto& s : mSeparator)
			{
				StringSize temp{mString.find(s)};
				if(temp < result) { result = temp; mLength = s.length(); }
			}
			return result;
		}
	}
}

