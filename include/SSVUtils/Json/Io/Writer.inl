// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_JSON_IO_WRITER_INL
#define SSVU_JSON_IO_WRITER_INL

namespace ssvu
{
	namespace Json
	{
		namespace Internal
		{
			template<typename TWS> inline void Writer<TWS>::write(const Val& mVal)
			{
				switch(mVal.getType())
				{
					case Val::Type::TObj: write(mVal.as<Obj>()); break;
					case Val::Type::TArr: write(mVal.as<Arr>()); break;
					case Val::Type::TStr: write(mVal.as<Str>()); break;
					case Val::Type::TNum: write(mVal.as<Num>()); break;
					case Val::Type::TBln: write(mVal.as<Bln>()); break;
					case Val::Type::TNll: write(Nll{}); break;
					default: SSVU_UNREACHABLE();
				}
			}
		}
	}
}

#endif
