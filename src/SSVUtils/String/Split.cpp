// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <algorithm>
#include "SSVUtils/String/Split.h"
#include "SSVUtils/String/Internal/SplitHelper.h"

using namespace std;

namespace ssvu
{
	void split(vector<string>& mTarget, const string& mString, char mSeparator)						{ Internal::SplitHelper<char>::split(mTarget, mString, mSeparator); }
	void split(vector<string>& mTarget, const string& mString, const vector<char>& mSeparator)		{ Internal::SplitHelper<vector<char>>::split(mTarget, mString, mSeparator); }
	void split(vector<string>& mTarget, const string& mString, const string& mSeparator)			{ Internal::SplitHelper<string>::split(mTarget, mString, mSeparator); }
	void split(vector<string>& mTarget, const string& mString, const vector<string>& mSeparator)	{ Internal::SplitHelper<vector<string>>::split(mTarget, mString, mSeparator); }

	vector<string> getSplit(const string& mString, char mSeparator)						{ vector<string> result; split(result, mString, mSeparator); return result; }
	vector<string> getSplit(const string& mString, const vector<char>& mSeparator)		{ vector<string> result; split(result, mString, mSeparator); return result; }
	vector<string> getSplit(const string& mString, const string& mSeparator)			{ vector<string> result; split(result, mString, mSeparator); return result; }
	vector<string> getSplit(const string& mString, const vector<string>& mSeparator)	{ vector<string> result; split(result, mString, mSeparator); return result; }
}
