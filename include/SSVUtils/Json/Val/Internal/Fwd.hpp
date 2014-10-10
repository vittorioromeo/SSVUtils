// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_VAL_INTERNAL_FWD
#define SSVU_JSON_VAL_INTERNAL_FWD

namespace ssvu
{
	namespace Json
	{
		class Val;

		namespace Internal
		{
			template<typename> struct Cnv;
			template<typename> struct Checker;
			template<typename> struct AsHelper;

			namespace Impl
			{
				struct TplHelper;
				struct IsTplHelper;
			}
		}
	}
}

#endif
