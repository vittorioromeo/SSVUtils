// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_STRINGIFIER
#define SSVU_JSON_STRINGIFIER

namespace ssvu
{
	template<> struct Stringifier<Json::Val>
	{
		template<bool TFmt> inline static void impl(std::ostream& mStream, const Json::Val& mVal)
		{
			mVal.writeToStream<Json::WriterSettings<TFmt, true>>(mStream);
		}
	};
}

#endif
