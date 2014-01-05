// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVU_PREPROCESSOR_FOREACH
#define SSVU_PREPROCESSOR_FOREACH

#include "SSVUtils/Preprocessor/Utils.hpp"

namespace ssvu
{
	#define SSVU_PP_IMPL_FOREACH_0(...)					SSVU_PP_EMPTY()
	#define SSVU_PP_IMPL_FOREACH_1(mAction, mArg)		mAction(mArg)
	#define SSVU_PP_IMPL_FOREACH_2(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_1(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_3(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_2(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_4(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_3(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_5(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_4(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_6(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_5(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_7(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_6(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_8(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_7(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_9(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_8(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_10(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_9(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_11(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_10(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_12(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_11(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_13(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_12(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_14(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_13(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_15(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_14(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_16(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_15(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_17(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_16(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_18(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_17(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_19(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_18(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_20(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_19(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_21(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_20(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_22(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_21(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_23(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_22(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_24(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_23(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_25(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_24(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_26(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_25(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_27(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_26(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_28(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_27(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_29(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_28(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_30(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_29(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_31(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_30(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_32(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_31(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_33(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_32(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_34(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_33(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_35(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_34(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_36(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_35(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_37(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_36(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_38(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_37(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_39(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_38(mAction, __VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_40(mAction, mArg, ...)	mAction(mArg) SSVU_PP_IMPL_FOREACH_39(mAction, __VA_ARGS__)

	#define SSVU_PP_IMPL_FOREACH_REVERSE_SEQUENCE()	SSVU_PP_IMPL_FOREACH_40, \
													SSVU_PP_IMPL_FOREACH_39, \
													SSVU_PP_IMPL_FOREACH_38, \
													SSVU_PP_IMPL_FOREACH_37, \
													SSVU_PP_IMPL_FOREACH_36, \
													SSVU_PP_IMPL_FOREACH_35, \
													SSVU_PP_IMPL_FOREACH_34, \
													SSVU_PP_IMPL_FOREACH_33, \
													SSVU_PP_IMPL_FOREACH_32, \
													SSVU_PP_IMPL_FOREACH_31, \
													SSVU_PP_IMPL_FOREACH_30, \
													SSVU_PP_IMPL_FOREACH_29, \
													SSVU_PP_IMPL_FOREACH_28, \
													SSVU_PP_IMPL_FOREACH_27, \
													SSVU_PP_IMPL_FOREACH_26, \
													SSVU_PP_IMPL_FOREACH_25, \
													SSVU_PP_IMPL_FOREACH_24, \
													SSVU_PP_IMPL_FOREACH_23, \
													SSVU_PP_IMPL_FOREACH_22, \
													SSVU_PP_IMPL_FOREACH_21, \
													SSVU_PP_IMPL_FOREACH_20, \
													SSVU_PP_IMPL_FOREACH_19, \
													SSVU_PP_IMPL_FOREACH_18, \
													SSVU_PP_IMPL_FOREACH_17, \
													SSVU_PP_IMPL_FOREACH_16, \
													SSVU_PP_IMPL_FOREACH_15, \
													SSVU_PP_IMPL_FOREACH_14, \
													SSVU_PP_IMPL_FOREACH_13, \
													SSVU_PP_IMPL_FOREACH_12, \
													SSVU_PP_IMPL_FOREACH_11, \
													SSVU_PP_IMPL_FOREACH_10, \
													SSVU_PP_IMPL_FOREACH_9, \
													SSVU_PP_IMPL_FOREACH_8, \
													SSVU_PP_IMPL_FOREACH_7, \
													SSVU_PP_IMPL_FOREACH_6, \
													SSVU_PP_IMPL_FOREACH_5, \
													SSVU_PP_IMPL_FOREACH_4, \
													SSVU_PP_IMPL_FOREACH_3, \
													SSVU_PP_IMPL_FOREACH_2, \
													SSVU_PP_IMPL_FOREACH_1, \
													SSVU_PP_IMPL_FOREACH_0

	#define SSVU_PP_IMPL_FOREACH_DISPATCH_IMPL(	m01, m02, m03, m04, m05, m06, m07, m08, m09, m10, m11, m12, m13, m14, m15, m16, m17, m18, m19, m20, \
												m21, m22, m23, m24, m25, m26, m27, m28, m29, m30, m31, m32, m33, m34, m35, m36, m37, m38, m39, m40, mN, ...) mN

	#define SSVU_PP_IMPL_FOREACH_DISPATCH(...)		SSVU_PP_IMPL_FOREACH_DISPATCH_IMPL(__VA_ARGS__)
	#define SSVU_PP_IMPL_FOREACH_IMPL(mAction, ...)	SSVU_PP_IMPL_FOREACH_DISPATCH(__VA_ARGS__, SSVU_PP_IMPL_FOREACH_REVERSE_SEQUENCE()) (mAction, __VA_ARGS__)

	#define SSVU_PP_FOREACH(...)					SSVU_PP_IMPL_FOREACH_IMPL(__VA_ARGS__)
}

#endif
