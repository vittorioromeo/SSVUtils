// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_PREPROCESSOR_REPEAT
#define SSVU_CORE_PREPROCESSOR_REPEAT

#include "SSVUtils/Core/Preprocessor/Utils.hpp"

#define SSVPP_IMPL_REPEAT_DEC(mTimes, mAction, mData) SSVPP_TKNCAT_2(SSVPP_IMPL_REPEAT_DEC_, mTimes) (mAction, mData)
#define SSVPP_REPEAT_DEC(...) SSVPP_IMPL_REPEAT_DEC(__VA_ARGS__)

#define SSVPP_IMPL_REPEAT_INC(mTimes, mAction, mData) SSVPP_TKNCAT_2(SSVPP_IMPL_REPEAT_INC_, mTimes) (mAction, mData, 0)
#define SSVPP_REPEAT_INC(...) SSVPP_IMPL_REPEAT_INC(__VA_ARGS__)

#endif
