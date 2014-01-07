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
	#define SSVU_PP_IMPL_STRINGIFYWITHCOMMAS_LOOPACTION(mIdx, mData, mArg)	SSVU_PP_STRINGIFY(mArg)SSVU_PP_IF(mIdx, ", ", SSVU_PP_EMPTY())
	#define SSVU_PP_STRINGIFYWITHCOMMAS(...)								SSVU_PP_FOREACH(SSVU_PP_IMPL_STRINGIFYWITHCOMMAS_LOOPACTION, SSVU_PP_EMPTY(), __VA_ARGS__)
}

#endif
