// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_PREPROCESSOR_SEP
#define SSVU_CORE_PREPROCESSOR_SEP

#include "SSVUtils/Core/Preprocessor/Utils.hpp"
#include "SSVUtils/Core/Preprocessor/ForEach.hpp"
#include "SSVUtils/Core/Preprocessor/Bool.hpp"

#define SSVPP_IMPL_SEP(mIdx, mData, mArg)	mArg SSVPP_IF(mIdx, mData, SSVPP_EMPTY())
#define SSVPP_SEP(mSeparator, ...)			SSVPP_FOREACH(SSVPP_IMPL_SEP, mSeparator, __VA_ARGS__)

#endif
