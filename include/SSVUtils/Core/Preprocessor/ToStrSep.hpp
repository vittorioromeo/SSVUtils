// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_CORE_PREPROCESSOR_TOSTR_SEP
#define SSVU_CORE_PREPROCESSOR_TOSTR_SEP

#define SSVPP_IMPL_TOSTR_SEP(mIdx, mData, mArg)	SSVPP_TOSTR(mArg) SSVPP_IF(mIdx, mData, SSVPP_EMPTY())
#define SSVPP_TOSTR_SEP(mSeparator, ...)		SSVPP_FOREACH(SSVPP_IMPL_TOSTR_SEP, mSeparator, __VA_ARGS__)

#endif
