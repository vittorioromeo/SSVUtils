// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_PREPROCESSOR_STRINGIFYWITHCOMMAS
#define SSVU_PREPROCESSOR_STRINGIFYWITHCOMMAS

#include "SSVUtils/Preprocessor/Utils.hpp"
#include "SSVUtils/Preprocessor/Bool.hpp"
#include "SSVUtils/Preprocessor/ForEach.hpp"

namespace ssvu
{
	#define SSVPP_IMPL_STRINGIFYWITHCOMMAS(mIdx, mData, mArg)	SSVPP_STRINGIFY(mArg) SSVPP_IF(mIdx, ", ", SSVPP_EMPTY())
	#define SSVPP_STRINGIFYWITHCOMMAS(...)						SSVPP_FOREACH(SSVPP_IMPL_STRINGIFYWITHCOMMAS, SSVPP_EMPTY(), __VA_ARGS__)
}

#endif
