// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_PREPROCESSOR_TUPLE
#define SSVU_PREPROCESSOR_TUPLE

#include "SSVUtils/Preprocessor/Utils.hpp"

namespace ssvu
{
	#define SSVU_PP_IMPL_TPL_ELEM_0(m0, ...) m0
	#define SSVU_PP_IMPL_TPL_ELEM_1(m0, m1, ...) m1
	#define SSVU_PP_IMPL_TPL_ELEM_2(m0, m1, m2, ...) m2
	#define SSVU_PP_IMPL_TPL_ELEM_3(m0, m1, m2, m3, ...) m3
	#define SSVU_PP_IMPL_TPL_ELEM_4(m0, m1, m2, m3, m4, ...) m4
	#define SSVU_PP_IMPL_TPL_ELEM_5(m0, m1, m2, m3, m4, m5, ...) m5
	#define SSVU_PP_IMPL_TPL_ELEM_6(m0, m1, m2, m3, m4, m5, m6, ...) m6
	#define SSVU_PP_IMPL_TPL_ELEM_7(m0, m1, m2, m3, m4, m5, m6, m7, ...) m7
	#define SSVU_PP_IMPL_TPL_ELEM_8(m0, m1, m2, m3, m4, m5, m6, m7, m8, ...) m8
	#define SSVU_PP_IMPL_TPL_ELEM_9(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, ...) m9
	#define SSVU_PP_IMPL_TPL_ELEM_10(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, ...) m10

	#define SSVU_PP_IMPL_TPL_ELEM(mIdx, mTpl)	SSVU_PP_TOKENPASTE_2(SSVU_PP_IMPL_TPL_ELEM_, mIdx) mTpl
	#define SSVU_PP_TPL_ELEM(mIdx, mTpl)		SSVU_PP_IMPL_TPL_ELEM(mIdx, mTpl)

	#define SSVU_PP_IMPL_TPL(...)				(__VA_ARGS__)
	#define SSVU_PP_TPL(...)					SSVU_PP_IMPL_TPL(__VA_ARGS__)

	#define SSVU_PP_IMPL_TPL_BREAK(...)			__VA_ARGS__
	#define SSVU_PP_TPL_BREAK(mTpl)				SSVU_PP_EXPAND(SSVU_PP_IMPL_TPL_BREAK mTpl)

	#define SSVU_PP_IMPL_TPL_PREPEND(mX, ...)	SSVU_PP_TPL(mX, __VA_ARGS__)
	#define SSVU_PP_TPL_PREPEND(mX, mTpl)		SSVU_PP_EXPAND(SSVU_PP_IMPL_TPL_PREPEND(mX, SSVU_PP_TPL_BREAK(mTpl)))

	#define SSVU_PP_IMPL_TPL_APPEND(mX, ...)	SSVU_PP_TPL(__VA_ARGS__, mX)
	#define SSVU_PP_TPL_APPEND(mX, mTpl)		SSVU_PP_EXPAND(SSVU_PP_IMPL_TPL_APPEND(mX, SSVU_PP_TPL_BREAK(mTpl)))
}

#endif
