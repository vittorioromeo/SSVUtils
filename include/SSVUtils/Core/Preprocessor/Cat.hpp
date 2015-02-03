// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_PREPROCESSOR_CAT
#define SSVU_CORE_PREPROCESSOR_CAT

#include "SSVUtils/Core/Preprocessor/Utils.hpp"

#define SSVPP_IMPL_CAT(...)	SSVPP_TKNCAT_2(SSVPP_IMPL_CAT_, SSVPP_ARGCOUNT(__VA_ARGS__)) (__VA_ARGS__)
#define SSVPP_CAT(...)		SSVPP_IMPL_CAT(__VA_ARGS__)

#endif
