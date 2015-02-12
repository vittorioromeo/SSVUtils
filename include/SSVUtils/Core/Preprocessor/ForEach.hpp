// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_PREPROCESSOR_FOREACH
#define SSVU_CORE_PREPROCESSOR_FOREACH

#include "SSVUtils/Core/Preprocessor/Utils.hpp"

#define SSVPP_IMPL_FOREACH(mAction, mData, ...)	SSVPP_TKNCAT_2(SSVPP_IMPL_FOREACH_, SSVPP_ARGCOUNT(__VA_ARGS__)) (mAction, mData, __VA_ARGS__)
#define SSVPP_FOREACH(...)						SSVPP_IMPL_FOREACH(__VA_ARGS__)

#endif

// TODO: SSVPP_REPEAT
